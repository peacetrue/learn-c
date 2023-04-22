#include "gtest/gtest.h"
#include <person.h>

using namespace learn;

// 测试构造函数
TEST(Person, constructor) {
    Person person(0);
    EXPECT_EQ(0, person.get_sex());
    person.introduce();
    auto person_ref = new Person(1);
    EXPECT_EQ(1, person_ref->get_sex());
}

// 测试析构函数
TEST(Person, destructor) {
    Person person(0);
}

TEST(Person, introduce) {
    std::string str = "cpp";
    Person person(str, 1, 18);
    person.introduce();
}

TEST(Person, observe) {
    Person person(1);
    person.observe(person);
}
