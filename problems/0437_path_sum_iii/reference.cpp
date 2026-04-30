/*
 * LeetCode 437: 路径总和 III (Path Sum III)
 *
 * 【题目本质】
 * 在二叉树中找所有从上到下的连续路径，使路径和等于 targetSum。
 * 等价于"在数组中找和为 target 的连续子数组"搬到树上。
 *
 * 【解法总览】
 * 解法1: 暴力双重 DFS      — O(n²) / O(n) — 直觉，对每个节点为起点 DFS
 * 解法2: 前缀和 + 哈希表    — O(n)  / O(n) — 最优，类比数组前缀和
 */

// ============================================================
// 解法1: 暴力双重 DFS — 对每个节点为起点，向下 DFS 求和
// 时间: O(n²) 最坏（链表形状），O(n log n) 平衡树
// 空间: O(n) 递归栈深度
//
// 【思路】
// 路径可以从任意节点开始，那就用两层递归：
// 外层：遍历树的每个节点作为"起点"
// 内层：从该起点向下走，累加路径和，等于 target 就 +1
//
//        10                 以 5 为起点：
//       /  \                5 → 3 = 8 ✓
//      5   -3               5 → 2 → 1 = 8 ✓
//     / \    \              以 -3 为起点：
//    3   2    11            -3 → 11 = 8 ✓
//   / \   \                 共 3 条路径
//  3  -2   1
//
// 注意：节点值可以为负，所以找到一条后不能停，
// 后面可能又有"拉回来再凑出"的路径。
// ============================================================
class Solution1 {
public:
    int pathSum(TreeNode* root, int targetSum) {
        if (!root) return 0;
        // 外层递归：以每个节点为起点
        return countFrom(root, targetSum)
             + pathSum(root->left, targetSum)
             + pathSum(root->right, targetSum);
    }

private:
    // 内层递归：从 node 出发向下走，统计和为 remain 的路径数
    // 用 long long 防止中间累加溢出（节点值 ±10^9，路径最长 1000）
    int countFrom(TreeNode* node, long long remain) {
        if (!node) return 0;
        int count = 0;
        if (node->val == remain) count = 1;
        // 即使已经找到一条，也不能 return——后续可能有负数"拉回来"
        count += countFrom(node->left, remain - node->val);
        count += countFrom(node->right, remain - node->val);
        return count;
    }
};


