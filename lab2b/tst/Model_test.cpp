#include "gtest/gtest.h"
#include "Model.h"

class GameTest : public testing::Test {
protected:
    GameTest() :
            in("for_test_s.lif"),
            out1("for_test_f1.lif"),
            out2("for_test_f2.lif"),
            rule1(emptyStr),
            field1(emptyStr),
            rules_s(in),
            field_s(in),
            rules_f1(out1),
            field_f1(out1),
            rules_f2(out2),
            field_f2(out2),
            size_s(0, 0),
            size_f(0, 0)
    {
        size_f = field_f1.GetSize(size_f);
    }

    std::string in;
    std::string out1;
    std::string out2;
    std::string emptyStr;
    Rule rule;
    Field field;
    Rule rule1;
    Field field1;
    Rule rules_s;
    Field field_s;
    Rule rules_f1;
    Field field_f1;
    Rule rules_f2;
    Field field_f2;
    Coords size_s;
    Coords size_f;
};

TEST_F(GameTest, Step) {
    rules_s.Step(field_s);
    ASSERT_EQ(rules_s.forBirth, rules_f1.forBirth);
    ASSERT_EQ(rules_s.forLive, rules_f1.forLive);
    ASSERT_EQ(rules_s.Name, rules_f1.Name);
    ASSERT_EQ(rules_s.size.x, rules_f1.size.x);
    ASSERT_EQ(rules_s.size.y, rules_f1.size.y);
    size_s = field_s.GetSize(size_s);
    ASSERT_EQ(size_s.x, size_f.x);
    ASSERT_EQ(size_s.y, size_f.y);
    for(unsigned y = 0; y < size_s.y; y++){
        for(unsigned x = 0; x < size_s.x; x++){
            Coords point(x, y);
            ASSERT_EQ(field_s.Get(point), field_f1.Get(point));
        }
    }
}

TEST_F(GameTest, StepN) {
    rules_s.Step(field_s, 5);
    ASSERT_EQ(rules_s.forBirth, rules_f2.forBirth);
    ASSERT_EQ(rules_s.forLive, rules_f2.forLive);
    ASSERT_EQ(rules_s.Name, rules_f2.Name);
    ASSERT_EQ(rules_s.size.x, rules_f2.size.x);
    ASSERT_EQ(rules_s.size.y, rules_f2.size.y);
    size_s = field_s.GetSize(size_s);
    ASSERT_EQ(size_s.x, size_f.x);
    ASSERT_EQ(size_s.y, size_f.y);
    for(unsigned y = 0; y < size_s.y; y++){
        for(unsigned x = 0; x < size_s.x; x++){
            Coords point(x, y);
            ASSERT_EQ(field_s.Get(point), field_f2.Get(point));
        }
    }
}