#ifndef CHEUKYIN_RAYTRACER_AST_H
#define CHEUKYIN_RAYTRACER_AST_H

#include <map>
#include <string>
#include <vector>

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

    } // namespace Parser
} // namespace RayTracer

#endif // CHEUKYIN_RAYTRACER_AST_H
