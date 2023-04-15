# json_parser
As a case to learn c++17 standard. Parse and modify json object.

使用variant 管理json数据类型 ，string_view进行代理模式明确只读语义，optional处理非侵入式异常处理机制。
通过递归下降对json字符串进行parser，解析后支持动态修改，并重新输出json格式。
## 递归下降
递归下降（recursive descent）是一种语法分析技术，它是一种自顶向下的分析方法，用于从上到下逐步解析语法结构，直到识别出整个输入的语法结构。
递归下降的基本思想是将一个复杂的语法规则分解为一系列简单的子规则，并按照它们的顺序逐一进行解析。
递归下降语法分析器通常使用函数递归来实现对复杂语法规则的分解和解析。在递归下降中，每个非终结符对应一个函数，函数的执行过程就是对该非终结符的语法规则进行解析的过程。
- json::JsonParser in json_parser.cpp
- tokenize::parse() in 2-Lexical_Analysis.h
## std::variant
`std::variant` 是 C++17 标准库中提供的一种类型安全的联合（Union）类，它允许在一个变量中存储不同类型的值。
`std::variant` 支持在编译时确定变量的类型和运行时访问变量的值，提供了一种更安全和方便的多态实现方式。
与传统的联合（`Union`）不同，`std::variant` 中的成员具有类型信息，这使得在使用时更加安全，可以避免一些类型转换或类型不匹配的错误；`Union`所有类型共享同一块内存，有时使用时需`reinterpret_cast`类型转换。

使用 std::variant 时，需要提前定义所有可能的类型，std::variant 还提供了一些辅助类型
- `std::monostate` 表示无值状态、std::variant_size 表示变量中可存储的不同类型数目等。
- `std::holds_alternative` 判断std::variant中是否包含指定的类型。
- `std::visit` 对一个 variant 或 tuple 中的所有类型执行相同的操作。看json_parser.cpp中JsonGenerator::generate()

variant.cpp
## std::optional
optional<> 对象可先用.has_value()方法判断是否为空,否则直接.value()取值可能发生std::bad_optional_access异常；也可以直接`*`解引用。

nullopt 宏。

make_optional<> 方法。参数直接初始化需要 in_place : op{in_place, args...}。

.value_or(defaultVal) 方法，如果optional<> 对象 op 没有值，则返回传入的 defaultVal 对象的值。

optional.cpp

