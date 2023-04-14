#include "3-Syntax_Analysis.h"

void test(){
    ifstream fin("/Users/zhe/Documents/cppCode/json_parser/text3.txt");
    if (!fin.is_open()) {
        cout << "文件打开失败" << endl;
        return;
    }
    stringstream ss;
    ss << fin.rdbuf();
    if (!fin.good()) {
        cout << "读取文件内容时出错" << endl;
        return;
    }
    string s = ss.str();
    auto vec = std::move(tokenize::tokenize(s));
//    cout<< vec.size()<<endl;
//    for (auto& i : vec) {
//        cout << i;
//    }

    var x = parse_json::parse_json(vec);
    cout << x << endl;      //重载了'<<'，可以直接输出json格式
    cout << x[114] << endl;
    cout << x["hello wolrd"] << endl;
    cout << x["hello wolrd"][5] << endl;
    cout << x["hello wolrd"][5][114514] << endl;

    fin.close();
}
int main(){
    test();
    return 0;
}