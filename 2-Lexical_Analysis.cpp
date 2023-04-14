#include "2-Lexical_Analysis.h"

void test(){
    //这里用绝对路径才成功，必然一直打不开
    ifstream infile("/Users/zhe/Documents/cppCode/json_parser/text2.txt");
    if (!infile.is_open()) {
        cout << "文件打开失败" << endl;
        return;
    }
    stringstream buffer;
    buffer << infile.rdbuf();
    if (!infile.good()) {
        cout << "读取文件内容时出错" << endl;
        return;
    }
    string str = buffer.str();
    //cout <<str;
    auto t = std::move(tokenize::tokenize(str));
    for (auto& i : t) {
        cout << i;
    }

    infile.close();
}
int main(){
    test();
    return 0;
}