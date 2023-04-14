#include <iostream>
using namespace std;

struct A {
    int x, y, z;
    ~A() {
        cout << "~A()" << endl;
    }
};

template<typename T>
class RAII {
private:
    T* data;
public:
    RAII() : data(nullptr) {}
    explicit RAII(T* rhs) : data{ rhs } {}

    ~RAII() { 
        if(data)delete data;
    }
    T* operator ->()const {
        return data;
    }
    RAII(const RAII<T>& rhs) = delete;
    RAII(RAII<T>&& rhs) {
        data = rhs.data;
        rhs.data = nullptr;
    }
    RAII& operator = (const RAII& rhs) = delete;

    void operator = (RAII<T>&& rhs) {
        data = rhs.data;
        rhs.data = nullptr;
    }
};

void foo(RAII<A> p) { }
void foo2(const RAII<A>& p) {}
int main(){
    {
        RAII<A> p1(new A{ 114,514,1919810 });
        cout << "here" << endl;
    }

    cout<< "--------------------------" <<endl;

    RAII<A> p2(new A{ 114,514,1919810 });
    {

        RAII<A>p3 = move(p2);
    }
    cout << "here" << endl;

    cout<< "--------------------------" <<endl;

    RAII<A> p4 (new A{1,2,3});
    foo(move(p4));

    RAII<A> p5 (new A{1,2,3});
    foo2(p5);
}