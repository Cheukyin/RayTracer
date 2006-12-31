#ifndef CHEUKYIN_RAYTRACER_XMLPARSER_H
#define CHEUKYIN_RAYTRACER_XMLPARSER_H

#include "parser.hpp"
#include "ast.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

namespace RayTracer{
    namespace Parser
    {
        // <S>                -> <NODE>*
        // <NODE>             -> <NORMAL_NODE> | <SELF_CLOSED_NODE>
        // <NORMAL_NODE>      -> <START_TAG> <NODE>* <CLOSED_TAG>
        // <SELF_CLOSED_NODE> -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* <SLASH> <RIGHT_ANGLE>
        // <START_TAG>        -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* <RIGHT_ANGLE>
        // <CLOSED_TAG>       -> <LEFT_ANGLE> <SLASH> <WORD> <RIGHT_ANGLE>
        // <ATTRIBUTE>        -> <WORD> <EQUAL> <QUOT> <STRING> <QUOT>

        // <WORD>             -> any character sequence not containing witespace or >
        // <STRING>           -> any character sequence not containing " or '
        // <WHITE_SPACE>      -> "\t\v\f\n "
        // <EQUAL>            -> "="
        // <LEFT_ANGLE>       -> "<"
        // <RIGHT_ANGLE>      -> ">"
        // <SLASH>            -> "/"
        // <QUOT>             -> '"
        class XmlParser
        {
        public:
            XmlParser() {}

            XmlParser(const string& config_path)
            {
                std::ifstream ifs(config_path, std::ios::in);
                content = string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
                content_iter = content.begin();
                ifs.close();
            }

            void set_content(const string& str) // for unittest
            {
                content = str;
            }

            // <S> -> <NODE>*
            vector<Node*> parse()
            {
                vector<Node*> vec_node;
                try
                {
                    while(1)
                        vec_node.push_back( parse_node() );
                }
                catch(EOFException e)
                { return vec_node; }
                catch(ErrorException e)
                {
                    std::cerr << e.msg << std::endl;
                    exit(1);
                }
            }

        private:
            string content;
            str_const_iter_t content_iter;
            std::stack<string> tag_stack;

            // <NODE>             -> <NORMAL_NODE> | <SELF_CLOSED_NODE>
            // <NORMAL_NODE>      -> <START_TAG> <NODE>* <CLOSED_TAG>
            // <SELF_CLOSED_NODE> -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* <SLASH> <RIGHT_ANGLE>
            // <START_TAG>        -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* <RIGHT_ANGLE>
            // <CLOSED_TAG>       -> <LEFT_ANGLE> <SLASH> <WORD> <RIGHT_ANGLE>
            // <ATTRIBUTE>        -> <WORD> <EQUAL> <QUOT> <STRING> <QUOT>

            // ======>

            // <NODE> -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* ( <NORMAL_NODE_LEFT> | <SELF_CLOSED_NODE_LEFT> )
            // <SELF_CLOSED_NODE_LEFT> -> <SLASH> <RIGHT_ANGLE>
            // <NORMAL_NODE_LEFT> -> <RIGHT_ANGLE> <NODE>* <CLOSED_TAG>

            Node* parse_node()
            {
                EleNode* ele_node(new EleNode);

                // parse <
                walk_until_any(content_iter, LEFT_ANGLE);
                skip_whitespace(content_iter);
                if( !*content_iter )
                    throw new EOFException;

                // parse tag_name
                auto start_iter = content_iter;
                walk_until_any(content_iter, WHITE_SPACE+RIGHT_ANGLE+SLASH);
                ele_node->tag_name = string(start_iter, content_iter);

                skip_whitespace(content_iter);

                // parse attributes
                while( (*content_iter != 0)
                    && (*content_iter != RIGHT_ANGLE)
                    && (*content_iter != SLASH) )
                {
                    skip_whitespace(content_iter);
                    parse_attr(ele_node->attr_kv);
                }
                if( !*content_iter )
                    throw new ErrorException{"EOF occurs when parsing start tag!"};

                // end of start tag, parse the left
                if(*content_iter == RIGHT_ANGLE)
                {
                    parse_normal_node_left(ele_node);
                    return reinterpret_cast<Node*>(ele_node);
                }

                // self closed tag
                if(*content_iter == SLASH)
                { return parse_normal_node_left(ele_node); }
            }

            Node* parse_normal_node_left(EleNode*& ele_node)
            {
            }

            // <SELF_CLOSED_NODE_LEFT> -> <SLASH> <RIGHT_ANGLE>
            Node* parse_self_closed_node_left(EleNode*& ele_node)
            {
            }

            Node* parse_attr(map<string, string>& attr_kv)
            {
                skip_whitespace(content_iter);

                auto start_iter = content_iter;
                walk_until_any(content_iter, EQUAL);
            }

            Node* parse_start_tag()
            {
                skip_whitespace(content_iter);
            }

            Node* parse_closed_tag()
            {
            }

        }; // class XmlParser
    } // namespace Parser
} // namespace RayTracer

#endif // XMLPARSER