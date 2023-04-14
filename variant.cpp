#include <variant>
#include <string>
#include <iostream>

/*
std::variant 是 C++17 标准库中提供的一种类型安全的联合（Union）类，它允许在一个变量中存储不同类型的值。
std::variant 支持在编译时确定变量的类型和运行时访问变量的值，提供了一种更安全和方便的多态实现方式。

与传统的联合（Union）不同，std::variant 中的成员具有类型信息，这使得在使用时更加安全，可以避免一些类型转换或类型不匹配的错误。

使用 std::variant 时，需要提前定义所有可能的类型，然后在创建 std::variant 对象时指定其类型。
std::variant 提供了一系列操作符和方法来访问和修改变量中的值，包括 std::get、std::visit、operator= 等等。
此外，std::variant 还提供了一些辅助类型，如 std::monostate 表示无值状态、std::variant_size 表示变量中可存储的不同类型数目等。

*/

int main()
{
    std::variant<int, double, std::string> var;

    var = 42;
    std::cout << std::get<int>(var) << std::endl;

    var = 3.14;
    std::cout << std::get<double>(var) << std::endl;

    var = "Hello, world!";
    std::cout << std::get<std::string>(var) << std::endl;

    std::variant<int, double, std::string> v("hello");
    std::cout<< std::get<std::string>(v)<<std::endl;
    
    
    return 0;
}