// ============================================================
// 解法2: 前缀和 + 哈希表 — 一次 DFS，O(n)
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法1的瓶颈：每个节点都要向下遍历整棵子树，重复计算大量路径和。
//
// 类比数组问题 "和为 k 的子数组" (LeetCode 560):
//   arr:      [a,  b,  c,  d,  e]
//   prefix: [0, a, a+b, a+b+c, ...]
//   子数组 [b,c] 的和 = prefix[3] - prefix[1]
//   如果 prefix[3] - prefix[1] == target
//   等价于 prefix[1] == prefix[3] - target
//   → 用哈希表记录每个前缀和出现次数，O(1) 查配对
//
// 搬到树上：
//   "根→当前节点"的路径就是一个"数组"
//   currSum = 根到当前节点的前缀和
//   查找路径上有多少祖先节点的前缀和 == currSum - target
//   每找到一个，就对应一条 "祖先→当前节点" 的路径和为 target
//
//        10 (prefixSum=10)
//       /  \
//      5    -3 (prefixSum=7)
//   (ps=15)   \
//     / \      11 (ps=18)
//    3   2
// (ps=18)(ps=17)
//   / \    \
//  3  -2    1
// (21)(16) (18)
//
// 在节点 1 (ps=18): 查找 18-8=10 → map 中有 10 (根节点)
//   → 说明 根→5→2→1 这段路径的"5→2→1"和为 8 ✓
//
// 【关键：回溯】
// 树有分叉！左子树路径上的前缀和不应影响右子树的查询。
// DFS 返回前必须 prefixCount[currSum]--，撤销当前节点的贡献。
// ============================================================
class Solution2 {
public:
    int pathSum(TreeNode* root, int targetSum) {
        unordered_map<long long, int> prefixCount;
        // 空前缀：处理"从根到当前节点路径和恰好 == target"的情况
        prefixCount[0] = 1;
        return dfs(root, 0, targetSum, prefixCount);
    }

private:
    int dfs(TreeNode* node, long long currSum, int target,
            unordered_map<long long, int>& prefixCount) {
        if (!node) return 0;

        currSum += node->val;

        // 步骤1：查找 — 有多少祖先的前缀和 == currSum - target
        // 每一个这样的祖先都对应一条满足条件的路径
        int count = prefixCount[currSum - target];

        // 步骤2：插入 — 把当前前缀和加入 map（供子孙节点查询）
        // 注意：必须先查再插入！否则 target==0 时自己会匹配自己
        prefixCount[currSum]++;

        // 步骤3：递归左右子树
        count += dfs(node->left, currSum, target, prefixCount);
        count += dfs(node->right, currSum, target, prefixCount);

        // 步骤4：回溯 — 离开当前节点，撤销前缀和
        // 这是树和数组的关键区别：数组是线性的不需要回溯，
        // 树有分叉，左子树的前缀和不能"泄漏"到右子树
        prefixCount[currSum]--;

        return count;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度     | 暴力双重 DFS      | 前缀和 + 哈希表   |
// |----------|-------------------|--------------------|
// | 时间     | O(n²) 最坏        | O(n)               |
// | 空间     | O(n) 递归栈       | O(n) 哈希表+栈     |
// | 思路难度 | 低，直觉           | 中，需要前缀和类比  |
// | 面试     | 说出来展示理解     | ⭐ 最终答案         |
// | 核心区别 | 每个起点重复遍历   | 一次遍历，map查配对 |
//
// 【易错点】
//
// 1. 忘记 prefixCount[0] = 1
//    ✗ 不初始化 → 漏掉从根开始的路径（currSum 本身就等于 target 时没有匹配）
//    ✓ prefixCount[0] = 1 代表"空前缀"
//
// 2. 忘记回溯 prefixCount[currSum]--
//    ✗ 左子树路径上的前缀和 "泄漏" 到右子树查询中，导致多计数
//    ✓ DFS 返回前必须撤销
//
// 3. 查询和插入顺序搞反
//    ✗ 先 prefixCount[currSum]++ 再查 prefixCount[currSum - target]
//      → 当 target == 0 时，currSum - target == currSum，自己匹配自己
//    ✓ 先查 prefixCount[currSum - target]，再 prefixCount[currSum]++
//
// 4. 前缀和用 int 导致溢出
//    ✗ int currSum → 节点值 ±10^9，累加 1000 个可能溢出
//    ✓ long long currSum
//
// 5. 暴力解中 countFrom 找到匹配就 return
//    ✗ if (node->val == remain) return 1;  → 漏掉后续负值"拉回来"的路径
//    ✓ count = 1 后继续递归
//
// 【面试追问】
//
// Q1: 暴力解最坏 O(n²) 出现在什么树形状？平衡树呢？
// A1: 最坏是退化为链表（每个节点只有一个孩子），外层 n 个起点，
//     内层每个起点最多走 n 步 → O(n²)。
//     平衡树高度 O(log n)，内层每次最多 O(log n) → O(n log n)。
//
// Q2: 为什么前缀和方法需要回溯？560 题（数组版）不需要回溯啊？
// A2: 数组是线性的，一维扫描，每个前缀和天然只属于一条路径。
//     树有分叉，左子树和右子树是不同的"路径分支"。
//     如果不回溯，左子树路径上的前缀和会被右子树的查询"看到"，
//     但这两者不在同一条根→叶路径上，会导致错误计数。
//
// Q3: 如果要输出所有满足条件的路径（不只是数量），怎么改？
// A3: DFS 中额外维护 vector<int> path 记录当前路径上的节点值。
//     当 prefixCount[currSum - target] > 0 时，从 path 末尾往前截取
//     到对应前缀和位置的子路径，加入结果集。需要注意同一个前缀和
//     可能对应多个位置，可以改用 map<long long, vector<int>> 记录索引。
//
// Q4: 如果路径可以"拐弯"（不限制只向下），怎么做？
// A4: 变成了树上任意两点间路径和问题。可以用树的最近公共祖先(LCA)
//     思路，或树形 DP：对每个节点计算经过该节点的最大路径（类似 124 题）。
//     复杂度分析和实现都会更复杂。
// ============================================================
