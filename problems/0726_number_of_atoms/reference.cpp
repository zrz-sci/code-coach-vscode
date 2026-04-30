// 726. Number of Atoms
// 难度: Hard
// 标签: Stack, Hash Table, String, Sorting

// ===================== 解法一：栈 + HashMap =====================
// 时间 O(n + k log k)  空间 O(n)
// 思路：维护 stack<map<string,int>>，每遇到 '(' 压入新 map，
//       遇到 ')' 弹出并乘以后续数字后合并回栈顶。
class Solution {
public:
    string countOfAtoms(string formula) {
        int n = formula.size();
        int i = 0;

        // 辅助函数：从位置 i 开始解析一个原子名
        // 原子名 = 一个大写字母 + 0或多个小写字母
        auto parseName = [&]() -> string {
            string name;
            name += formula[i++]; // 大写字母
            while (i < n && islower(formula[i])) {
                name += formula[i++];
            }
            return name;
        };

        // 辅助函数：从位置 i 开始解析一个数字
        // 没有数字则返回 1
        auto parseNum = [&]() -> int {
            if (i >= n || !isdigit(formula[i])) return 1;
            int num = 0;
            while (i < n && isdigit(formula[i])) {
                num = num * 10 + (formula[i++] - '0');
            }
            return num;
        };

        // 栈：每一层是一个 map<原子名, 计数>
        stack<map<string, int>> stk;
        stk.push({});

        while (i < n) {
            if (formula[i] == '(') {
                // 新建一层
                i++;
                stk.push({});
            } else if (formula[i] == ')') {
                // 弹出当前层，乘以后续数字，合并到上一层
                i++;
                int multiplier = parseNum();
                map<string, int> top = stk.top();
                stk.pop();
                for (auto& [atom, cnt] : top) {
                    stk.top()[atom] += cnt * multiplier;
                }
            } else {
                // 解析原子名和数字
                string name = parseName();
                int cnt = parseNum();
                stk.top()[name] += cnt;
            }
        }

        // 栈底的 map 就是最终结果，按字典序输出
        string result;
        for (auto& [atom, cnt] : stk.top()) {
            result += atom;
            if (cnt > 1) result += to_string(cnt);
        }
        return result;
    }
};

// ===================== 解法二：递归下降解析 =====================
// 时间 O(n + k log k)  空间 O(n)
// 思路：利用递归函数 parse() 处理嵌套括号。
//       遇到 '(' 递归调用，遇到 ')' 或字符串结束返回当前 map。
class Solution2 {
public:
    string countOfAtoms(string formula) {
        int i = 0;
        map<string, int> counts = parse(formula, i);

        string result;
        for (auto& [atom, cnt] : counts) {
            result += atom;
            if (cnt > 1) result += to_string(cnt);
        }
        return result;
    }

private:
    // 从 formula[pos] 开始解析，返回原子计数 map
    map<string, int> parse(const string& formula, int& pos) {
        int n = formula.size();
        map<string, int> counts;

        while (pos < n && formula[pos] != ')') {
            if (formula[pos] == '(') {
                pos++; // 跳过 '('
                // 递归处理括号内的部分
                map<string, int> inner = parse(formula, pos);
                pos++; // 跳过 ')'
                // 解析括号后的乘数
                int multiplier = parseNum(formula, pos);
                // 将内层结果乘以 multiplier 合并到当前 map
                for (auto& [atom, cnt] : inner) {
                    counts[atom] += cnt * multiplier;
                }
            } else {
                // 解析原子名和计数
                string name = parseName(formula, pos);
                int cnt = parseNum(formula, pos);
                counts[name] += cnt;
            }
        }
        return counts;
    }

    string parseName(const string& formula, int& pos) {
        string name;
        name += formula[pos++]; // 大写字母
        while (pos < (int)formula.size() && islower(formula[pos])) {
            name += formula[pos++];
        }
        return name;
    }

    int parseNum(const string& formula, int& pos) {
        if (pos >= (int)formula.size() || !isdigit(formula[pos])) return 1;
        int num = 0;
        while (pos < (int)formula.size() && isdigit(formula[pos])) {
            num = num * 10 + (formula[pos++] - '0');
        }
        return num;
    }
};

// ===================== 解法三：栈 + 逆向遍历 =====================
// 时间 O(n + k log k)  空间 O(n)
// 思路：从右往左遍历化学式。维护一个乘数栈：
//       遇到 ')' 将当前乘数压栈，遇到 '(' 弹栈恢复乘数。
//       遇到原子时直接乘以当前累积乘数。
class Solution3 {
public:
    string countOfAtoms(string formula) {
        int n = formula.size();
        int i = n - 1;
        map<string, int> counts;
        stack<int> mulStack;
        mulStack.push(1); // 基础乘数为 1

        while (i >= 0) {
            if (formula[i] == ')') {
                // ')' 前面可能有数字，我们需要从右往左读
                // 先跳过 ')'
                i--;
            } else if (formula[i] == '(') {
                // 弹出一层乘数
                mulStack.pop();
                i--;
            } else if (isdigit(formula[i])) {
                // 逆向读取完整数字
                int end = i;
                while (i >= 0 && isdigit(formula[i])) i--;
                int num = stoi(formula.substr(i + 1, end - i));

                // 判断这个数字是跟在 ')' 后面（乘数）还是跟在原子后面
                if (i >= 0 && formula[i] == ')') {
                    // 乘数：压入栈
                    mulStack.push(mulStack.top() * num);
                    i--; // 跳过 ')'
                } else {
                    // 原子后面的数字：先逆向读原子名
                    int nameEnd = i;
                    while (i >= 0 && islower(formula[i])) i--;
                    // formula[i] 是大写字母
                    string name = formula.substr(i, nameEnd - i + 1);
                    counts[name] += num * mulStack.top();
                    i--; // 跳过大写字母
                }
            } else {
                // 字母：可能是单字符原子（大写）或多字符原子的一部分
                int nameEnd = i;
                while (i >= 0 && islower(formula[i])) i--;
                // formula[i] 是大写字母
                string name = formula.substr(i, nameEnd - i + 1);
                counts[name] += mulStack.top(); // 数字默认为 1
                i--;
            }
        }

        string result;
        for (auto& [atom, cnt] : counts) {
            result += atom;
            if (cnt > 1) result += to_string(cnt);
        }
        return result;
    }
};

/*
 * 总结：
 * - 解法一（栈 + HashMap）是最直观、最推荐的面试解法
 * - 解法二（递归下降）代码更优雅，适合熟悉编译原理的候选人
 * - 解法三（逆向遍历）展示了另一种思路但较复杂，不推荐面试使用
 *
 * 关键点：
 * 1. 字符串解析需要仔细处理大小写字母和数字的边界
 * 2. 栈用于处理括号嵌套，每层 map 记录当前作用域的原子计数
 * 3. 最终结果需要按 map 的 key 字典序排列（std::map 自带排序）
 */
