/*
 * 【题目本质】
 * 后序遍历计算每个节点的子树元素和，统计出现频率最高的子树和。
 *
 * 【解法总览】
 * 解法一：DFS + 哈希表（推荐）    O(n) / O(n)
 * 解法二：DFS + 一次遍历优化       O(n) / O(n)
 */

// ===================== 解法一：DFS + 哈希表 =====================
// 思路：后序遍历算子树和，哈希表统计频率，最后收集最大频率的和
class Solution1 {
public:
    vector<int> findFrequentTreeSum(TreeNode* root) {
        unordered_map<int, int> freq;  // 子树和 -> 出现次数
        int maxFreq = 0;

        // 后序遍历，返回以 node 为根的子树和
        function<int(TreeNode*)> dfs = [&](TreeNode* node) -> int {
            if (!node) return 0;
            int sum = node->val + dfs(node->left) + dfs(node->right);
            maxFreq = max(maxFreq, ++freq[sum]);
            return sum;
        };

        dfs(root);

        // 收集所有频率 == maxFreq 的子树和
        vector<int> res;
        for (auto& [s, c] : freq) {
            if (c == maxFreq) res.push_back(s);
        }
        return res;
    }
};

// ===================== 解法二：一次遍历优化 =====================
// 思路：在 DFS 过程中实时维护结果列表，避免第二次扫描哈希表
class Solution2 {
public:
    vector<int> findFrequentTreeSum(TreeNode* root) {
        unordered_map<int, int> freq;
        int maxFreq = 0;
        vector<int> res;

        function<int(TreeNode*)> dfs = [&](TreeNode* node) -> int {
            if (!node) return 0;
            int sum = node->val + dfs(node->left) + dfs(node->right);
            int cnt = ++freq[sum];
            if (cnt > maxFreq) {
                maxFreq = cnt;
                res.clear();          // 新的最大频率，清空之前的
                res.push_back(sum);
            } else if (cnt == maxFreq) {
                res.push_back(sum);   // 频率并列，追加
            }
            return sum;
        };

        dfs(root);
        return res;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优点 | 缺点 |
 * |------|------|------|------|------|
 * | DFS+哈希表 | O(n) | O(n) | 逻辑清晰 | 需两次遍历 |
 * | 一次遍历优化 | O(n) | O(n) | 只遍历一次 | 代码稍复杂 |
 *
 * 【易错点】
 * 1. 空节点返回 0，不是不返回
 * 2. 子树和可为负数，不能用数组索引
 * 3. 可能有多个并列最高频率的和，需全部返回
 *
 * 【面试追问】
 * Q1: 树极深时如何避免栈溢出？→ 迭代后序遍历
 * Q2: 如果只需要出现次数最多的某一个子树和？→ 只保留第一个即可
 * Q3: 能否 O(1) 空间？→ 不行，需要哈希表统计频率
 */
