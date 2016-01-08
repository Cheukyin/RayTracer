#ifndef CHEUKYIN_RAYTRACER_AST_H
#define CHEUKYIN_RAYTRACER_AST_H

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace RayTracer{
    namespace Parser
    {
        using std::string;
        using std::vector;
        using std::map;
        using std::shared_ptr;

        struct Node
        {
        };

        struct TextNode: public Node
        {
            string text;
        };

        struct EleNode: public Node
        {
            string tag_name;
            map<string, string> attr_kv;
            vector< shared_ptr<Node> > text;
        };

    } // namespace Parser
} // namespace RayTracer

#endif // CHEUKYIN_RAYTRACER_AST_H