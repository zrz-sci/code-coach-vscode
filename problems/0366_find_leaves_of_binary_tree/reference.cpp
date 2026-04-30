/*
 * LeetCode 366: 寻找二叉树的叶子节点 (Find Leaves of Binary Tree)
 *
 * 【题目本质】
 * 按节点的"高度"（叶子=0，往根方向递增）对二叉树节点分组。
 * 每一轮剥掉的叶子，恰好是当前高度最小的那一层。
 *
 * 【解法总览】
 * 解法1: 暴力模拟（多轮剥叶子）  — O(n²) / O(n) — 直觉模拟
 * 解法2: 后序DFS按高度分组       — O(n) / O(n) — ⭐ 面试首选
 * 解法3: BFS拓扑排序             — O(n) / O(n) — 迭代替代方案
 */

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

// ============================================================
// 解法1: 暴力模拟 — 每轮找叶子、收集、删除
// 时间: O(n²) 最坏（链表形态树，每轮删一个节点）
// 空间: O(n)  递归栈 + 结果
//
// 【思路】
// 直接模拟题意：
//   while 树还在:
//     遍历整棵树，找到所有叶子（左右孩子都为null）
//     收集叶子值，把父节点对应指针设为null（"删除"叶子）
//
// 瓶颈：每一轮都要完整遍历剩余树。如果树是链表，
// 第1轮遍历n个，第2轮n-1个...总共 n+(n-1)+...+1 = O(n²)
//
// 示例过程:
//       1              1            1
//      / \            / \          
//     2   3    →     2       →           → 空
//    / \
//   4   5
//  轮1:[4,5,3]     轮2:[2]       轮3:[1]
// ============================================================
class Solution1 {
public:
    vector<vector<int>> findLeaves(TreeNode* root) {
        vector<vector<int>> result;
        while (root) {
            vector<int> leaves;
            root = removeLeaves(root, leaves);
            result.push_back(leaves);
        }
        return result;
    }

private:
    // 后序遍历：先处理子树，再判断自己是否变成叶子
    // 返回值：修改后的子树根（叶子被删返回nullptr）
    TreeNode* removeLeaves(TreeNode* node, vector<int>& leaves) {
        if (!node) return nullptr;

        // 当前就是叶子 → 收集并删除
        if (!node->left && !node->right) {
            leaves.push_back(node->val);
            return nullptr;  // 告诉父节点"我已被删"
        }

        // 递归处理子树，必须接收返回值更新指针
        node->left = removeLeaves(node->left, leaves);
        node->right = removeLeaves(node->right, leaves);
        return node;
    }
};

// ============================================================
// 解法2: 后序DFS按高度分组 — 一次遍历，O(n)  ⭐ 面试首选
// 时间: O(n)  每个节点恰好访问一次
// 空间: O(n)  递归栈O(h) + 结果O(n)
//
// 【思路】
// 关键洞察：节点在第几轮被剥掉 = 节点的"高度"
//   - 叶子高度 = 0  → 第0轮被剥掉
//   - 其他节点高度 = 1 + max(左子高度, 右子高度)
//   - 高度就是在结果数组中的下标
//
// 因此只需一次后序DFS（自底向上），算每个节点高度，
// 按高度放入 result[height] 即可。
//
// 示例详细推演:
//       1  (h=2)
//      / \
//     2   3  (h=1, h=0)
//    / \
//   4   5  (h=0, h=0)
//
// 后序遍历顺序: 4 → 5 → 2 → 3 → 1
//
//   节点4: left=null(-1), right=null(-1)
//          h = 1 + max(-1,-1) = 0 → result[0].push(4)
//
//   节点5: left=null(-1), right=null(-1)
//          h = 1 + max(-1,-1) = 0 → result[0].push(5)
//
//   节点2: left=4(h=0), right=5(h=0)
//          h = 1 + max(0,0) = 1   → result[1].push(2)
//
//   节点3: left=null(-1), right=null(-1)
//          h = 1 + max(-1,-1) = 0 → result[0].push(3)
//
//   节点1: left=2(h=1), right=3(h=0)
//          h = 1 + max(1,0) = 2   → result[2].push(1)
//
// 最终 result = [[4,5,3], [2], [1]]  ✓
// ============================================================
class Solution2 {
public:
    vector<vector<int>> findLeaves(TreeNode* root) {
        vector<vector<int>> result;
        getHeight(root, result);
        return result;
    }

private:
    // 返回节点的高度（叶子=0, null=-1）
    int getHeight(TreeNode* node, vector<vector<int>>& result) {
        if (!node) return -1;  // 为什么是-1？因为叶子要算出0：1+max(-1,-1)=0

        // 后序：先递归左右，再处理自己
        int leftH = getHeight(node->left, result);
        int rightH = getHeight(node->right, result);
        int h = 1 + max(leftH, rightH);

        // 高度h就是result的下标，按需扩容
        // 为什么不预分配？因为不知道树高，动态增长更简洁
        if (h >= (int)result.size()) {
            result.push_back({});
        }
        result[h].push_back(node->val);

        return h;
    }
};

