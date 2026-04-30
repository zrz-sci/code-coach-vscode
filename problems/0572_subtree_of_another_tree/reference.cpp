// =============================================
// LeetCode 572: Subtree of Another Tree
// =============================================

// 【题目本质】
// 在主树中查找是否存在一个节点，使得以它为根的子树
// 与给定的 subRoot 完全相同（结构 + 值都一致）。

// 【解法总览】
// 1. DFS 暴力匹配 O(m*n)  ← 推荐
// 2. 序列化 + 字符串匹配 O(m+n)
// 3. 树哈希 O(m+n)

// =============================================
// 解法一: DFS 暴力匹配（推荐）
// 遍历 root 每个节点，对每个节点调用 isSame 判断
// 时间 O(m*n), 空间 O(h)
// =============================================
class Solution {
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        if (!root) return false;
        if (isSame(root, subRoot)) return true;
        return isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
    }

private:
    bool isSame(TreeNode* a, TreeNode* b) {
        if (!a && !b) return true;
        if (!a || !b) return false;
        return a->val == b->val
            && isSame(a->left, b->left)
            && isSame(a->right, b->right);
    }
};

// =============================================
// 解法二: 序列化 + 字符串匹配
// 将两棵树序列化为字符串，用 find (或 KMP) 判断子串
// 时间 O(m+n), 空间 O(m+n)
// =============================================
class Solution2 {
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        string s1 = serialize(root);
        string s2 = serialize(subRoot);
        return s1.find(s2) != string::npos;
    }

private:
    string serialize(TreeNode* node) {
        if (!node) return ",#";
        // 分隔符防止 "12" 匹配 "1" + "2" 的歧义
        return "," + to_string(node->val) + serialize(node->left) + serialize(node->right);
    }
};

// =============================================
// 解法三: 树哈希
// 对每棵子树计算 hash，先匹配 hash 再验证
// 时间 O(m+n), 空间 O(m+n)
// =============================================
class Solution3 {
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        size_t targetHash = computeHash(subRoot);
        return dfs(root, subRoot, targetHash);
    }

private:
    size_t computeHash(TreeNode* node) {
        if (!node) return 0x9e3779b9;  // 空节点用特殊常量
        size_t lh = computeHash(node->left);
        size_t rh = computeHash(node->right);
        size_t h = hash<int>()(node->val);
        // 组合哈希：异或 + 位移避免交换律碰撞
        return h ^ (lh * 6364136223846793005ULL + 1) ^ (rh * 2654435769ULL + 3);
    }

    bool isSame(TreeNode* a, TreeNode* b) {
        if (!a && !b) return true;
        if (!a || !b) return false;
        return a->val == b->val && isSame(a->left, b->left) && isSame(a->right, b->right);
    }

    bool dfs(TreeNode* node, TreeNode* subRoot, size_t targetHash) {
        if (!node) return false;
        if (computeHash(node) == targetHash && isSame(node, subRoot)) return true;
        return dfs(node->left, subRoot, targetHash) || dfs(node->right, subRoot, targetHash);
    }
};

// 【解法对比】
// | 解法         | 时间    | 空间    | 优点         | 缺点            |
// |-------------|---------|---------|-------------|-----------------|
// | DFS 暴力     | O(m*n)  | O(h)   | 简单直观      | 最坏情况慢       |
// | 序列化+匹配   | O(m+n)  | O(m+n) | 线性时间      | 序列化占空间      |
// | 树哈希        | O(m+n)  | O(m+n) | 理论最优      | 哈希碰撞需验证    |

// 【易错点】
// 1. isSame 必须检查两个都为 null → true；一个 null → false
// 2. 序列化必须加分隔符，否则值域歧义（如 1,2 vs 12）
// 3. 子树定义：必须包含某节点的全部后代（不能只截取一部分）

// 【面试追问】
// Q1: 如何将暴力 O(m*n) 优化到 O(m+n)？→ 序列化+KMP 或树哈希
// Q2: 与 Same Tree (100) 的关系？→ isSame 就是 100 题的解
// Q3: 如果允许子树缺少叶节点呢？→ 那就是"子结构"问题（剑指 Offer 26）
