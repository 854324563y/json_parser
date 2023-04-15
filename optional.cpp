#include <iostream>
#include <tuple>
#include <optional> //c++17
#include <complex>
#include <vector>

/**
 * optional<> 对象可先用 .has_value() 方法判断是否为空。否则直接 .value() 取值可能发生std::bad_optional_access异常；也可以直接'*'解引用。
 * nullopt 宏
 * make_optional<> 方法。参数直接初始化需要 in_place : op{in_place, args...}。
 * .value_or(defaultVal) 方法，如果optional<> 对象 op 没有值，则返回传入的 defaultVal 对象的值。
 */

using namespace std;
struct Out {
        string out1 { "" };
        string out2 { "" };
        ~Out(){ cout<<"~out"<<endl; }
    };
namespace test{
    //tuple<bool, string, string> func(const string& in) { }
    pair<bool, Out> func(const string& in) {    //return时会多拷贝一次out
        Out o;
        if (in.size() == 0)
            return { false, o };
        o.out1 = "hello";
        o.out2 = "world";
        return { true, o };
    }
}
namespace test_optional {
    optional<Out> func(const string& in) {
        if (in.size()==0) return nullopt;
        Out o ;
        o.out1 = "hello";
        o.out2 = "world";
        return o;
    }
}

int main() {
     if (auto [status, o] = test::func("hi"); status) {
         cout << o.out1 << endl;
         cout << o.out2 << endl;
     }

     // ret.has_value();但访问成员要用 -> ，ret->out1 ret->out2；或者 *ret.out1 *ret.out2
     if (auto ret = test_optional::func("hi") ; ret.has_value()) {
         cout << ret->out1 << endl;
         cout << ret->out2 << endl;
     }

     // 空 optiolal
     optional<int> oEmpty;
     optional<float> oFloat = nullopt;
     optional<int> oInt(10);
     optional oIntDeduced(10);  // type deduction
     // make_optional
     auto oDouble = std::make_optional(3.0);
     auto oComplex = make_optional<complex<double>>(3.0, 4.0);
     // in_place
     optional<complex<double>> o7{in_place, 3.0, 4.0};
     // initializer list
     optional<vector<int>> oVec(in_place, {1, 2, 3});    //size=3
     // 拷贝赋值
     auto oIntCopy = oInt;

     auto ret = test_optional::func("");
     cout << (*ret).out1 << endl;    //不会抛出异常
     cout << ret->out1 << endl;      //不会抛出异常

     auto ret2 = test_optional::func("");
     //抛出异常 libc++abi: terminating with uncaught exception of type std::bad_optional_access: bad_optional_access
     cout << ret2.value().out1 << endl;

    auto ret3 = test_optional::func("");
    // 当没有 value 调用该方法时将使用传入的默认值
    Out defaultVal;
    cout << ret3.value_or(defaultVal).out1 << endl;
    return 0;
}