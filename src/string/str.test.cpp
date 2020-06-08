#include "../debug/test_def.h"
#include "str.h"
#include "../other/defdef.h"
#include "../other/rand.h"

TEST_PACK(str)
{
    TEST(create)
    {
        str a("1");
        str b = "Hello, World!";
        str c;
        str d(c);
        str e = d;
    }

    TEST(convert)
    {
        std::string a = "string";
        str b = a;
        char *c = b;
        UNUSED(c);
        const str d = b;
        const char *e = d;
        const char *f = d.c_str();
        EXPECT_CSTRING_EQ(e, f);
        delete [] c;
        delete [] e; //convert to char* always creates new array(use c_str instead)
    }

    TEST(compare)
    {
        std::string a = "a";
        std::string b = "";
        str c;
        str d = "b";
        str e = "b";
        str f = d;
        EXPECT_EQ(a, str("a"));
        EXPECT_EQ(b, c);
        EXPECT_NE(a, c);
        EXPECT_NE(a, d);
        EXPECT_NE(c, d);
        EXPECT_EQ(c, str());
        EXPECT_EQ(d, "b");
        EXPECT_EQ(f, d);
        EXPECT_EQ(d, e);
        EXPECT_EQ(f, "b");
        f = "";
        EXPECT_EQ(c, f);
    }

    TEST(access)
    {
        str a = "012345";
        EXPECT_EQ(a[4], '4');
        EXPECT_EQ(a[2], '2');
        EXPECT_EQ(a[0], '0');
        EXPECT_EQ(a[5], '5');
    }

    TEST(substr_access)
    {
        str a = "012345";
        str b = a.substr(3);
        str c = a.substr(1, 4);
        EXPECT_EQ(b, "345");
        EXPECT_EQ(c, "123");
        EXPECT_EQ(b[0], '3');
        EXPECT_EQ(c[2], b[0]);
    }

    TEST(concatenation)
    {
        str a = "1";
        str b = "2";
        str c = "3";
        str d = a + b;
        str e = str("1") + "2";
        str f = d;
        EXPECT_EQ(d, e);
        EXPECT_EQ(f, "12");
        EXPECT_EQ(e, f);
        str g = a + b + c;
        str h = d + "3";
        str j = f + c;
        EXPECT_EQ(g, h);
        EXPECT_EQ(g, j);
        EXPECT_EQ(j, "123");
        a = d;
        b = c;
        EXPECT_EQ(d, "12");
        EXPECT_EQ(a, "12");
        EXPECT_EQ(c, b);
        EXPECT_EQ(b, "3");
        str k = "aa";
        EXPECT_EQ(k + k, "aaaa");
        str k2 = k + k;
        EXPECT_CSTRING_EQ(k2.c_str(), "aaaa");
    }

    TEST(multiplication)
    {
        str _1000 = "0";
        _1000 *= 1000;
        EXPECT_EQ(_1000.length(), 1000);
        EXPECT_EQ(_1000.at(999), '0');
    }

    TEST(concatenation_memory)
    {
        str a = "1";
        str b = "2";
        for(int i = 0;i < 40; i++)
            if(i & 1)
                a += b;
            else
                b += a;
        EXPECT_EQ(b.length(), 165580141); //41th fibonacci number
        EXPECT_EQ(b.at(312), '2');
    }

    TEST(concatenation_speed)
    {
        str a;
        str one = "1";
        str two = "2";
        str three = "3";
        for(int i = 0;i < 10000;i++)
            if(i & 1)
                a += one;
            elif(i & 2)
                a += two;
            else
                a += three;
        EXPECT_EQ(a.length(), 10000);
        EXPECT_EQ(a.at(0), '3');
        EXPECT_EQ(a.at(797), '1');
    }

    TEST(substr)
    {
        str a = "1234567890";
        str b = a.substr(0, 5);
        EXPECT_EQ(b, "12345");
        str c = a.substr(5);
        EXPECT_EQ(c, "67890");
        EXPECT_EQ(a, b + c);
        str d = b.substr(3);
        EXPECT_EQ(d, "45");
        EXPECT_EQ(a.substr(4), a(4));
        EXPECT_EQ(a.substr(1, 2), a(1, 2));
        EXPECT_EQ(d(2), "");
        EXPECT_EQ(a(7), a(7, 10));
        EXPECT_EQ(a(1, 7)(1, 2), "3");
        EXPECT_EQ(a(2)(1, 5), "4567");
        str e = a + b + c + d;
        EXPECT_EQ(e(4, 16), "567890123456");

        str a2 = "1234567890";
        a2 = a2.substr(1, 7).substr(2, 5);
        EXPECT_CSTRING_EQ(a2.c_str(), "456");
    }

    TEST(not_equals)
    {
        str a = "1";
        str b = "@";
        EXPECT_NE(a, b);
        str c;
        EXPECT_NE(a, c);
        str q = "1";
        q *= 2;
        EXPECT_NE(a, q);
        c = "0";
        c *= 31;
        EXPECT_NE("a" + c, c + "a");
        c += '0';
        EXPECT_NE("a" + c + "b", "b" + c + "a");
    }

    TEST(invert)
    {
        str a = "12323410";
        str b = a.substr(2).substr(1, 5);
        EXPECT_EQ(b, "2341");
        b = b.invert();
        EXPECT_EQ(b, "1432");
        EXPECT_EQ(a.invert(), "01432321");
    }

    TEST(write_access)
    {
        str a = "12345";
        a[2] = '0';
        EXPECT_EQ(a, "12045");
        str b = a;
        a[4] = '0';
        EXPECT_EQ(a, "12040");
        EXPECT_EQ(b, "12045");
        str c = a(2, 3);
        EXPECT_EQ(c, "0");
        c[0] = '5';
        EXPECT_EQ(c, "5");
        EXPECT_EQ(a, "12040");
    }

    TEST(write_aceess_through)
    {
        str a = str("1") * 10000;
        str p = a;
        for(int i = 0; i < 9992; i++)
            p = p(1);
        a = "some other string";
        p[0] = '2';
        EXPECT_EQ(p, "21111111");
    }

    TEST(write_speed)
    {
        str a = "0123456789";
        for(unsigned long i = 0;i < 10000000;i++)
            a[randomUL() % 10] = '0' + (randomUC() % 10);
        EXPECT_EQ(a.length(), 10);
        for(unsigned i = 0;i < a.length();i++)
            EXPECT_IN_RANGE(a.at(i), '0', '9', "%c not in 0..9", a.at(i));
    }

    TEST(char_append)
    {
        str d = ">";
        for(int i = 0;i < 9;i++)
            d += '^';
        str h;
        h += '>';
        EXPECT_EQ(h, ">", "\"%s\" != \">\"", h.c_str());
        h += "^^^^^^^^^";
        EXPECT_EQ(d, h, "\"%s\" != \"%s\" (expected == \">^^^^^^^^^\" for both)",
            d.c_str(), h.c_str());
        str s = "str";
        str c;
        c += 'c';
        str u;
        unsigned long str_count = 0;
        for(unsigned i = 0;i < 100000;i++)
            if(randomUC() < 5)
            {
                u += s;
                str_count++;
            }
            else
                u += c;
        EXPECT_EQ(u.length(), str_count * 2 + 100000,
                  "Expected length of str: %lu, but length is %lu",
                  str_count * 2lu + 100000lu, u.length());
    }

    TEST(last_char_is_nul)
    {
        str e;
        EXPECT_EQ(e.at(0), '\0');
        for(int i = 0;i < 10;i++)
        {
            e += '1';
            EXPECT_EQ(e.at(e.length()), '\0');
        }
        str a = "1234567";
        EXPECT_EQ(a.at(a.length()), '\0');
        e += a;
        EXPECT_EQ(e.at(e.length()), '\0');
        for(int i = 0;i < 10;i++)
        {
            e += '1';
            EXPECT_EQ(e.at(e.length()), '\0');
        }
        a = e.substr(3, 7);
        EXPECT_EQ(a.at(a.length()), '\0');
        a = a.substr(3);
        EXPECT_EQ(a.at(a.length()), '\0');
        a = "";
        EXPECT_EQ(a.at(a.length()), '\0');
    }

    TEST(str_comparision)
    {
        str a, b;
        EXPECT_LE(a, b);
        EXPECT_GE(a, b);
        EXPECT_EQ(a, b);
        a = "123";
        EXPECT_LE(b, a);
        EXPECT_FALSE(a <= b);
        b = "12";
        EXPECT_LE(b, a);
        EXPECT_FALSE(a <= b);
        b += '5';
        EXPECT_GT(b, a);
        EXPECT_LE(a, b);
    }

    TEST(str_equals_speed)
    {
        str a = "1";
        str b = "1";
        for(unsigned i = 0;i < 22;i++)
        {
            EXPECT_EQ(a, b);
            a += a;
            b += b;
        }
    }

    TEST(str_comparision_speed, REPEAT(100))
    {
        str a = "1";
        str b = "1";
        a *= 100000;
        b *= 100000;
        EXPECT_EQ(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(b, a);
        EXPECT_FALSE(b < a);
        a = "0" + a;
        EXPECT_LT(a, b);
        EXPECT_NE(a, b);
        b = "0" + b + "1";
        EXPECT_LT(a, b);
        EXPECT_NE(a, b);
    }

    TEST(find_char)
    {
        str a = "aabaa.baaz..a..";
        for(unsigned i = 0;i < 100000;i++)
        {
            EXPECT_EQ(a.find_char('a'), 0);
            EXPECT_EQ(a.find_char('a', 1), 1);
            EXPECT_EQ(a.find_char('a', 2), 3);
            EXPECT_EQ(a.find_char('a', 5), 7);
            EXPECT_EQ(a.find_char('a', 13), str::not_found);
            EXPECT_EQ(a.find_char('a', 19), str::not_found);
            EXPECT_EQ(a.find_char('.', 14), 14);
            EXPECT_EQ(a.find_char('x'), str::not_found);
        }
    }

    TEST(rfind_char)
    {
        str a = "aabaa.baaz..a..";
        for(unsigned i = 0;i < 100000;i++)
        {
            EXPECT_EQ(a.rfind_char('.'), 14);
            EXPECT_EQ(a.rfind_char('.', 14), 13);
            EXPECT_EQ(a.rfind_char('.', 13), 11);
            EXPECT_EQ(a.rfind_char('.', 7), 5);
            EXPECT_EQ(a.rfind_char('.', 5), str::not_found);
            EXPECT_EQ(a.rfind_char('.', 0), str::not_found);
            EXPECT_EQ(a.rfind_char('a', 1), 0);
            EXPECT_EQ(a.rfind_char('a', 0), str::not_found);
            EXPECT_EQ(a.rfind_char('x'), str::not_found);
        }
    }

    TEST(find_all_char)
    {
        str a = "aabaa.baaz..a..";
        for(unsigned i = 0;i < 100000;i++)
        {
            for(char c : "abxz.")
            {
                auto v = a.find_all_char(c);
                for(unsigned long t = 0, p = 0;;t++)
                    if(a.at(t) == '\0')
                    {
                        EXPECT_EQ(v.size(), p, "Char: %c", c);
                        break;
                    }
                    elif(a.at(t) == c)
                    {
                        EXPECT_NE(v.size(), p, "Char: %c", c);
                        EXPECT_EQ(v[p++], t, "Char: %c", c);
                    }

            }
        }
    }

    TEST(count_char)
    {
        str a = "aabaa.baaz..a..";
        for(unsigned i = 0;i < 100000;i++)
        {
            for(char c : "abxz.")
            {
                unsigned long v = a.count_char(c);
                unsigned long ct = 0;
                for(unsigned long t = 0;t < a.length();t++)
                    if(a.at(t) == c)
                        ct++;
                EXPECT_EQ(v, ct);
            }
        }
    }
    
    TEST(contains_char)
    {
        str a = "aabaa.baaz..a..y";
        for(unsigned i = 0;i < 100000;i++)
        {
            EXPECT_TRUE(a.contains_char('a'));
            EXPECT_TRUE(a.contains_char('.'));
            EXPECT_TRUE(a.contains_char('y'));
            EXPECT_FALSE(a.contains_char('x'));
        }
    }

    TEST(find)
    {
        str a = "aaaaaaaaaaaabababaaaba.baab..baaaba";
        str b = ".";
        str c = "aba";
        str d = "aa";
        str e = a;
        str f = "aaab";
        for(unsigned i = 0;i < 100000;i++)
        {
            EXPECT_EQ(a.find(b), a.find_char('.'));
            EXPECT_EQ(a.find(c), a.find_char('b') - 1);
            EXPECT_EQ(a.find(d), 0);
            EXPECT_EQ(a.find(e), 0);
            EXPECT_EQ(a.find(f), a.find_char('b') - 3);

            EXPECT_EQ(a.find(b, 3), a.find_char('.', 3));
            EXPECT_EQ(a.find(c, 3), a.find_char('b', 3) - 1);
            EXPECT_EQ(a.find(d, 3), 3);
            EXPECT_EQ(a.find(e, 3), str::not_found);
            EXPECT_EQ(a.find(f, 3), a.find_char('b', 3) - 3);

            unsigned long p = a.find_char('.') + 1;
            EXPECT_EQ(a.find(b, p), a.find_char('.', p));
            EXPECT_EQ(a.find(c, p), p + 9);
            EXPECT_EQ(a.find(d, p), p + 1);
            EXPECT_EQ(a.find(e, p), str::not_found);
            EXPECT_EQ(a.find(f, p), p + 7);

            EXPECT_EQ(a.find(b, a.length()), str::not_found);
            if(random8() == 0)
                b[0] = b.at(0);
            if(random8() == 0)
                c[0] = c.at(0);
            if(random8() == 0)
                d[0] = d.at(0);
            if(random8() == 0)
                e[0] = e.at(0);
            if(random8() == 0)
                f[0] = f.at(0);
        }
    }

    TEST(rfind)
    {
        str a = "aaaaaaaaaaaabababaaaba.baab..baaaba";
        str b = ".";
        str c = "aba";
        str d = "aa";
        str e = a;
        str f = "aaab";
        for(unsigned i = 0;i < 100000;i++)
        {
            EXPECT_EQ(a.rfind(b), a.rfind_char('.'));
            EXPECT_EQ(a.rfind(c), a.rfind_char('b') - 1);
            EXPECT_EQ(a.rfind(d), a.rfind_char('b') - 2);
            EXPECT_EQ(a.rfind(e), 0);
            EXPECT_EQ(a.rfind(f), a.rfind_char('b') - 3);

            EXPECT_EQ(a.rfind(b, 3), a.rfind_char('.', 3));
            EXPECT_EQ(a.rfind(c, 3), str::not_found);
            EXPECT_EQ(a.rfind(d, 3), 1);
            EXPECT_EQ(a.rfind(e, 3), str::not_found);
            EXPECT_EQ(a.rfind(f, 3), str::not_found);

            unsigned long p = a.find_char('.') + 1;
            EXPECT_EQ(a.rfind(b, p), a.rfind_char('.', p));
            EXPECT_EQ(a.rfind(c, p), p - 4);
            EXPECT_EQ(a.rfind(d, p), p - 5);
            EXPECT_EQ(a.rfind(e, p), str::not_found);
            EXPECT_EQ(a.rfind(f, p), p - 6);

            EXPECT_EQ(a.rfind(b, a.length()), a.rfind_char('.'));
            if(random8() == 0)
                b[0] = b.at(0);
            if(random8() == 0)
                c[0] = c.at(0);
            if(random8() == 0)
                d[0] = d.at(0);
            if(random8() == 0)
                e[0] = e.at(0);
            if(random8() == 0)
                f[0] = f.at(0);
        }
    }

    TEST(find_all)
    {
        str a = "aaaaaaaaaaaabababaaaba.baab..baaaba";
        str b = ".";
        str c = "aba";
        str d = "aa";
        str e = a;
        str f = "aaab";
        for(unsigned i = 0;i < 100000;i++)
        {
            auto v = a.find_all(b);
            EXPECT_EQ_RA_VALS(v, v.size(), 22, 27, 28);
            v = a.find_all(c);
            EXPECT_EQ_RA_VALS(v, v.size(), 11, 15, 19, 32);
            v = a.find_all(d);
            EXPECT_EQ_RA_VALS(v, v.size(), 0, 2, 4, 6, 8, 10, 17, 24, 30);
            v = a.find_all(e);
            EXPECT_EQ_RA_VALS(v, v.size(), 0);
            v = a.find_all(f);
            EXPECT_EQ_RA_VALS(v, v.size(), 9, 17, 30);
            v = a.find_all(b + f);
            EXPECT_EQ_RA_VALS(v, v.size());

            if(random8() == 0)
                b[0] = b.at(0);
            if(random8() == 0)
                c[0] = c.at(0);
            if(random8() == 0)
                d[0] = d.at(0);
            if(random8() == 0)
                e[0] = e.at(0);
            if(random8() == 0)
                f[0] = f.at(0);
        }
    }

    TEST(find_all_intersect)
    {
        str a = "aaaaaaaaaaaabababaaaba.baab..baaaba";
        str b = ".";
        str c = "aba";
        str d = "aa";
        str e = a;
        str f = "aaab";
        for(unsigned i = 0;i < 100000;i++)
        {
            auto v = a.find_all_intersect(b);
            EXPECT_EQ_RA_VALS(v, v.size(), 22, 27, 28);
            v = a.find_all_intersect(c);
            EXPECT_EQ_RA_VALS(v, v.size(), 11, 13, 15, 19, 32);
            v = a.find_all_intersect(d);
            EXPECT_EQ_RA_VALS(v, v.size(), 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 17, 18, 24, 30, 31);
            v = a.find_all_intersect(e);
            EXPECT_EQ_RA_VALS(v, v.size(), 0);
            v = a.find_all_intersect(f);
            EXPECT_EQ_RA_VALS(v, v.size(), 9, 17, 30);
            v = a.find_all_intersect(b + f);
            EXPECT_EQ_RA_VALS(v, v.size());

            if(random8() == 0)
                b[0] = b.at(0);
            if(random8() == 0)
                c[0] = c.at(0);
            if(random8() == 0)
                d[0] = d.at(0);
            if(random8() == 0)
                e[0] = e.at(0);
            if(random8() == 0)
                f[0] = f.at(0);
        }
    }

    TEST(count)
    {
        str a = "aaaaaaaaaaaabababaaaba.baab..baaaba";
        str b = ".";
        str c = "aba";
        str d = "aa";
        str e = a;
        str f = "aaab";
        for(unsigned i = 0;i < 100000;i++)
        {
            EXPECT_EQ(a.count(b), 3);
            EXPECT_EQ(a.count(c), 4);
            EXPECT_EQ(a.count(d), 9);
            EXPECT_EQ(a.count(e), 1);
            EXPECT_EQ(a.count(f), 3);
            EXPECT_EQ(a.count(b + f), 0);

            if(random8() == 0)
                b[0] = b.at(0);
            if(random8() == 0)
                c[0] = c.at(0);
            if(random8() == 0)
                d[0] = d.at(0);
            if(random8() == 0)
                e[0] = e.at(0);
            if(random8() == 0)
                f[0] = f.at(0);
        }
    }

    TEST(count_intersect)
    {
        str a = "aaaaaaaaaaaabababaaaba.baab..baaaba";
        str b = ".";
        str c = "aba";
        str d = "aa";
        str e = a;
        str f = "aaab";
        for(unsigned i = 0;i < 100000;i++)
        {
            EXPECT_EQ(a.count_intersect(b), 3);
            EXPECT_EQ(a.count_intersect(c), 5);
            EXPECT_EQ(a.count_intersect(d), 16);
            EXPECT_EQ(a.count_intersect(e), 1);
            EXPECT_EQ(a.count_intersect(f), 3);
            EXPECT_EQ(a.count_intersect(b + f), 0);

            if(random8() == 0)
                b[0] = b.at(0);
            if(random8() == 0)
                c[0] = c.at(0);
            if(random8() == 0)
                d[0] = d.at(0);
            if(random8() == 0)
                e[0] = e.at(0);
            if(random8() == 0)
                f[0] = f.at(0);
        }
    }
    
    TEST(contains)
    {
        str a = "aaaaaaaaaaaabababaaaba.baab..baaaba";
        str b = ".";
        str c = "aba";
        str d = "aa";
        str e = a;
        str f = "aaab";
        for(unsigned i = 0;i < 100000;i++)
        {
            EXPECT_TRUE(a.contains(b));
            EXPECT_TRUE(a.contains(b * 2));
            EXPECT_FALSE(a.contains(b * 3));
            
            EXPECT_TRUE(a.contains(c));
            EXPECT_FALSE(a.contains(c * 2));
            
            EXPECT_TRUE(a.contains(d));
            EXPECT_TRUE(a.contains(d * 3));
            
            EXPECT_TRUE(a.contains(d * 6));
            EXPECT_FALSE(a.contains(d * 7));
            
            EXPECT_TRUE(a.contains(e));
            EXPECT_FALSE(a.contains(e + e));
            
            EXPECT_TRUE(a.contains(f));
            EXPECT_FALSE(a.contains(f + f));

            if(random8() == 0)
                b[0] = b.at(0);
            if(random8() == 0)
                c[0] = c.at(0);
            if(random8() == 0)
                d[0] = d.at(0);
            if(random8() == 0)
                e[0] = e.at(0);
            if(random8() == 0)
                f[0] = f.at(0);
        }
    }

    TEST(action_sequence)
    {
        for(unsigned i = 0;i < 100000;i++)
        {
            str a = "aax";
            str b = "bbx";
            str c = a + a + b;
            c += '0';
            c += '0';
            c += a;
            c += '0';
            c += b;
            c += '0';
            EXPECT_EQ(c.find(b), 6);
            EXPECT_EQ(c.rfind(b), 15);
            c += '0';
            EXPECT_EQ(c.rfind("00"), 18);
            c *= 2;
            c += c;
            c += '0';
            c += '0';
            EXPECT_EQ(c.rfind("00"), 80);
        }
    }

    TEST(split)
    {
        str s = "a\nb\nc\n";
        auto split_obj = s.split();
        auto it = split_obj.begin();
        EXPECT_EQ(*it, "a");
        EXPECT_EQ(*++it, "b");
        EXPECT_EQ(*++it, "c");
        ++it;
        EXPECT_NE(it, split_obj.end());
        EXPECT_EQ(*it, "");
        EXPECT_EQ(++it, split_obj.end());
    }
}
