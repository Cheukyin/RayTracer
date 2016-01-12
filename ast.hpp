#ifndef CHEUKYIN_RAYTRACER_AST_H
#define CHEUKYIN_RAYTRACER_AST_H

#include <map>
#include <string>
#include <vector>
#include <iostream>

namespace RayTracer{
    namespace Parser
    {
        using std::string;
        using std::vector;
        using std::map;

        enum class NodeType
        {ELE, TEXT};

        struct Node
        { NodeType type; };

        struct TextNode
        {
            NodeType type;
            string text;

            TextNode()
            : type(NodeType::TEXT)
            {}
        };

        struct EleNode
        {
            NodeType type;
            string tag_name;
            map<string, string> attr_kv;
            vector<Node*> subnodes;

            EleNode()
            : type(NodeType::ELE)
            {}
        };

        inline void free_node(Node *root)
        {
            if(root->type == NodeType::TEXT)
                delete reinterpret_cast<TextNode*>(root);
            else // element type
            {
                EleNode *ele_node = reinterpret_cast<EleNode*>(root);
                for(auto each : ele_node->subnodes)
                    free_node(each);
                delete ele_node;
            }
        }

    } // namespace Parser
} // namespace RayTracer

#endif // CHEUKYIN_RAYTRACER_AST_H