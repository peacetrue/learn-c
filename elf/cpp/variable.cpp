/**
 * 变量案例，用于分析变量在汇编及 ELF 中的表示。
 * 变量命名规则：unin/init + static/const(修饰符) + long/Classes(类型) + value/ref/ptr(传递方式) + dyn(动态分配)。
 */
long unin_long;  //未初始化，编译器设置默认值为 0
//unin_long = 0; //编译报错：数据区内后续不能再赋值，A type specifier is required for all declarations
long init_long = 10;
//const long unin_const_long; // uninitialized ‘const unin_const_long’
const long init_const_long = 20; //数据区内所有变量具有常量效果，常量修饰符用于控制方法区内不能改变变量
static long unin_static_long;
static long init_static_long = 30;

class Classes { // 2 个 long 型变量共 16 个字节。为了区别指针，不使用 int 型，2 个 int 型 8 个字节与指针相同了
public:
    long unin_long;
    long init_long = 40;

    Classes() {}//默认构造函数

    Classes(long unin_long, long init_long) : unin_long(unin_long), init_long(init_long) {}

    static long unin_static_long;
//    static long init_static_long = 1; // Non-const static data member must be initialized out of line
    static const long unin_static_const_long;
    static const long init_static_const_long = 50;
};

long Classes::unin_static_long = 100;
//long Classes::unin_static_long = 0; // Redefinition of 'unin_static_long'
const long Classes::unin_static_const_long = 110;
// const long Classes::init_static_const_long = 0; // Static data member 'init_static_const_long' already has an initializer

Classes unin_classes;           //未初始化，调用默认构造函数
Classes init_classes(120, 130); //已初始化，调用指定构造函数
Classes init_classes_value = init_classes;  //值传递
Classes &init_classes_ref = init_classes;   //引用传递
Classes *init_classes_ptr = &init_classes;  //指针传递。指针和引用在汇编实现上相同；区别在语法上，指针可以为空，引用不能为空等
Classes *init_classes_ptr_dyn = new Classes(1, 2); //指针传递，动态分配内存

int main() {
    long unin_long; //未初始化，未设置初始值，值不确定
    const long init_long = 140;
    static long unin_static_long;
    static long init_static_long = 150;
    { static long init_static_long = 155; }//重复声明，汇编中如何避免重名

    Classes::unin_static_long = 160; //在方法中再次赋值
    //Classes::unin_static_const_long = 160; //Cannot assign to variable 'unin_static_const_long' with const-qualified type 'const long'
    Classes unin_classes;
    Classes init_classes(120, 130);
    Classes init_classes_value = init_classes;
    Classes &init_classes_ref = init_classes;
    Classes *init_classes_ptr = &init_classes;
    // delete init_classes_ptr; //运行时报错：pointer being freed was not allocated
    Classes *init_classes_ptr_dyn = new Classes(1, 2);
    delete init_classes_ptr_dyn;
}

