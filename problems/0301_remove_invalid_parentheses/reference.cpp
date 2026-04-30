/*
 * LeetCode 301: Remove Invalid Parentheses (删除无效的括号)
 *
 * 【题目本质】
 * 给定含括号和字母的字符串，删除最少数量的无效括号使字符串有效。
 * 返回所有可能的有效结果（不重复）。
 *
 * 约束: s.length <= 25, 括号最多 20 个 → 指数级搜索可接受
 *
 * 【解法总览】
 * 解法1: BFS 逐层搜索          — O(n * 2^n) / O(2^n)  — 直觉好
 * 解法2: DFS 回溯(简洁版)      — O(2^n) / O(n)        — 面试推荐 ⭐
 * 解法3: DFS 回溯(open计数版)  — O(2^n) / O(n)        — 更精确剪枝
 */

// ============================================================
// 辅助函数: 判断括号串是否有效
// 从左到右扫描，维护 open 计数器:
//   '(' → open++
//   ')' → open--
//   过程中 open < 0 → 右括号多了，无效
//   最终 open != 0 → 左右不匹配，无效
// ============================================================
// (每个 Solution 类中内联定义，此处为说明)

// ============================================================
// 解法1: BFS — 逐层删除括号
// 时间: O(n * 2^n)  空间: O(2^n) (set 去重)
//
// 【思路】
// BFS 按"删除个数"逐层扩展:
//   第0层: 原串 s              (删0个)
//   第1层: 删1个括号后的所有串   (删1个)
//   第2层: 删2个括号后的所有串   (删2个)
//   ...
// 第一次在某层发现有效串 → 该层所有有效串就是最少删除的答案
// BFS 天然保证最少删除!
//
// 去重: 删不同位置可能产生相同串
//   例如 "((" 删 idx=0 或 idx=1 都得到 "("
//   用 unordered_set 记录已访问的串
//
// 示例: s = "()())()"
//   第0层: "()())()" → 无效
//   第1层: 删1个括号后的6种串 (去重后)
//     ")()()" ✗  "()()()" ✓  "()())" ✗  "()()()" 重复
//     "(())()" ✓  ...
//   找到有效串! 收集第1层所有有效串: ["(())()", "()()()"]
//
// 注意: 只删括号不删字母 (字母不影响有效性，删了只会减少结果)
// ============================================================
class Solution1 {
public:
    vector<string> removeInvalidParentheses(string s) {
        vector<string> res;
        unordered_set<string> visited; // 去重: 记录已处理的字符串
        queue<string> q;
        q.push(s);
        visited.insert(s);
        bool found = false; // 是否在当前层找到了有效串

        while (!q.empty()) {
            int size = q.size();
            // 处理当前层的所有串
            for (int i = 0; i < size; i++) {
                string cur = q.front(); q.pop();

                // 检查是否有效
                if (isValid(cur)) {
                    res.push_back(cur);
                    found = true; // 标记: 本层有有效串
                }

                // 如果本层已找到有效串，不需要向下层扩展
                // (BFS 保证本层就是最少删除数)
                if (found) continue;

                // 尝试删除每个括号，生成下一层候选
                for (int j = 0; j < (int)cur.size(); j++) {
                    // 只删括号，不删字母
                    if (cur[j] != '(' && cur[j] != ')') continue;

                    // 生成删除 cur[j] 后的新串
                    string next = cur.substr(0, j) + cur.substr(j + 1);

                    // 去重: 只处理未访问过的串
                    if (!visited.count(next)) {
                        visited.insert(next);
                        q.push(next);
                    }
                }
            }
            // 本层处理完，如果找到有效串就结束
            if (found) break;
        }
        return res;
    }

    bool isValid(const string& s) {
        int count = 0;
        for (char c : s) {
            if (c == '(') count++;
            else if (c == ')') count--;
            if (count < 0) return false; // 右括号多了
        }
        return count == 0; // 左右匹配
    }
};

