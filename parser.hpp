#ifndef CHEUKYIN_RAYTRACER_PARSER_H
#define CHEUKYIN_RAYTRACER_PARSER_H

#include <string>

namespace RayTracer{
    namespace Parser
    {
        using std::string;
        typedef string::const_iterator str_const_iter_t;

        const char LEFT_ANGLE = '<';
        const char RIGHT_ANGLE = '>';
        const char EQUAL = '=';
        const string WHITE_SPACE = "\t\v\f\n ";
        const string QUOT = "'\"";

        inline bool is_char_in_str(char ch, const string& str)
        {
            for(const auto iter : str)
                if(ch == iter)
                    return true;
            return false;
        }

        inline char peek(str_const_iter_t iter)
        {
            return *(iter+1);
        }

        inline void skip_whitespace(str_const_iter_t& iter)
        {
            while( *iter && is_char_in_str(*iter, WHITE_SPACE) )
                iter++;
        }

        inline void skip_word(str_const_iter_t& iter, const string& word)
        {
            str_const_iter_t origin_iter = iter;
            str_const_iter_t expect_iter = iter + word.size();
            str_const_iter_t word_iter = word.begin();

            while( *iter && *word_iter && (*iter == *word_iter) )
            {
                iter++;
                word_iter++;
            }

            if(iter != expect_iter)
                iter = origin_iter;
        }

    } // namespace Parser
} // namespace RayTracer


#endif // CHEUKYIN_RAYTRACER_PARSER_H
