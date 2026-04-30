/*
 * 【题目本质】
 * 因式分解的所有组合 —— 回溯搜索因子，保持非递减避免重复
 *
 * 【解法总览】
 * 解法1: 回溯 (枚举到 sqrt)     O(2^logn) / O(logn)  ⭐推荐
 * 解法2: 回溯 (枚举到 n 的写法)  O(2^logn) / O(logn)
 */

// ===================== 解法1: 回溯 (枚举到 sqrt，更高效) =====================
// 思路: 从 start 枚举因子 i 到 sqrt(n)，每找到 i 是 n 的因子：
//   1. [path..., i, n/i] 直接是一个合法分解
//   2. 继续递归分解 n/i（因子从 i 开始，保证非递减）
class Solution1 {
public:
    vector<vector<int>> getFactors(int n) {
        vector<vector<int>> res;
        vector<int> path;
        dfs(n, 2, path, res);
        return res;
    }

    void dfs(int n, int start, vector<int>& path, vector<vector<int>>& res) {
        for (int i = start; (long long)i * i <= n; i++) {
            if (n % i == 0) {
                // 方案一: i 和 n/i 构成一对因子，直接收集
                path.push_back(i);
                path.push_back(n / i);
                res.push_back(path);
                path.pop_back(); // 只弹出 n/i

                // 方案二: i 作为因子，继续递归分解 n/i
                dfs(n / i, i, path, res);
                path.pop_back(); // 弹出 i
            }
        }
    }
};

// ===================== 解法2: 回溯 (枚举到 n，n==1时收集) =====================
// 思路: 因子枚举从 start 到 n，当 n 被分解为 1 时，
//       若 path 长度 > 1 则是合法分解（排除 [n] 本身）
class Solution2 {
public:
    vector<vector<int>> getFactors(int n) {
        vector<vector<int>> res;
        vector<int> path;
        dfs(n, 2, path, res);
        return res;
    }

    void dfs(int n, int start, vector<int>& path, vector<vector<int>>& res) {
        if (n == 1) {
            if (path.size() > 1) { // 排除 [n] 本身
                res.push_back(path);
            }
            return;
        }
        // 枚举因子 i: start <= i <= n
        for (int i = start; i <= n; i++) {
            if (n % i == 0) {
                path.push_back(i);
                dfs(n / i, i, path, res);   // 下一个因子 >= i（非递减）
                path.pop_back();
            }
        }
    }
};

/*
 * 【解法对比】
 * ┌──────────────────────┬───────────┬──────────┬────────────────┐
 * │ 解法                 │ 时间      │ 空间     │ 特点           │
 * ├──────────────────────┼───────────┼──────────┼────────────────┤
 * │ 回溯(sqrt上界)       │ O(2^logn) │ O(logn)  │ 更快，面试推荐 │
 * │ 回溯(枚举到n)        │ O(2^logn) │ O(logn)  │ 逻辑更清晰     │
 * └──────────────────────┴───────────┴──────────┴────────────────┘
 *
 * 【易错点】
 * 1. 因子从 start 开始枚举，不是从 2，否则产生重复组合
 * 2. 解法1中先 push i 和 n/i 收集结果，再 pop n/i 继续递归
 * 3. 解法2中 path.size() > 1 的检查：排除 [n] 本身
 * 4. i*i <= n 要注意 int 溢出，用 (long long)i * i
 *
 * 【面试追问】
 * Q1: 如何只返回质因数分解？ → 只枚举素数因子
 * Q2: 这和 Combination Sum 有什么异同？
 *     → 都是回溯，但这里是乘法关系而非加法
 * Q3: n 很大时如何优化？ → 先质因数分解，再组合质因子
 */
