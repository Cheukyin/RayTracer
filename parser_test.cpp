#include "unittest.hpp"
#include "parser.hpp"
#include "xmlparser.hpp"

using namespace RayTracer::Parser;

TEST_CASE(TestParserBasicFunctions)
{
    string str = "ab cdef\nswef\t  \f \v\n 3567= 29";
    str_const_iter_t iter = str.begin() + 2;

    EXPECT_TRUE(is_char_in_str('a', str));
    EXPECT_FALSE(is_char_in_str('k', str));

    EXPECT_EQ(peek(iter), 'c');
    EXPECT_EQ(*iter, ' ');

    skip_whitespace(iter);
    EXPECT_EQ(*iter, 'c');

    skip_word(iter, "cre");
    EXPECT_EQ(*iter, 'c');
    skip_word(iter, "cdef\nswef\t  \f \v\n 4");
    EXPECT_EQ(*iter, 'c');
    skip_word(iter, "cdef\nswe");
    EXPECT_EQ(*iter, 'f');

    skip_whitespace(iter);
    EXPECT_EQ(*iter, 'f');
    skip_word(iter, "f");
    EXPECT_EQ(*iter, '\t');
    skip_whitespace(iter);
    EXPECT_EQ(*iter, '3');

    walk_until_any(iter, " \t=");
    EXPECT_EQ(*iter, '=');
    walk_until_any(iter, " ");
    EXPECT_EQ(*iter, ' ');

    walk_until_any(iter, '9');
    EXPECT_EQ(*iter, '9');

    walk_until_any(iter, "8");
    EXPECT_EQ(*iter, 0);
    skip_word(iter, "001");
    EXPECT_EQ(*iter, 0);
}

TEST_CASE(TestXmlParser)
{
    // XmlParser a;
    // TEST_PRINT(a.content);
}