#ifndef CHEUKYIN_RAYTRACER_XMLPARSER_H
#define CHEUKYIN_RAYTRACER_XMLPARSER_H

#include "parser.hpp"
#include "ast.hpp"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

namespace RayTracer{
    namespace Parser
    {
        // <S>                -> <NODE>*
        // <NODE>             -> <NORMAL_NODE> | <SELF_CLOSED_NODE> | <TEXT>
        // <NORMAL_NODE>      -> <START_TAG> <NODE>* <CLOSED_TAG>
        // <SELF_CLOSED_NODE> -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* <SLASH> <RIGHT_ANGLE>
        // <START_TAG>        -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* <RIGHT_ANGLE>
        // <CLOSED_TAG>       -> <LEFT_ANGLE> <SLASH> <WORD> <RIGHT_ANGLE>
        // <ATTRIBUTE>        -> <WORD> <EQUAL> <QUOT> <STRING> <QUOT>

        // <WORD>             -> any character sequence not containing witespace or >
        // <TEXT>             -> any character sequence not containing <
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
                content = string(std::istreambuf_iterator<char>(ifs),
                                 std::istreambuf_iterator<char>());
                content_iter = content.begin();
                ifs.close();
            }

            void set_content(const string& str) // for unittest
            {
                content = str;
                content_iter = content.begin();
            }

