/*
 * 【题目本质】
 * 在数字串的间隔中插入 +、-、* 运算符，使表达式结果等于 target。
 * 核心难点：乘法优先级处理——需记录上一个操作数以便"撤销重算"。
 *
 * 【解法总览】
 * Solution1: 回溯 + prevOp 追踪 — 标准解法，用 prevOp 解决乘法优先级 ⭐推荐
 * Solution2: 回溯 + string 优化 — 避免 substr 的优化版本
 */

// ===================== Solution1: 回溯 + prevOp 追踪 =====================
// 思路：回溯遍历所有分割+运算符组合
//       curVal: 当前表达式的累积值
//       prevOp: 上一个操作数（含符号），用于处理乘法优先级
//       乘法处理: curVal = curVal - prevOp + prevOp * curNum
// 时间：O(4^n * n)，n 为字符串长度
// 空间：O(n) 递归栈深度
class Solution1 {
public:
    vector<string> addOperators(string num, int target) {
        vector<string> res;
        string path;
        dfs(num, target, 0, 0, 0, path, res);
        return res;
    }

private:
    void dfs(const string& num, int target, int idx,
             long long curVal, long long prevOp,
             string& path, vector<string>& res) {
        if (idx == (int)num.size()) {
            if (curVal == target) {
                res.push_back(path);
            }
            return;
        }

        for (int i = idx; i < (int)num.size(); i++) {
            // 前导零剪枝：多位数不允许以 0 开头
            if (i > idx && num[idx] == '0') break;

            string curStr = num.substr(idx, i - idx + 1);
            long long curNum = stoll(curStr);
            int len = path.size();

            if (idx == 0) {
                // 第一个数字无需运算符
                path += curStr;
                dfs(num, target, i + 1, curNum, curNum, path, res);
                path.resize(len);
            } else {
                // +
                path += "+" + curStr;
                dfs(num, target, i + 1,
                    curVal + curNum, curNum, path, res);
                path.resize(len);

                // -
                path += "-" + curStr;
                dfs(num, target, i + 1,
                    curVal - curNum, -curNum, path, res);
                path.resize(len);

                // * (关键：撤销上一步的 prevOp，改用 prevOp*curNum)
                path += "*" + curStr;
                dfs(num, target, i + 1,
                    curVal - prevOp + prevOp * curNum,
                    prevOp * curNum, path, res);
                path.resize(len);
            }
        }
    }
};

// ===================== Solution2: 回溯 + 手动数字解析 =====================
// 思路：避免 substr + stoll，手动累加数字值
//       性能略优，适合面试中追问优化时展示
// 时间：O(4^n * n)
// 空间：O(n)
class Solution2 {
public:
    vector<string> addOperators(string num, int target) {
        vector<string> res;
        string path;
        dfs(num, target, 0, 0, 0, path, res);
        return res;
    }

private:
    void dfs(const string& num, long long target, int idx,
             long long curVal, long long prevOp,
             string& path, vector<string>& res) {
        if (idx == (int)num.size()) {
            if (curVal == target) res.push_back(path);
            return;
        }

        long long curNum = 0;
        int len = path.size();

        for (int i = idx; i < (int)num.size(); i++) {
            // 前导零剪枝
            if (i > idx && num[idx] == '0') break;

            curNum = curNum * 10 + (num[i] - '0');

            string curStr = num.substr(idx, i - idx + 1);

            if (idx == 0) {
                path += curStr;
                dfs(num, target, i + 1, curNum, curNum, path, res);
                path.resize(len);
            } else {
                // + 运算
                path += '+'; path += curStr;
                dfs(num, target, i + 1, curVal + curNum, curNum, path, res);
                path.resize(len);

                // - 运算
                path += '-'; path += curStr;
                dfs(num, target, i + 1, curVal - curNum, -curNum, path, res);
                path.resize(len);

                // * 运算
                path += '*'; path += curStr;
                dfs(num, target, i + 1,
                    curVal - prevOp + prevOp * curNum,
                    prevOp * curNum, path, res);
                path.resize(len);
            }
        }
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | prevOp追踪 | O(4^n*n) | O(n) | 标准解法，逻辑清晰 |
 * | 手动数字解析 | O(4^n*n) | O(n) | 避免substr，略快 |
 *
 * 【易错点】
 * 1. 前导零: "105" 中 "05" 不合法 -> i>idx && num[idx]=='0' 时 break
 * 2. 溢出: 中间值可能超过 int 范围 -> 必须用 long long
 * 3. 乘法 prevOp: 减法时 prevOp = -curNum (带负号)
 *    -> 连续乘法 "2-3*4" 中 prevOp=-3, 再 *4 得 prevOp=-12
 * 4. 第一个数字: 不加运算符，直接作为起始值
 * 5. path回溯: resize 而非 pop_back，因为追加了多字符
 *
 * 【面试追问】
 * Q1: 时间复杂度? -> O(4^n * n)，n-1个间隔各4种选择，substr O(n)
 * Q2: 加入除法? -> 类似乘法处理，注意除数为0和整除问题
 * Q3: 加入括号? -> 需要区间DP，搜索空间急剧增大
 */
