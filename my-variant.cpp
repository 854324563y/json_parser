#include <iostream>
#include <type_traits>
#include <vector>
using namespace std;

template<typename T, typename ...Ts>
struct Maxsize{     //注意这里是Maxsize
    constexpr static int size = sizeof(T) > Maxsize<Ts...>::size ? sizeof(T) : Maxsize<Ts...>::size; 
};
template<typename T>
struct Maxsize<T>{  //注意这里是Maxsize<T>
    constexpr static int size = sizeof(T);
};

template<int id, typename T, typename... Ts>
struct Type_element
{
    using Type = typename Type_element<id-1, Ts...>::Type;
};
template<typename T, typename... Ts>
struct Type_element<0, T, Ts...>
{
    using Type = T;
};

template<int id, typename U, typename T, typename... Ts>
struct Position {
    constexpr static int pos = is_same_v<U,T> ? id : Position<id+1,U,Ts...>::pos;   //is_same<U,T>::value也一样
};
template<int id, typename U, typename T>
struct Position<id,U,T> {
    constexpr static int pos = id;
};

template<typename T>
void destroy(char* data) {
    //reinterpret_cast<T*>(data) -> ~T();
    //如果类型 T 是一个 trivially_destructible 类型，则不需要调用析构函数，否则调用析构函数（如有需函数的类、有非trivially_destructible成员对象的类）。
    if constexpr (!std::is_trivially_destructible_v<T>)
        reinterpret_cast<T*>(data)->~T();
}

template<typename ... Ts>
struct Variant
{
    int type = -1;
    char data[Maxsize<Ts...>::size] = {0};

    using destroy_func_t = void (*) (char*);    //析构函数类型
    //constexpr 表示一个值可以在编译时确定，而 static 表示变量在整个程序运行过程中都存在，不需要每次使用时重新创建
    constexpr static destroy_func_t destroy_func[] = { destroy<Ts>... };    //析构函数的数组

    Variant() {};
    //同类型的拷贝构造函数
    Variant(const Variant<Ts...>& rhs) {
        type = rhs.type;
        memcpy(data, rhs.data, Maxsize<Ts...>::size);
    }
    //特定类型的拷贝构造函数
    template<typename T>
    Variant(T&& rhs) {
        type = Position<0,T,Ts...>::pos;
        *reinterpret_cast<T*>(data) = forward<T>(rhs);
    }
    //同类型的赋值构造。因为是构造函数，就不需要调用析构函数了
    void operator =(const Variant<Ts...>& rhs) {
        type = rhs.type;
        memcpy(data, rhs.data, Maxsize<Ts...>::size);
    }
    // 完美转发。调用析构函数
    template<typename T>
    void operator = (T&& rhs) {
        if (type != -1) {
            destroy_func[type](data);
        }
        type = Position<0, T, Ts...>::pos;
        // 去掉带引用的类型
        using rm_ref = typename remove_reference<T>::type;
        memset(data, 0, sizeof(rm_ref));
        //这里如果不使用forward<T>(rhs)，则无法保留T&&类型的右值引用特性。如果右值引用被转为左值，可能会影响代码的正确性和性能。因此，在需要保留右值引用特性时，通常需要使用forward函数。
        *reinterpret_cast<rm_ref*>(data) = forward<T>(rhs);
    }

    // template<typename T>                        //x2的测试
    // void operator = (const T& rhs) {
    //     *reinterpret_cast<T*>(data) = rhs;
    // }
    // template<typename T>                        //x4的测试
    // void operator = (T&& rhs) {     
    //     using rm_ref = typename remove_reference<T>::type;
    //     memset(data, 0 ,sizeof(rm_ref));
    //     *reinterpret_cast<rm_ref*>(data) = forward<T>(rhs);
    // }

    //根据类型返回
    template<typename T>
    T& get() {
        return *reinterpret_cast<T*>(data);
    }
    //根据下标返回
    template<int id>
    auto& get(){    //这里要c++14
        using T = typename Type_element<id, Ts...>::Type;
        return *reinterpret_cast<T*>(data);
    }

    //析构
    ~Variant() {
        if (type != -1) {
            destroy_func[type](data);
        }
    }
};

void foo(int & x) {
    cout << "int &" << endl;
}
void foo(int&& x) {
    cout << "int&&" << endl;
}
template<typename T>
// is_same_v 在<type_traits>中，且-std=c++17

// C++11 标准中规定，通常情况下右值引用形式的参数只能接收右值，不能接收左值。
// 但对于函数模板中使用右值引用语法定义的参数来说，它不再遵守这一规定，既可以接收右值，也可以接收左值（此时的右值引用又被称为“万能引用”）。

// check()T && x 和 foo(forward<T>(x))一起实现了完美转发，其中用到了c++11的引用折叠
void check(T && x) {
    if (is_same_v<T, int>)cout << "int  ->";
    if (is_same_v<T, int&>)cout << "int&  ->";
    if (is_same_v<T, int&&>)cout << "int&&  ->";
    foo(forward<T>(x));
}

struct line
{
    int x1, y1, x2, y2;
};

int main(){
    // cout<< Maxsize<char[7],int>::size << endl;

    // Type_element<2,int,double,float>::Type x1 = 3.13;
    // cout<< typeid(x1).name()<<" : "<<x1;  // f : 3.13

    // Variant<int, double>x2;
    // x2 = 114;
    // cout << x2.get<0>() << endl;
    // x2 = 3.14;
    // x2.get<1>() *= 1.11;
    // cout << x2.get<1>() << endl;

    // int x3;
    // //int&&是右值引用，int&是左值引用
    // check(x3);          //int&  ->int &     //x3是左值
    // check(move(x3));    //int  ->int&&      //move(x3)是右值

    // //更改 operator= 之后
    // Variant<int, string, vector<double>>x4;
    // x4 = 114514;
    // string s = "hello wrold";
    // x4 = move(s);
    // cout << x4.get<1>() << endl;
    // vector<double> v = { 1.9,1.9,8.10 };
    // x4 = move(v);
    // for (auto e : x4.get<2>())cout << e << " ";

    //cout<<Position<0,int, string,char,int>::pos;

    Variant<line, int[4]>arr;
    for (int i = 0; i < 4; i++)arr.get<1>()[i] = ( i + 1)  * 100;
    cout << arr.get<0>().x1 << " " << arr.get<0>().y1 << " " 
        << arr.get<0>().x2 << " " << arr.get<0>().y2 <<endl;
    Variant<int, vector<string>>x;
    x = 14 + arr.get<0>().x1;
    cout << x.get<int>() << endl;
    x = move(vector<string>{"hello", "world"});
    string str = "ygg";
    x.get<1>().push_back(move(str));
    for (auto s : x.get<1>()) {
        cout << s << endl;
    }
    return 0;
}