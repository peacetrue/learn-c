#include "person.h"

using namespace learn;

void Person::introduce() {
    //<<：插入运算符，可以理解为调用一个方法，std::cout.print("")
    //std::endl：换行符
    std::cout
            << "My name is " << this->name_
            << ", My age " << this->age_
            << ", My gender as you see."
            << std::endl;
}

void Person::observe(const Person &person) const {
    //如果阅历不足，只能看出性别
    if (this->age_ < 100) {
        std::cout
                << "His name is unknown"
                << ", His age is also unknown"
                << ", His sex is " << person.get_sex() << "."
                << std::endl;
    } else {
        // 如果经验丰富，能看出真实信息
        std::cout
                << "His name is " << person.name_
                << ", His age is " << person.age_
                << ", His sex is " << person.get_sex()
                << "." << std::endl;
    }
}

std::string Person::askName(const Person &person) {
    return person.name_;
}