            // <S> -> <NODE>*
            Node* parse()
            {
                EleNode *root = new EleNode;
                try
                {
                    while(1)
                    {
                        skip_whitespace(content_iter);
                        walk_until_any(content_iter, LEFT_ANGLE);
                        if( !*content_iter )
                            throw EOFException();
                        root->subnodes.push_back( parse_node(new EleNode) );
                    }
                }
                catch(EOFException e)
                { return reinterpret_cast<Node*>(root); }
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

            // <NODE>             -> <NORMAL_NODE> | <SELF_CLOSED_NODE> | <TEXT>
            // <NORMAL_NODE>      -> <START_TAG> <NODE>* <CLOSED_TAG>
            // <SELF_CLOSED_NODE> -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* <SLASH> <RIGHT_ANGLE>
            // <START_TAG>        -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* <RIGHT_ANGLE>
            // <CLOSED_TAG>       -> <LEFT_ANGLE> <SLASH> <WORD> <RIGHT_ANGLE>
            // <ATTRIBUTE>        -> <WORD> <EQUAL> <QUOT> <STRING> <QUOT>

            // ======>

            // <NODE> -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* ( <NORMAL_NODE_LEFT> | <SELF_CLOSED_NODE_LEFT> )
            // <SELF_CLOSED_NODE_LEFT> -> <SLASH> <RIGHT_ANGLE>
            // <NORMAL_NODE_LEFT> -> <RIGHT_ANGLE> <NODE>* <CLOSED_TAG>

            Node* parse_node(EleNode* ele_node)
            {
                // parse text node
                if(*content_iter != LEFT_ANGLE)
                {
                    TextNode *tn = new TextNode;

                    skip_whitespace(content_iter);
                    auto start_iter = content_iter;
                    walk_until_any(content_iter, WHITE_SPACE+LEFT_ANGLE);
                    if( !*content_iter )
                        throw ErrorException{"EOF occurs when parsing Text!"};

                    tn->text = string(start_iter, content_iter);
                    skip_whitespace(content_iter);

                    return reinterpret_cast<Node*>(tn);
                }

                // parse element node
                assert(*content_iter == LEFT_ANGLE);

                content_iter++;
                skip_whitespace(content_iter);
                if( !*content_iter )
                    throw EOFException();
                if(*content_iter == SLASH)
                    throw ErrorException{"SLASH occurs when parsing start tag!"};

                // parse tag_name
                auto start_iter = content_iter;
                walk_until_any(content_iter, WHITE_SPACE+RIGHT_ANGLE+SLASH);
                ele_node->tag_name = string(start_iter, content_iter);
                tag_stack.push(ele_node->tag_name);

                skip_whitespace(content_iter);
                // parse attributes
                while( (*content_iter != 0)
                    && (*content_iter != RIGHT_ANGLE)
                    && (*content_iter != SLASH) )
                {
                    parse_attr(ele_node->attr_kv);
                    skip_whitespace(content_iter);
                }
                if( !*content_iter )
                    throw ErrorException{"EOF occurs when parsing start tag!"};

                // end of start tag, parse the left
                if(*content_iter == RIGHT_ANGLE)
                    return parse_normal_node_left(ele_node);

                // self closed tag
                if(*content_iter == SLASH)
                    parse_self_closed_node_left();
                return reinterpret_cast<Node*>(ele_node);
            }

            // <NORMAL_NODE_LEFT> -> <RIGHT_ANGLE> <NODE>* <CLOSED_TAG>
            Node* parse_normal_node_left(EleNode* ele_node)
            {
                assert(*content_iter == RIGHT_ANGLE);
                content_iter++;

                // parse <NODE>*
                while(1)
                {
                    skip_whitespace(content_iter);
                    auto iter = content_iter;

                    // parse text node
                    if(*content_iter != LEFT_ANGLE)
                    {
                        ele_node->subnodes.push_back( parse_node(new EleNode) );
                        continue;
                    }

                    // parse <, element node
                    // peek
                    content_iter++;
                    skip_whitespace(content_iter);
                    // check if < is followed by /
                    if(*content_iter == SLASH) // it's a closed tag
                        break;
                    if( !*content_iter )
                        throw ErrorException{"EOF occurs when parsing inside a tag!"};

                    // it's a start tag
                    content_iter = iter; // recover content_iter, back to LEFT_ANGLE
                    ele_node->subnodes.push_back( parse_node(new EleNode) );
                }

                // parse closed tag
                assert(*content_iter == SLASH);

                content_iter++;
                skip_whitespace(content_iter);

                auto start_iter = content_iter;
                walk_until_any(content_iter, WHITE_SPACE+RIGHT_ANGLE);
                string str(start_iter, content_iter);
                if(tag_stack.top() == str)
                    tag_stack.pop();
                else
                    throw ErrorException{"Name of end tag doesn't match!"};
                skip_whitespace(content_iter);

                assert(*content_iter == RIGHT_ANGLE);
                content_iter++;
                return reinterpret_cast<Node*>(ele_node);
            }

            // <SELF_CLOSED_NODE_LEFT> -> <SLASH> <RIGHT_ANGLE>
            void parse_self_closed_node_left()
            {
                assert(*content_iter == SLASH);

                walk_until_any(content_iter, RIGHT_ANGLE);
                if( !*content_iter )
                    throw ErrorException{"EOF occurs when parsing self closed tag!"};

                tag_stack.pop();
                assert(*content_iter == RIGHT_ANGLE);
                content_iter++;
            }

            void parse_attr(map<string, string>& attr_kv)
            {
                assert(*content_iter != RIGHT_ANGLE
                    && *content_iter != SLASH
                    && !is_char_in_str(*content_iter, WHITE_SPACE));

                // parse key
                auto start_iter = content_iter;
                walk_until_any(content_iter, WHITE_SPACE+EQUAL);
                string key(start_iter, content_iter);

                // parse =
                skip_whitespace(content_iter);
                if(*content_iter != EQUAL)
                    throw ErrorException{"= missing when parsing " + key};
                content_iter++;
                walk_until_any(content_iter, QUOT);

                // parse value
                start_iter = ++content_iter;
                walk_until_any(content_iter, QUOT);
                string val(start_iter, content_iter);

                // store it
                attr_kv[key] = val;

                content_iter++;
            }

        }; // class XmlParser
    } // namespace Parser
} // namespace RayTracer

#endif // XMLPARSER