// ============================================================
// 解法2: DFS 回溯 (简洁版) — 面试推荐 ⭐
// 时间: O(2^n)  空间: O(n) 递归栈
//
// 【思路】
// Step 1: 预计算需要删除的左括号数 rmL 和右括号数 rmR
//   扫描 s: 遇 '(' → rmL++
//           遇 ')' → 如果 rmL>0 则 rmL-- (配对消除)，否则 rmR++
//
// Step 2: DFS 回溯，尝试在每个位置删除括号
//   从 start 位置开始向右搜索 (避免重复)
//   对每个括号:
//     如果是 '(' 且 rmL > 0 → 尝试删除
//     如果是 ')' 且 rmR > 0 → 尝试删除
//   删完 (rmL==0 且 rmR==0) 后检查有效性
//
// 去重技巧: 跳过连续相同括号
//   例如 "))" 中删 idx=0 和 idx=1 结果相同
//   只在连续相同字符的第一个位置尝试删除:
//     if (i > start && s[i] == s[i-1]) continue;
//
// 【决策树示例】 s = "()())()" → rmL=0, rmR=1
//
//           "()())()"  start=0, rmL=0, rmR=1
//          /    |    \     \
//   删s[1]  删s[3]  删s[4]  删s[6]   (只看')'位置, rmR=1)
//   s[1]=')'  s[3]=')'  s[4]=')'  s[6]=')'
//     |         |         |         |
//  "(())()"  "()()()"  "()()()"  "()()("
//  rmR=0     rmR=0     rmR=0     rmR=0
//  valid?✓   valid?✓   ← 和s[3]重复被跳过!  valid?✗
//
// 结果: ["(())()", "()()()"]
//
// 关于 start 参数的作用:
//   确保每次递归只在 idx >= start 处删除，
//   避免 "先删 idx=4 再删 idx=1" 和 "先删 idx=1 再删 idx=4"
//   产生相同结果。保证删除操作的索引是单调递增的。
// ============================================================
class Solution2 {
public:
    vector<string> res;

    vector<string> removeInvalidParentheses(string s) {
        // Step 1: 计算需要删除的左/右括号数
        int rmL = 0, rmR = 0;
        for (char c : s) {
            if (c == '(') {
                rmL++;
            } else if (c == ')') {
                if (rmL > 0) rmL--; // 与一个左括号配对
                else rmR++;         // 多余的右括号
            }
        }

        // Step 2: DFS 回溯
        dfs(s, 0, rmL, rmR);
        return res;
    }

    void dfs(string s, int start, int rmL, int rmR) {
        // 终止: 不需要再删除任何括号
        if (rmL == 0 && rmR == 0) {
            if (isValid(s)) {
                res.push_back(s);
            }
            return;
        }

        for (int i = start; i < (int)s.size(); i++) {
            // ╔═══════════════════════════════════════╗
            // ║ 关键去重: 跳过连续相同括号             ║
            // ║ 例如 "))" 删第1个和删第2个结果相同     ║
            // ║ 只在连续段的第一个位置尝试删除          ║
            // ╚═══════════════════════════════════════╝
            if (i > start && s[i] == s[i - 1]) continue;

            // 尝试删除左括号
            if (s[i] == '(' && rmL > 0) {
                string next = s.substr(0, i) + s.substr(i + 1);
                dfs(next, i, rmL - 1, rmR);
                // 注意: start 传 i (不是 i+1)，因为删除 s[i] 后
                // 原来的 s[i+1] 变成了新串的 index i
            }

            // 尝试删除右括号
            if (s[i] == ')' && rmR > 0) {
                string next = s.substr(0, i) + s.substr(i + 1);
                dfs(next, i, rmL, rmR - 1);
            }
        }
    }

    bool isValid(const string& s) {
        int count = 0;
        for (char c : s) {
            if (c == '(') count++;
            else if (c == ')') count--;
            if (count < 0) return false;
        }
        return count == 0;
    }
};

// ============================================================
// 解法3: DFS 回溯 (open 计数版) — 更精确的剪枝
// 时间: O(2^n)  空间: O(n)
//
// 【思路】
// 遍历字符串的每个字符，对每个括号做"保留"或"删除"的选择。
// 维护 open 计数器实时检查合法性:
//   保留 '(' → open++
//   保留 ')' → open--（仅当 open > 0 时才允许）
//   删除括号 → rmL-- 或 rmR--
//
// 比解法2的优势: 在搜索过程中就通过 open 剪枝，
// 而不是等到 rmL==rmR==0 才检查 isValid。
//
// 但代码更长，面试中解法2更推荐。
// ============================================================
class Solution3 {
public:
    vector<string> removeInvalidParentheses(string s) {
        int rmL = 0, rmR = 0;
        for (char c : s) {
            if (c == '(') rmL++;
            else if (c == ')') {
                if (rmL > 0) rmL--;
                else rmR++;
            }
        }

        vector<string> res;
        string path;
        dfs(s, 0, rmL, rmR, 0, path, res);
        return res;
    }

