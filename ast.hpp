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

        struct Node
        { virtual ~Node() {} };

        struct TextNode : Node
        { string text; };

        struct EleNode : Node
        {
            string tag_name;
            map<string, string> attr_kv;
            vector<Node*> subnodes;

            virtual ~EleNode() override
            {
                for (Node* eachNode : subnodes)
                    delete eachNode;
            }
        };

    } // namespace Parser
} // namespace RayTracer

#endif // CHEUKYIN_RAYTRACER_AST_H