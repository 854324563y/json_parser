#include <iostream>
#include <string>
#include <string_view>
#include<chrono>
void test_str(){
    //C风格字符串总是以null结尾
    //char cstr1[] = { 'y', 'a', 'n', 'g', NULL};
    char cstr12[] = {'y','a','n','g','\0'};
    char cstr2[5];
    //strcpy(cstr1, cstr2);
    //std::cout << cstr2 << std::endl;
    strcpy(cstr2,cstr12);
    std::cout << cstr2 << std::endl;

    //C++风格的字符串操作更方便，但是性能不如C风格字符串
    std::string str = "yang";
    std::string str2 = str;
}

void test_strview(){
    const char* str1 = "yangzhe";
    std::string_view strview1(str1);
    std::string_view strview2(str1,4);
    std::cout << "stringView1: " << strview1 << ", stringView2: " << strview2 << std::endl;
 
    std::string str = "yangzhe";
    std::string_view stringView3(str.c_str());
    std::string_view stringView4(str.c_str(), 4);
    std::cout << "stringView3: " << stringView3 << ", stringView4: " << stringView4 << std::endl;
}
void FunctionWithString(const std::string& str){ }
void FunctionWithString(const std::string_view& str){ }
void test_time(){
    class Timer
    {
    private:
        /* data */
        std::string title;
        std::chrono::high_resolution_clock::time_point m_start,m_stop;
    public:
        Timer(const std::string& title_):title(title_){
            m_start = std::chrono::high_resolution_clock::now();
        };
        ~Timer(){
            m_stop = std::chrono::high_resolution_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::microseconds>(m_stop - m_start);
            std::cout << title << " "<< ms.count()*0.000001 << " s." << std::endl;
        };
    };
    {
        Timer timer1("std::string");
        for(int i ; i<10000; ++i){
            std::string name = "yangzhe";
            std::string firstname = name.substr(0,4);
            std::string lastname = name.substr(4,6);
            FunctionWithString(firstname);
            FunctionWithString(lastname);
        }
    }
    {
        Timer timer2("std::string_view");
        for(int i ; i<10000; ++i){
            const char* name = "yangzhe";
            std::string_view firstname = std::string_view(name,4);
            std::string_view lastname = std::string_view(name+4,6);
            FunctionWithString(firstname);
            FunctionWithString(lastname);
        }
    }
}

int main()
{
   //test_str();
   //test_strview();
   
    test_time();
    //std::string 22.8305 s.
    //std::string_view 4.8e-05 s.
}