    void dfs(const string& s, int idx, int rmL, int rmR, int open,
             string& path, vector<string>& res) {
        if (idx == (int)s.size()) {
            if (rmL == 0 && rmR == 0 && open == 0) {
                res.push_back(path);
            }
            return;
        }

        char c = s[idx];

        // 去重: 跳过与前一个相同的连续括号
        // (仅当选择"删除"时需要去重)

        if (c == '(') {
            // 选择1: 删除这个 '('
            if (rmL > 0) {
                // 去重: 如果前一个字符也被删除且也是 '('，跳过
                // 简化做法: 直接递归，用 rmL 控制
                int k = idx;
                // 找连续 '(' 段的长度，只在第一个位置尝试删除
                while (k + 1 < (int)s.size() && s[k + 1] == '(') k++;
                dfs(s, idx + 1, rmL - 1, rmR, open, path, res);
            }
            // 选择2: 保留这个 '('
            path.push_back('(');
            dfs(s, idx + 1, rmL, rmR, open + 1, path, res);
            path.pop_back();

        } else if (c == ')') {
            // 选择1: 删除这个 ')'
            if (rmR > 0) {
                dfs(s, idx + 1, rmL, rmR - 1, open, path, res);
            }
            // 选择2: 保留这个 ')' (仅当 open > 0，即有匹配的左括号)
            if (open > 0) {
                path.push_back(')');
                dfs(s, idx + 1, rmL, rmR, open - 1, path, res);
                path.pop_back();
            }

        } else {
            // 字母: 直接保留，不能删除
            path.push_back(c);
            dfs(s, idx + 1, rmL, rmR, open, path, res);
            path.pop_back();
        }
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间       | 空间     | 去重方式       | 推荐度 |
// |-------------|-----------|---------|---------------|--------|
// | BFS         | O(n*2^n)  | O(2^n)  | set           | ⭐⭐  直觉好 |
// | DFS 简洁版  | O(2^n)    | O(n)    | 跳连续相同字符  | ⭐⭐⭐ 面试首选 |
// | DFS open版  | O(2^n)    | O(n)    | open实时剪枝   | ⭐⭐  代码长 |
//
// ============================================================
// 【rmL 和 rmR 的计算原理】
//
// 扫描字符串 s，维护两个计数器:
//   rmL: 需要删除的多余左括号数
//   rmR: 需要删除的多余右括号数
//
// 遍历每个字符:
//   '(' → rmL++ (暂时认为是多余的左括号)
//   ')' → 如果 rmL > 0: rmL-- (与一个待删的左括号配对消除，都不需要删了)
//          否则: rmR++ (真正多余的右括号)
//   字母 → 忽略
//
// 示例: s = "()())()"
//   '(' → rmL=1
//   ')' → rmL>0, rmL=0 (配对)
//   '(' → rmL=1
//   ')' → rmL>0, rmL=0 (配对)
//   ')' → rmL==0, rmR=1 (多余右括号)
//   '(' → rmL=1
//   ')' → rmL>0, rmL=0 (配对)
//   最终: rmL=0, rmR=1 → 只需删1个右括号
//
// ============================================================
// 【易错点】
//
// 1. 忘记处理字母:
//    ✗ 对每个字符都尝试删除 → 删字母是不必要的
//    ✓ 只对 '(' 和 ')' 尝试删除
//
// 2. DFS 去重遗漏:
//    ✗ 不跳过连续相同括号 → 重复结果如 ["()", "()"]
//    ✓ if (i > start && s[i] == s[i-1]) continue;
//
// 3. rmL/rmR 计算:
//    ✗ rmL = count('(') - count(')') → 无法区分哪边多
//    ✓ 用配对消除: ')' 遇 rmL>0 则配对, 否则 rmR++
//
// 4. BFS 中 found 后仍扩展:
//    ✗ 找到有效串后仍生成下层 → 非最少删除结果
//    ✓ if (found) continue; → 不扩展
//
// 5. DFS start 传值:
//    ✗ dfs(next, i+1, ...) → 跳过了新串中 idx=i 位置(原 i+1)
//    ✓ dfs(next, i, ...)   → 删除 s[i] 后, 原 s[i+1] 变成 next[i]
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: BFS 和 DFS 哪个更好?
//     DFS + rmL/rmR 更好。BFS 的 set 去重占空间大，
//     且每层生成大量无效候选。DFS 有目标地删除。
//
// Q2: 如果只需返回一个有效结果?
//     DFS 找到第一个就返回。或贪心: 从左到右删多余')',
//     再从右到左删多余'(', O(n)。
//
// Q3: 怎么理解 start 参数?
//     保证删除操作的索引单调递增。避免 "先删 idx=3 再删 idx=1"
//     和 "先删 idx=1 再删 idx=3" 产生相同结果。
//
// Q4: 复杂度怎么分析?
//     最坏 O(2^n): 每个括号可删/不删。但剪枝(rmL/rmR限制、
//     open约束、跳连续字符)使实际远小于理论上界。
//     n<=25, 2^20 ≈ 10^6, 完全够。
//
// ============================================================
// 【相关题型】
//
// - 20. 有效的括号: isValid 的来源，栈匹配
// - 22. 括号生成: 回溯生成所有有效括号组合
// - 1249. 移除无效的括号: 只需返回一个结果，栈 O(n) 解决
// - 32. 最长有效括号: DP 或栈，找最长有效子串
// - 678. 有效的括号字符串: 含 '*' 通配符，贪心解法
// ============================================================
