/*
 * 【题目本质】
 * 字符串解析 + 数学公式应用。解析 "a+bi" 格式的复数，
 * 用公式 (a+bi)(c+di) = (ac-bd) + (ad+bc)i 计算乘积。
 *
 * 【解法总览】
 * Solution1: 字符串手动解析 + 公式（推荐）
 * Solution2: sscanf 格式化解析 + 公式
 */

// 解法一：字符串手动解析（推荐）
// 思路：find('+') 定位分隔符，substr 提取实部虚部，套公式
// 时间 O(n)，空间 O(1)
class Solution1 {
public:
    string complexNumberMultiply(string num1, string num2) {
        auto parse = [](const string& s) -> pair<int, int> {
            int plus = s.find('+');
            int real = stoi(s.substr(0, plus));
            int imag = stoi(s.substr(plus + 1, s.size() - plus - 2)); // 去掉 'i'
            return {real, imag};
        };

        auto [a, b] = parse(num1);
        auto [c, d] = parse(num2);

        int realPart = a * c - b * d;
        int imagPart = a * d + b * c;

        return to_string(realPart) + "+" + to_string(imagPart) + "i";
    }
};

// 解法二：sscanf 格式化解析
// 思路：用 sscanf 直接按 "%d+%di" 格式提取四个整数
// 时间 O(n)，空间 O(1)
class Solution2 {
public:
    string complexNumberMultiply(string num1, string num2) {
        int a, b, c, d;
        sscanf(num1.c_str(), "%d+%di", &a, &b);
        sscanf(num2.c_str(), "%d+%di", &c, &d);

        int realPart = a * c - b * d;
        int imagPart = a * d + b * c;

        return to_string(realPart) + "+" + to_string(imagPart) + "i";
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 手动解析 | O(n) | O(1) | 灵活可控，面试推荐 |
 * | sscanf | O(n) | O(1) | 简洁但依赖C风格函数 |
 *
 * 【易错点】
 * 1. 虚部提取时必须去掉末尾的 'i' 字符
 * 2. 实部公式是 ac - bd（不是 ac + bd），因为 i^2 = -1
 * 3. "1+-1i" 是合法格式，虚部为负时 stoi 自动处理负号
 *
 * 【面试追问】
 * Q1: 复数除法？→ 分子分母同乘共轭 (ac+bd)/(c^2+d^2)
 * Q2: 设计完整复数类？→ 重载 +,-,*,/ 运算符
 * Q3: sscanf vs 手动解析？→ sscanf 简洁但不灵活
 */
