// ============================================================
// 640. Solve the Equation
// ============================================================
// 解析一元一次方程字符串，返回 "x=#value", "No solution", 或 "Infinite solutions"
// 方程只包含 +, -, x 及其系数, 和整数常量
// 约束: 3 <= equation.length <= 1000, 系数/常数 in [0, 100]
// ============================================================


// ============================================================
// 解法一：统一解析（推荐）
// ============================================================
// 思路：
//   遍历整个等式，遇到 '=' 后将符号翻转（等价于移项）
//   最终得到 totalCoeff * x + totalConst = 0
//   即 x = -totalConst / totalCoeff
//
//   解析逻辑：逐字符扫描，维护：
//   - sign: 当前项的符号 (+1 或 -1)
//   - side: 当前在等号的哪一侧 (左=+1, 右=-1)
//   - num: 当前正在读取的数字
//   - hasNum: 是否已读到数字（区分 "x" 和 "0x"）
//
// 时间: O(n)  空间: O(1)
class Solution {
public:
    string solveEquation(string equation) {
        int coeff = 0;   // x 的总系数（左侧为正，右侧为负）
        int constant = 0; // 常数项总和（左侧为正，右侧为负）

        int sign = 1;     // 当前项的符号
        int side = 1;     // 左侧 = 1, 右侧 = -1 (过等号后翻转)
        int num = 0;
        bool hasNum = false;
        int n = equation.size();

        for (int i = 0; i <= n; ++i) {
            char c = (i < n) ? equation[i] : '+'; // 末尾加虚拟 '+' 触发最后一项处理

            if (c >= '0' && c <= '9') {
                num = num * 10 + (c - '0');
                hasNum = true;
            } else if (c == 'x') {
                // 遇到 x，num 是系数
                // 如果没有读到数字，系数为 1（如 "x" 或 "+x" 或 "-x"）
                int coeffVal = hasNum ? num : 1;
                coeff += sign * side * coeffVal;
                num = 0;
                hasNum = false;
            } else if (c == '+' || c == '-' || c == '=') {
                // 当前项结束，处理之前累积的数字（如果是常数项）
                if (hasNum) {
                    constant += sign * side * num;
                }
                num = 0;
                hasNum = false;

                // 更新符号和侧
                if (c == '=') {
                    side = -1;
                    sign = 1;
                } else {
                    sign = (c == '+') ? 1 : -1;
                }
            }
        }

        // coeff * x + constant = 0
        // x = -constant / coeff
        if (coeff == 0) {
            return (constant == 0) ? "Infinite solutions" : "No solution";
        }
        return "x=" + to_string(-constant / coeff);
    }
};


// ============================================================
// 解法二：左右分别解析
// ============================================================
// 思路：
//   1. 按 '=' 分割成左右两个表达式
//   2. 分别解析每个表达式，返回 (coeff, constant) 对
//   3. 合并: leftCoeff - rightCoeff = rightConst - leftConst
//
// 解析函数 parse(expr) 的实现：
//   逐字符扫描，遇到 +/- 或末尾时处理一个 token
//
// 时间: O(n)  空间: O(1)
class Solution2 {
public:
    string solveEquation(string equation) {
        int eqPos = equation.find('=');
        string left = equation.substr(0, eqPos);
        string right = equation.substr(eqPos + 1);

        auto [lc, lk] = parse(left);   // lc: coeff, lk: constant
        auto [rc, rk] = parse(right);

        // lc * x + lk = rc * x + rk
        // (lc - rc) * x = rk - lk
        int coeffDiff = lc - rc;
        int constDiff = rk - lk;

        if (coeffDiff == 0) {
            return (constDiff == 0) ? "Infinite solutions" : "No solution";
        }
        return "x=" + to_string(constDiff / coeffDiff);
    }

private:
    // 返回 {x的系数之和, 常数项之和}
    pair<int, int> parse(const string& expr) {
        int coeff = 0, constant = 0;
        int sign = 1, num = 0;
        bool hasNum = false;
        int n = expr.size();

        for (int i = 0; i <= n; ++i) {
            char c = (i < n) ? expr[i] : '+';

            if (c >= '0' && c <= '9') {
                num = num * 10 + (c - '0');
                hasNum = true;
            } else if (c == 'x') {
                coeff += sign * (hasNum ? num : 1);
                num = 0;
                hasNum = false;
            } else { // '+', '-', or end
                if (hasNum) {
                    constant += sign * num;
                }
                num = 0;
                hasNum = false;
                sign = (c == '-') ? -1 : 1;
            }
        }

        return {coeff, constant};
    }
};


// ============================================================
// 解法三：正则 + token 解析（思路框架）
// ============================================================
// 思路：
//   使用 regex 将表达式分割成 token
//   模式: ([+-]?\d*x|[+-]?\d+)
//   匹配结果如 "+5", "-3", "x", "+x", "2x" 等
//
//   对每个 token：
//   - 如果以 'x' 结尾: 提取系数
//   - 否则: 作为常数
//
// 由于 C++ regex 性能较差，面试中不推荐
// 这里只给出逻辑框架，不实现完整代码


// ============================================================
// 测试用例验证
// ============================================================
//
// 用例 1: "x+5-3+x=6+x-2"
//   左: coeff=2, const=2
//   右: coeff=1, const=4
//   -> (2-1)x = 4-2 -> x=2
//
// 用例 2: "x=x"
//   左: coeff=1, const=0
//   右: coeff=1, const=0
//   -> 0x = 0 -> Infinite solutions
//
// 用例 3: "2x=x"
//   左: coeff=2, const=0
//   右: coeff=1, const=0
//   -> x = 0
//
// 边界用例: "0x=0"
//   左: coeff=0, const=0
//   右: coeff=0, const=0
//   -> 0x = 0 -> Infinite solutions
//
// 边界用例: "-x=-1"
//   左: coeff=-1, const=0
//   右: coeff=0, const=-1
//   -> -1x = -1 -> x = 1
//
// 边界用例: "x=x+2"
//   左: coeff=1, const=0
//   右: coeff=1, const=2
//   -> 0x = 2 -> No solution


// ============================================================
// 字符串解析关键边界案例
// ============================================================
//
// 1. "x" -> 系数 1, 不是 0
// 2. "-x" -> 系数 -1
// 3. "+x" -> 系数 1
// 4. "0x" -> 系数 0 (hasNum=true, num=0)
// 5. "100x" -> 系数 100
// 6. "x+x" -> 系数 1+1=2
// 7. "-x+x" -> 系数 -1+1=0
// 8. 等号后紧跟负号: "x=-x" -> left: c=1,k=0; right: c=-1,k=0 -> 2x=0 -> x=0