// ============================================================
// 解法3: BFS拓扑排序 — 从叶子向根逐层剥离（迭代方案）
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 把树看成无向图，叶子节点的"度"为0（只有父边，无子边）。
// 类似拓扑排序：
//   1. 统计每个节点的子节点数（度数）
//   2. 度为0的入队（叶子）
//   3. 每轮弹出当前所有度为0的节点，将父节点度数-1
//   4. 新的度为0的节点入队，重复
//
// 和课程表(207/210)的拓扑排序框架一模一样，
// 区别在于这里的图是树，而且要分层收集。
//
// 适用场景：
//   - 树非常深时避免递归栈溢出
//   - 面试官追问"不用递归怎么做"
// ============================================================
class Solution3 {
public:
    vector<vector<int>> findLeaves(TreeNode* root) {
        if (!root) return {};

        // Step1: BFS预处理——建立 parent 映射 + 统计度数
        unordered_map<TreeNode*, int> childCount;  // 子节点数量
        unordered_map<TreeNode*, TreeNode*> parent;
        queue<TreeNode*> bfs;
        bfs.push(root);
        childCount[root] = 0;

        while (!bfs.empty()) {
            TreeNode* node = bfs.front(); bfs.pop();
            if (node->left) {
                childCount[node]++;
                parent[node->left] = node;
                childCount[node->left] = 0;
                bfs.push(node->left);
            }
            if (node->right) {
                childCount[node]++;
                parent[node->right] = node;
                childCount[node->right] = 0;
                bfs.push(node->right);
            }
        }

        // Step2: 叶子节点入队（childCount == 0）
        queue<TreeNode*> q;
        for (auto& [node, cnt] : childCount) {
            if (cnt == 0) q.push(node);
        }

        // Step3: 逐层剥离
        vector<vector<int>> result;
        while (!q.empty()) {
            int sz = q.size();
            vector<int> layer;
            for (int i = 0; i < sz; i++) {
                TreeNode* node = q.front(); q.pop();
                layer.push_back(node->val);
                // 父节点度数-1，如果变成0则入队
                if (parent.count(node)) {
                    childCount[parent[node]]--;
                    if (childCount[parent[node]] == 0) {
                        q.push(parent[node]);
                    }
                }
            }
            result.push_back(layer);
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 方面         | 解法1(暴力模拟) | 解法2(后序DFS) | 解法3(拓扑BFS) |
// |-------------|----------------|----------------|----------------|
// | 时间         | O(n²) 最坏     | O(n)           | O(n)           |
// | 空间         | O(n)           | O(n)           | O(n)           |
// | 修改原树     | 是             | 否             | 否             |
// | 代码简洁度   | 中等           | 最简洁         | 较复杂         |
// | 适用场景     | 理解题意       | 面试首选       | 不能递归时      |
//
// 面试推荐顺序：
//   先说解法1展示理解 → 指出O(n²)瓶颈 → 给出解法2
//   被追问"不用递归"时 → 给出解法3
//
// 【易错点】
//
// 1. 空节点高度返回0而非-1:
//    ✗ if (!node) return 0;
//      → 叶子高度变成 1+max(0,0)=1，所有分组偏移一位
//    ✓ if (!node) return -1;
//      → 叶子高度 = 1+max(-1,-1) = 0，正确
//
// 2. result数组越界:
//    ✗ result[h].push_back(node->val);  // h可能超出size
//    ✓ if (h >= (int)result.size()) result.push_back({});
//      result[h].push_back(node->val);
//
// 3. 暴力解中不接收removeLeaves返回值:
//    ✗ removeLeaves(node->left, leaves);  // 叶子没被"删除"
//    ✓ node->left = removeLeaves(node->left, leaves);  // 接收nullptr
//
// 4. int强转问题:
//    ✗ if (h >= result.size())  // h是int，size()是size_t(unsigned)
//      → 当h=-1时，-1被隐式转为巨大的unsigned值，条件为true
//    ✓ if (h >= (int)result.size())  // 显式转换
//      （虽然本题h不会是-1因为null不进此逻辑，但养成好习惯）
//
// 【面试追问 — 递进链】
//
// Q1: 暴力解时间复杂度？什么输入最坏？
//   → O(n²)，链表形态的树（每轮只剥一个叶子，要n轮）
//
// Q2: 怎么一次遍历解决？
//   → 观察"剥离轮次=节点高度"，后序DFS计算高度并按高度分组
//
// Q3: 不能递归怎么办？（比如树很深，栈溢出）
//   → 方案A: 用显式栈模拟后序遍历 + 哈希表存高度
//   → 方案B: BFS拓扑排序，从叶子逐层剥离（解法3）
//
// Q4: 如果要返回节点指针而不是值？
//   → 同样的框架，result类型改为 vector<vector<TreeNode*>> 即可
//
// Q5: 这道题和"二叉树的直径(543)"有什么联系？
//   → 都是后序DFS自底向上计算高度。543用 leftH+rightH 更新直径，
//     本题用高度做分组。掌握"后序DFS返回高度"是两题的共同核心。
// ============================================================
