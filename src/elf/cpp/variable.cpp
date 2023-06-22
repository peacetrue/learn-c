/** 独立案例，用于分析变量在汇编及 ELF 中的表示 */


int unin_int;
int init_int = 30;
//const int unin_const_int; // uninitialized ‘const unin_const_int’
const int init_const_int = 40; // declaration specifier
static int unin_static_int;
static int init_static_int = 50;
//static const int unin_static_const_int; // uninitialized ‘const unin_const_int’
static const int init_static_const_int = 60;

class Classes {
public:
    int unin_int;
    int init_int = 10;

    Classes() {}

    Classes(int uninInt, int initInt) : unin_int(uninInt), init_int(initInt) {}

    static int unin_static_int;
//    static int init_static_int = 1; // Non-const static data member must be initialized out of line
    static const int unin_static_const_int;
    static const int init_static_const_int = 20;
};

int Classes::unin_static_int = 100;
//int Classes::unin_static_int = 2; // Redefinition of 'unin_static_int'
const int Classes::unin_static_const_int = 110;
// const int Classes::init_static_const_int = 1; // Static data member 'init_static_const_int' already has an initializer

Classes unin_classes;
Classes init_classes(120, 130);
Classes *unin_classes_ptr;
Classes *init_classes_ptr = &init_classes;

int main() {
    int unin_int_m;
    const int init_int_m = 140;
    static int unin_static_int_m;
    static int init_static_int_m = 150;

    Classes::unin_static_int = 160;
//    Classes::unin_static_const_int = 1; //Cannot assign to variable 'unin_static_const_int' with const-qualified type 'const int'

    Classes unin_classes_m;
    Classes init_classes_m(1, 2);
    Classes *unin_classes_ptr_m;

}

