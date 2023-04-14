#include "2-Lexical_Analysis.h"
#include <map>
#define INT 0
#define DOU 1
#define STR 2
#define MAP 3
#define ARR 4

class var
{
public:
    int INT_VAL = 0;
    double DOU_VAL;
    char CHA_VAL;
    string STR_VAL;
    vector<var> ARR_VAL;
    struct Cmp
    {
        bool operator () (var const& a, var const& b)const  //从小到大排序
        {
            if (a.type != b.type)return a.type < b.type;
            else {
                if (a.type == INT)return a.INT_VAL < b.INT_VAL;
                if (a.type == DOU)return a.DOU_VAL < b.DOU_VAL;
                if (a.type == STR)return a.STR_VAL < b.STR_VAL;
//                if (a.type == ARR)return a.ARR_VAL < b.ARR_VAL;
//                else return a.MAP_VAL < b.MAP_VAL;
                if (a.type == ARR)return a.ARR_VAL.size() < b.ARR_VAL.size();
                else return a.MAP_VAL.size() < b.MAP_VAL.size();
            }
        }
    };

    map<var, var, Cmp> MAP_VAL;
    int type = MAP;
    void del() { INT_VAL = DOU_VAL = 0; STR_VAL.clear(), MAP_VAL.clear(); }
    var() {}
    var(int x) { INT_VAL = x; type = INT; }
    var(double x) { DOU_VAL = x; type = DOU; }
    var(string x) { STR_VAL = x; type = STR; }
    var(const char* ch) {
        int n = strlen(ch);
        string s;
        for (int i = 0; i < n; i++) s.push_back(ch[i]);
        STR_VAL = s; type = STR;
    }
    var& operator [] (var id);
    void add(var x) {
        type = ARR;
        ARR_VAL.push_back(x);
    }
    void put(var key, var val) {
        type = MAP;
        MAP_VAL[key] = val;
    }
    int size() {
        if (type == STR)return STR_VAL.length();
        return MAP_VAL.size();
    }
};

var tmp;
var& var::operator[](var id) {
    if (type == STR) {
        string res; res.push_back(STR_VAL[id.INT_VAL]);
        tmp = res;
        return tmp;
    }
    else if (type == ARR) {
        return ARR_VAL[id.INT_VAL];
    }
    type = MAP;
    return MAP_VAL[id];
}
ostream& operator << (ostream& out, const var& v) {
    if (v.type == INT)out << v.INT_VAL;
    if (v.type == DOU)out << v.DOU_VAL;
    if (v.type == STR)out << "\"" << v.STR_VAL<<"\"";
    if (v.type == MAP) {
        out << "{";
        for (auto it = v.MAP_VAL.begin(); it != v.MAP_VAL.end(); it++) {
            out << (it->first) << ":" << (it->second);
            if (it != prev(v.MAP_VAL.end()))out << ",";
        }
        out << "}";
    }
    if (v.type == ARR) {
        out << "[";
        for (auto it = v.ARR_VAL.begin(); it != v.ARR_VAL.end(); it++) {
            out << *it;
            if (it != prev(v.ARR_VAL.end()))out << ",";
        }
        out << "]";
    }
    return out;
}

namespace parse_json {
    vector<token>tokens;
    int id = 0;
    token get() {
        return tokens[id];
    }
    void next() {
        id++;
    }
    void eat(string s) {
        if (get().value != s) {
            cout << "error : cant find " << s << " in line:" << get().line << endl;
            exit(0);
        }
        next();
    }
    var parse_obj();
    var parse_arr();
    var parse_con();
    var parse_map();
    var parse_obj() {       //递归的过程
        var obj;
        if (get().value == "[") {
            eat("[");
            obj = parse_arr();
            eat("]");
            return obj;
        }
        if (get().value == "{") {
            eat("{");
            obj = parse_map();  //两边已有eat()
            eat("}");
            return obj;
        }
        obj = parse_con(); next();
        return obj;
    }

    var parse_con() {
        string s = get().value;
        if (count(s.begin(), s.end(), '"')) {
            return { s.substr(1, s.length() - 2) };
        }
        if (count(s.begin(), s.end(), '.') || count(s.begin(), s.end(), 'f') || count(s.begin(), s.end(), 'e')) {
            stringstream ss; ss << s;
            double x; ss >> x;
            return { x };
        }
        stringstream ss; ss << s;
        int x;
        ss >> x;
        return { x };   //将x类型转换为var
    }
    var parse_arr() {
        var arr; arr.type = ARR;
        while (get().value != "]") {
            arr.add(parse_obj());
            if (get().value == "]")break;
            eat(",");
        }
        return arr;
    }
    var parse_map() {
        var map; map.type = MAP;
        while (get().value != "}") {
            var key = parse_obj();
            eat(":");
            var val = parse_obj();
            map.put(key, val);
            if (get().value == "}")break;
            eat(",");
        }
        return map;
    }
    var parse_json(vector<token>_tokens) {
        id = 0;
        tokens = _tokens;
        return parse_obj();
    }
}