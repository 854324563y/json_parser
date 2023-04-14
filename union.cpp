#include <iostream>
using namespace std;
union U1
{
    int x;
    float y;
    double z;
};
union U2
{
    int arr[10];
    double z;
};
union U3
{
    struct node
    {
        int x;
        long long y;
    }u;
    double z;
};

//查看内存分布
union U4
{
    unsigned char bits[4];
    int num;
};

//优化短字符串
struct short_str
{
    union   //匿名的union
    {
        unsigned long long hs;
        char s[8];
    };
};

//取别名
struct point{
    int x, y;
};
struct line{
    union {
        struct {
            point p1, p2;
        };
        int arr[4];
    };
};

//动态类型
struct var{
    union {
        int iv;
        double dv;
        char* sv;
    };
    var(const int& v) :iv(v) {};
    var(const double& v) :dv(v) {};
    //没有释放new的内存！！！！！！！！！！！！！！！！！！
    var(const char* s) {
        int len = strlen(s);
        sv = new char[len + 1];
        memcpy(sv, s, len + 1);
    }
    //加上析构函数，避免内存泄露！！！！！！！！！！！！！！
    // ~var() {
    //     if (sv != nullptr) {
    //         delete[] sv;
    //         sv = nullptr;
    //     }
    // }
};
int main()
{
    {
        U1 x;
        x.y=3.14;                   //
        cout<< x.x <<endl;          //1078523331 x和y公用内存
        cout << sizeof(U1) << endl; //8
    }

    cout << sizeof(U2) << endl; //40
    
    cout << sizeof(U3) << " " << sizeof(U3::node) << endl; //16 16
    
    {
        U4 x;
        x.num = 64;
        for (int i = 0; i < 4; i++) {
            cout << bitset<8>(x.bits[i]) << " ";
        }
        cout << endl;
        x.num = 123456;
        for (int i = 0; i < 4; i++) {
            cout << bitset<8>(x.bits[i]) << " ";
        }
        cout<< endl;
    }

    {
        short_str s;
        memcpy(s.s, "hello", 6);
        cout << s.s << endl;
        cout << s.hs << endl;   //478560413032 //快速的获得短字符串的哈希值(把这个ull看成哈希值)
    }

    {
        line L = {11,51,1919,810};
        cout << L.p1.x << " " << L.p1.y << endl;    //11 51
        for (int i = 0; i < 4; i++)L.arr[i] = i;
        cout << L.p1.x << " " << L.p1.y << endl;    //0 1
        cout << L.p2.x << " " << L.p2.y << endl;    //2 3
        cout << sizeof(line) << endl;               //16
    }

    {
        var x = 1415;           //调用构造函数
        cout << x.iv << endl;
        x = 3.14;               //调用构造函数
        cout << x.dv << endl;
        //x = "hello world";      //调用构造函数
        cout << x.sv << endl;
    }
    return 0;
}