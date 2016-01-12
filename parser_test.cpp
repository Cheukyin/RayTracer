#include "unittest.hpp"
#include "parser.hpp"
#include "xmlparser.hpp"
#include "ast.hpp"

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

TEST_CASE(TestNodeCoercion)
{
    TextNode *tnp = new TextNode;
    tnp->text = "text";

    EleNode *enp = new EleNode;
    enp->tag_name = "enp";
    enp->attr_kv["k"] = "v";
    enp->subnodes.push_back( reinterpret_cast<Node*>(tnp) );

    Node *np = reinterpret_cast<Node*>(tnp);
    EXPECT_EQ(reinterpret_cast<TextNode*>(np)->text, "text");

    np = reinterpret_cast<Node*>(enp);
    EXPECT_EQ(reinterpret_cast<EleNode*>(np)->tag_name, "enp");
    EXPECT_EQ(reinterpret_cast<EleNode*>(np)->attr_kv["k"], "v");

    np = reinterpret_cast<EleNode*>(np)->subnodes[0];
    EXPECT_EQ(reinterpret_cast<TextNode*>(np)->text, "text");
}

TEST_CASE(TestXmlParser)
{
    EleNode *root = new EleNode;

    XmlParser p;

    // ----------------------------------------------------------------
    p.set_content(R"(   < surface >  <  / surface  >  )");
    root->subnodes = p.parse();

    EXPECT_EQ(reinterpret_cast<EleNode*>(root->subnodes[0])->tag_name,
              "surface");

    // ----------------------------------------------------------------
    p.set_content(R"(   < shader /  >  )");
    root->subnodes = p.parse();

    EXPECT_EQ(reinterpret_cast<EleNode*>(root->subnodes[0])->tag_name,
              "shader");

    // ----------------------------------------------------------------
    p.set_content(R"(   < surface  k1="v1"  k2 =   'v2' >

                          < shader >
                          < /shader >
                            <radius >
                              <abc k4="v4" k5 ="v5"> <alpha> < / alpha > qie <ab  k3  =   'y2'  /> in <db > </db> < / abc>
                                <dbc> </ dbc>
                            </radius>

                          <  center > < / center >
                        <  / surface  >  )");
    root->subnodes = p.parse();

    EleNode *n0 = reinterpret_cast<EleNode*>(root->subnodes[0]); // <surface>...</surface>
    EXPECT_EQ(n0->attr_kv["k1"], "v1");
    EXPECT_EQ(n0->attr_kv["k2"], "v2");
    EXPECT_EQ(reinterpret_cast<EleNode*>(n0->subnodes[0])->tag_name, "shader");
    EXPECT_EQ(reinterpret_cast<EleNode*>(n0->subnodes[1])->tag_name, "radius");
    EXPECT_EQ(reinterpret_cast<EleNode*>(n0->subnodes[2])->tag_name, "center");

    EleNode *n1 = reinterpret_cast<EleNode*>(n0->subnodes[1]); // <radius>...</radius>
    EXPECT_EQ(reinterpret_cast<EleNode*>(n1->subnodes[0])->tag_name, "abc");
    EXPECT_EQ(reinterpret_cast<EleNode*>(n1->subnodes[1])->tag_name, "dbc");

    EleNode *n2 = reinterpret_cast<EleNode*>(n1->subnodes[0]); // <abc>...</abc>
    EXPECT_EQ(n2->attr_kv["k4"], "v4");
    EXPECT_EQ(n2->attr_kv["k5"], "v5");
    EXPECT_EQ(reinterpret_cast<EleNode*>(n2->subnodes[0])->tag_name, "alpha");

    TextNode *n3 = reinterpret_cast<TextNode*>(n2->subnodes[1]); // qie
    EXPECT_EQ(n3->text, "qie");

    EXPECT_EQ(reinterpret_cast<EleNode*>(n2->subnodes[2])->tag_name, "ab");
    EXPECT_EQ(reinterpret_cast<EleNode*>(n2->subnodes[2])->attr_kv["k3"], "y2");

    TextNode *n5 = reinterpret_cast<TextNode*>(n2->subnodes[3]); // in
    EXPECT_EQ(n5->text, "in");

    EXPECT_EQ(reinterpret_cast<EleNode*>(n2->subnodes[4])->tag_name, "db");

    // ----------------------------------------------------------------
}