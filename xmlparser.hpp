#ifndef CHEUKYIN_RAYTRACER_XMLPARSER_H
#define CHEUKYIN_RAYTRACER_XMLPARSER_H

#include <string>

namespace RayTracer{
    namespace Parser
    {
        using std::string;

        // <S>                -> <NODE>*
        // <NODE>             -> <NORMAL_NODE> | <SELF_CLOSED_NODE>
        // <NORMAL_NODE>      -> <START_TAG> <NODE>* <CLOSED_TAG>
        // <SELF_CLOSED_NODE> -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* <SLASH> <RIGHT_ANGLE>
        // <START_TAG>        -> <LEFT_ANGLE> <WORD> <ATTRIBUTE>* <RIGHT_ANGLE>
        // <CLOSED_TAG>       -> <LEFT_ANGLE> <SLASH> <WORD> <RIGHT_ANGLE>
        // <ATTRIBUTE>        -> <WORD> <EQUAL> <QUOT> <STRING> <QUOT>

        // <WORD>             -> any character sequence not containing witespace
        // <STRING>           -> any character sequence not containing " or '
        // <WHITE_SPACE>      -> "\t\v\f "
        // <EQUAL>            -> "="
        // <LEFT_ANGLE>       -> "<"
        // <RIGHT_ANGLE>      -> ">"
        // <SLASH>            -> "/"
        class XmlParser
        {
        public:
            XmlParser(const string& xmlconfig);

        private:
            string xmlconfig;

        }; // class XmlParser
    } // namespace Parser
} // namespace RayTracer

#endif // XMLPARSER