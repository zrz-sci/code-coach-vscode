/*
 * 386. Lexicographical Numbers
 * 难度: Medium
 * 标签: Depth-First Search, Trie
 *
 * 【题目本质】
 * 将 [1, n] 的整数按字典序排列。
 * 等价于一棵隐式 10 叉 Trie 的前序遍历。
 *
 * 【解法总览】
 * 解法1: 迭代模拟 DFS - O(1) 额外空间
 * 解法2: 递归 DFS - 直观清晰
 */

// ===================== Solution 1: 迭代模拟 DFS =====================
// 思路：用 cur 模拟 DFS 指针，优先深入(cur*10)，否则移向兄弟(cur+1)或回溯(cur/10)
// 时间：O(n)
// 空间：O(1)（不计结果数组）
class Solution1 {
public:
    vector<int> lexicalOrder(int n) {
        vector<int> result;
        int cur = 1;
        for (int i = 0; i < n; i++) {
            result.push_back(cur);
            if (cur * 10 <= n) {
                cur *= 10;              // 深入子节点：1→10
            } else {
                // 需要回溯的情况：末位是9 或 cur+1超出n
                while (cur % 10 == 9 || cur + 1 > n) {
                    cur /= 10;          // 回溯到父节点
                }
                cur++;                  // 移向兄弟节点：11→12
            }
        }
        return result;
    }
};

// ===================== Solution 2: 递归 DFS =====================
// 思路：模拟10叉Trie前序遍历，根的子节点1-9，每个节点x的子节点是x*10+0到x*10+9
// 时间：O(n)
// 空间：O(log n) 递归栈深度
class Solution2 {
    void dfs(int cur, int n, vector<int>& result) {
        if (cur > n) return;
        result.push_back(cur);
        for (int i = 0; i <= 9; i++) {
            int next = cur * 10 + i;
            if (next > n) return;       // 剪枝
            dfs(next, n, result);
        }
    }
public:
    vector<int> lexicalOrder(int n) {
        vector<int> result;
        for (int i = 1; i <= 9; i++) {  // 根的子节点从1开始
            dfs(i, n, result);
        }
        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 空间 | 代码量 | 特点 |
 * |------|------|--------|------|
 * | 迭代 | O(1) | 中等 | 最优空间，面试首选 |
 * | 递归 | O(log n) | 简洁 | 逻辑清晰，递归栈深=位数 |
 *
 * 【易错点】
 * 1. 迭代法回溯条件必须用 while 循环（可能连续回溯多层）
 * 2. 递归中子节点追加 0-9，但根节点从 1 开始（无前导零）
 * 3. cur+1 > n 也是回溯条件（不仅仅是 cur%10==9）
 * 4. 剪枝时 cur*10+i > n 直接 return（后续更大的 i 也超限）
 *
 * 【面试追问】
 * Q1: O(1) 空间如何实现 → 迭代模拟 DFS 三个动作：深入/兄弟/回溯
 * Q2: 求第 k 个字典序数 → LeetCode 440，计算子树大小跳过
 * Q3: 与 Trie 的关系 → 隐式 10 叉 Trie 前序遍历
 */
