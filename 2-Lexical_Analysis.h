#include <string>
#include <vector>
#include <set>

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

enum token_type {
    KEY,
    VAR,
    FUNC,
    CON,
    OP,
};
struct token{
    token_type type;
    string value;
    int line;
    //friend ostream& operator << (ostream& os, token& t)
};
ostream& operator << (ostream& os, token& t) {
    os << "token_type : "<< t.type << ", value : " << t.value << ", line : "<<t.line <<endl;
    return os;
}

//用于输出
string token_print_map[30] = { "key","var","func","con","op" };

namespace  tokenize {

    string text;
    int pc;
    int cur_line;
    vector<token>tokens;
    bool find(string res) {
        for (int i = 0; i < res.length(); i++) {
            if (res[i] != text[pc + i])return false;
        }
        return true;
    }
    vector<string>op_words = {
            "&&","||",
            "++","--",
            "<=",">=",
            "==","!=",
            "+", "-","*","/",
            ">","<",
            "=",
            "%","!",
            "(",")",
            "[","]"
    };
    vector<string> key_words = {
            "def",
            "if",
            "else",
            "while",
            "for",
            "in",
            "println",
            "print",
            "scanf",
            "global",
            "{","}",
            ";", ":",
            ",",
            "return"
    };
    string get_var() {          //变量，考虑'_'、字符、数字
        string res;
        while (text[pc] == '_' || isalpha(text[pc]) || (text[pc] >= '0' && text[pc] <= '9')) {
            res.push_back(text[pc]);
            pc++;
        }
        return res;
    }
    string get_con() {
        string res;
        if (text[pc] == '"') {  //字符串常量
            res.push_back(text[pc++]);
            while (text[pc] != '"')res.push_back(text[pc++]);
            res.push_back(text[pc++]);
        }
        else {                  //数字常量（要考虑浮点）
            set<char> float_need = { 'e','f','.' };//浮点数需要的字符
            while ((text[pc] >= '0' && text[pc] <= '9') || float_need.count(text[pc]))res.push_back(text[pc++]);
        }
        return res;
    }
    set<string>func_set;
    void parse() {
        for (auto key_word : key_words) {
            if (find(key_word)) {
                tokens.push_back({ KEY,key_word ,cur_line});
                pc += key_word.length();
                return;
            }
        }
        for (auto op_word : op_words) {
            if (find(op_word)) {
                tokens.push_back({ OP,op_word,cur_line });
                pc += op_word.length();
                return;
            }
        }
        if ((text[pc] >= '0' && text[pc] <= '9') || text[pc] == '"') {  //数字常量 或 字符常量
            tokens.push_back({ CON,get_con(),cur_line });
            return;
        }
        if (tokens.back().value == "def") {                             //新函数
            string v = get_var();
            func_set.insert(v);
            tokens.push_back({ FUNC,v,cur_line });
            return;
        }
        string v = get_var();                                           //已有函数
        if (func_set.count(v)) {
            tokens.push_back({ FUNC,v,cur_line });
            return;
        }
        tokens.push_back({ VAR,v,cur_line });         //变量
    }

    vector<token>  tokenize(string s) {
        tokens.clear();
        pc = 0;
        text = s;
        cur_line = 1;
        while (pc < text.length()) {
            while (text[pc] == ' ' || text[pc] == '\n' || text[pc] == '\t') {
                if (text[pc] == '\n') cur_line++;
                pc++;
            }
            parse();
            while (text[pc] == ' ' || text[pc] == '\n' || text[pc] == '\t') {
                if (text[pc] == '\n') cur_line++;
                pc++;
            }
        }
        return tokens;
    }
}