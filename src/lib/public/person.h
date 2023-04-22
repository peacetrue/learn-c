#include <iostream>
#include <utility>

// C++ 面向对象之 Person。
/** 命名空间，分组隔离不同的类 */
namespace learn {

    /** 人 */
    class Person {

    private:
        //私有变量带后缀 _
        /** 姓名 */
        std::string name_;//:: 作用域解析运算符，命名空间和对象、对象和方法之间的分隔符
        /** 年龄 */
        int age_;
        /** 性别 */
        int sex_;
    protected:
        /** 构造函数。年龄默认设置为 0 */
        Person() : age_(0) {}
//        Person() {
//            this->age_ = 0; // -> 从指针对象中访问类成员的运算符
//        }
    public:
        /** 析构函数。对应构造函数，对象销毁时触发 */
        ~Person() { printf("Person destroy!"); }

        /**
         * 构造时指定性别。
         * @param sex 性别
         */
        explicit Person(int sex) : Person() {
            // Clang-Tidy: Single-argument constructors must be marked explicit to avoid unintentional implicit conversions
            // Person person = 1; // 隐式转换为 Person 对象，Person person(1) -> Person person = new Person(1)
            // explicit 防止隐式转换
            this->sex_ = sex;
        }

        /**
         * 构造时指定全部属性。
         * @param name 姓名
         * @param sex 性别
         * @param age 年龄
         */
        Person(const std::string &name, int sex, int age) : Person(sex) {
            this->name_ = name;
            this->age_ = age;
        }

        /** 自我介绍 */
        void introduce();

        /**
         * 观察对方。
         * @param person 对方
         */
        void observe(const Person &person) const;

        /**
         * 询问对方姓名。
         * @param person 对方
         */
        static std::string askName(const Person &person);

        /**
         * 获取性别。头文件中对象实现的函数都作为内联函数。
         * @return 性别
         */
        int get_sex() const {
            // const：
            // 在C++中，成员函数后面的 const 关键字表示该成员函数是一个只读函数，不会对对象进行任何修改，只是读取对象的状态并返回结果。
            // 使用 const 关键字还可以使编译器在编译时进行更多的优化，因为编译器可以确定这些函数不会修改对象，因此可以更安全地进行优化。
            return sex_;
        }
    };
}
