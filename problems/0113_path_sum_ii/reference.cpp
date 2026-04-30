/*
 * LeetCode 113: Path Sum II (路径总和 II)
 *
 * 【题目本质】
 * 给定二叉树和目标和 targetSum，找出所有从根到叶子的路径，
 * 使得路径上节点值之和等于 targetSum。
 *
 * 【ASCII 树示例】
 *              5
 *             / \
 *            4   8
 *           /   / \
 *          11  13   4
 *         / \      / \
 *        7   2    5   1
 *
 *   targetSum = 22
 *   答案: [[5,4,11,2], [5,8,4,5]]
 *
 * 【解法总览】
 * 解法1: DFS 回溯（递归）— O(n*H) / O(H)   — 面试首选，简洁优雅
 * 解法2: DFS 迭代（栈） — O(n*H) / O(n*H) — 追问"不用递归"
 * 解法3: 带父指针的 DFS — O(n*H) / O(n)   — 优化迭代解法的空间
 */

// ============================================================
// 解法1: DFS 回溯（递归）— 面试首选 ⭐
// 时间: O(n * H)  空间: O(H) (递归栈 + path)
//
// 【思路】
// 维护一个 path 向量记录当前路径。
// 每进入一个节点：
//   1. 把节点值加入 path
//   2. 如果是叶子且 remain == node->val → 收集答案
//   3. 递归左右子树，remain 减去当前节点值
//   4. 回溯：pop_back
//
// 【DFS 执行过程 (targetSum=22)】
//   进入 5:  path=[5], remain=22
//     进入 4:  path=[5,4], remain=17
//       进入 11: path=[5,4,11], remain=13
//         进入 7:  path=[5,4,11,7], remain=2
//           叶子! 2≠7 ✗ → pop → path=[5,4,11]
//         进入 2:  path=[5,4,11,2], remain=2
//           叶子! 2==2 ✓ → 收集! → pop → path=[5,4,11]
//         pop → path=[5,4]
//       pop → path=[5]
//     进入 8:  path=[5,8], remain=14
//       进入 13: path=[5,8,13], remain=9
//         叶子! 9≠13 ✗ → pop → path=[5,8]
//       进入 4:  path=[5,8,4], remain=9
//         进入 5: path=[5,8,4,5], remain=5
//           叶子! 5==5 ✓ → 收集! → pop → path=[5,8,4]
//         进入 1: path=[5,8,4,1], remain=5
//           叶子! 5≠1 ✗ → pop → path=[5,8,4]
//         pop → path=[5,8]
//       pop → path=[5]
//     pop → path=[]
//
// 关键细节：
//   - remain == node->val 等价于 remain - node->val == 0
//   - 节点值可能为负，不能提前剪枝！
//   - pop_back 在所有递归调用之后，确保回溯正确
// ============================================================
class Solution1 {
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        vector<vector<int>> res;
        vector<int> path;
        dfs(root, targetSum, path, res);
        return res;
    }

private:
    void dfs(TreeNode* node, int remain, vector<int>& path,
             vector<vector<int>>& res) {
        if (!node) return;

        // 选择：把当前节点加入路径
        path.push_back(node->val);

        // 叶子节点检查
        if (!node->left && !node->right && remain == node->val) {
            res.push_back(path); // 拷贝当前路径
        }

        // 递归左右子树
        dfs(node->left, remain - node->val, path, res);
        dfs(node->right, remain - node->val, path, res);

        // 回溯：撤销选择
        path.pop_back();
    }
};

// ============================================================
// 解法2: DFS 迭代（用栈模拟递归）
// 时间: O(n * H)  空间: O(n * H)
//
// 【思路】
// 用显式栈替代递归。栈中每个帧存储：
//   - 当前节点
//   - 到达当前节点的路径（副本）
//   - 剩余需要的目标和
//
// 注意：因为没有回溯机制，每次入栈都要拷贝路径，
// 空间开销比递归版本大很多。
//
// 面试中通常不需要写这个版本，除非面试官要求
// "不用递归"。递归版本更优。
// ============================================================
class Solution2 {
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        vector<vector<int>> res;
        if (!root) return res;

        // 栈存: {节点, 到达该节点的路径, 剩余目标和}
        stack<tuple<TreeNode*, vector<int>, int>> stk;
        stk.push({root, {root->val}, targetSum - root->val});

        while (!stk.empty()) {
            auto [node, path, remain] = stk.top();
            stk.pop();

            // 叶子节点检查
            if (!node->left && !node->right && remain == 0) {
                res.push_back(path);
            }

            // 先 push right 再 push left，这样左子树先处理（模拟前序遍历）
            for (auto child : {node->right, node->left}) {
                if (child) {
                    auto newPath = path;            // 路径副本
                    newPath.push_back(child->val);
                    stk.push({child, newPath, remain - child->val});
                }
            }
        }
        return res;
    }
};

// ============================================================
// 解法3: DFS 回溯 — 变体：用减法代替传递 remain
// 时间: O(n * H)  空间: O(H)
//
// 【思路】
// 和解法1本质相同，但用不同的参数传递方式。
// 递归时直接传 targetSum，每层减去当前节点值。
// 到叶子时检查 targetSum == 0。
//
// 这个变体的好处是判断条件更直观：remain 正好减到 0。
// ============================================================
class Solution3 {
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        vector<vector<int>> res;
        vector<int> path;
        dfs(root, targetSum, path, res);
        return res;
    }

private:
    void dfs(TreeNode* node, int remain, vector<int>& path,
             vector<vector<int>>& res) {
        if (!node) return;

        path.push_back(node->val);
        remain -= node->val; // 先减去当前节点值

        // 叶子节点且剩余为 0
        if (!node->left && !node->right && remain == 0) {
            res.push_back(path);
        }

        dfs(node->left, remain, path, res);
        dfs(node->right, remain, path, res);

        path.pop_back(); // 回溯
        // 注意: remain 是值传递，不需要手动恢复
    }
};

// ============================================================
// 解法4: BFS + 队列（层序遍历记录路径）
// 时间: O(n * H)  空间: O(n * H)
//
// 【思路】
// 层序遍历，队列中每个元素存 {节点, 路径, 剩余和}。
// 到叶子时检查剩余和是否为 0。
// 这种方式空间很大（每个队列元素都存路径副本），
// 一般不推荐，但展示了 BFS 也能解决路径问题。
// ============================================================
class Solution4 {
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        vector<vector<int>> res;
        if (!root) return res;

        queue<tuple<TreeNode*, vector<int>, int>> q;
        q.push({root, {root->val}, targetSum - root->val});

        while (!q.empty()) {
            auto [node, path, remain] = q.front();
            q.pop();

            if (!node->left && !node->right && remain == 0) {
                res.push_back(path);
            }

            for (auto child : {node->left, node->right}) {
                if (child) {
                    auto newPath = path;
                    newPath.push_back(child->val);
                    q.push({child, newPath, remain - child->val});
                }
            }
        }
        return res;
    }
};

/*
 * 易错点总结:
 * 1. 叶子节点判定: 必须 !left && !right 同时成立
 *    - 错误: if (!node) 只检查了空节点，不是叶子
 *    - 错误: if (!node->left || !node->right) 只有一边为空不算叶子
 * 2. 回溯时 pop_back 不能忘
 *    - path 是引用传递，push_back 和 pop_back 必须配对
 * 3. 节点值可能为负，不能提前剪枝
 *    - 即使 remain < 0，后面可能有负值节点
 * 4. 迭代解法中路径需要拷贝
 *    - 递归用引用+回溯避免拷贝，迭代没法回溯所以必须拷贝
 *
 * 面试追问:
 * Q: 只判断是否存在路径? → LC 112, 返回 bool 即可
 * Q: 路径不从根开始?    → LC 437, 前缀和+哈希表
 * Q: 时间复杂度分析?    → O(n*H), H=树高, 拷贝路径的开销
 * Q: 空间能否优化?      → 递归版 O(H) 已经最优（不含结果空间）
 *
 * ============================================================
 * 总结：解法对比 & 面试要点
 * ============================================================
 *
 * 【解法选择决策树】
 *
 *   需要所有路径？
 *    ├─ YES → DFS回溯(递归) O(n*H), O(H)  ← 面试首选
 *    │         核心: push_back → 递归 → pop_back 三步配对
 *    │         迭代栈版 O(n*H), O(n*H)    ← 追问"不用递归"时用
 *    └─ 只需判断存在 → LC112, 返回bool即可，不需要path
 *
 *   路径不从根开始？ → LC437, 前缀和+哈希表
 *
 * | 解法            | 时间     | 空间     | 面试推荐 |
 * |----------------|---------|---------|---------|
 * | DFS回溯(递归)   | O(n*H)  | O(H)   | ⭐⭐⭐  |
 * | DFS迭代(栈)     | O(n*H)  | O(n*H) | ⭐     |
 * | DFS变体(先减)   | O(n*H)  | O(H)   | ⭐⭐    |
 * | BFS(队列)       | O(n*H)  | O(n*H) | ⭐     |
 *
 * 【核心易错点】
 * 1. 叶子节点判定必须是 !left && !right 同时成立
 *    错: if(!node) — 这是空节点不是叶子
 *    错: if(!node->left || !node->right) — 只有一边空不算叶子
 * 2. 回溯 pop_back 不能遗漏 — path是引用传递，push和pop必须配对
 *    递归左右子树之后才pop，不是之前
 * 3. 节点值可能为负，不能用 remain<0 做剪枝
 *    例: 路径 [5, -3, 8] remain可以先变负再变正
 * 4. 收集答案时 res.push_back(path) 是拷贝 — path后续还会变
 *    不能 push 引用或指针
 * 5. 迭代解法中每次入栈都要拷贝路径 — 没有回溯机制所以无法避免
 *
 * 【面试追问链 Q1 → Q2 → Q3】
 *
 * Q1: 如果只需要判断是否存在满足条件的路径？(LC 112)
 *   → 不需要 path 向量和 res 收集，直接返回 bool。
 *     叶子节点时 return remain == node->val。
 *     非叶子: return dfs(left) || dfs(right)。
 *
 * Q2: 如果路径不必从根节点开始、不必到叶子节点结束？(LC 437)
 *   → 前缀和 + 哈希表。对每个节点维护从根到当前的前缀和 prefixSum，
 *     查找 HashMap 中是否存在 prefixSum - targetSum。
 *     时间 O(n)，空间 O(H)。注意回溯时要从 HashMap 中移除当前前缀和。
 *
 * Q3: 时间复杂度为什么是 O(n*H) 而不是 O(n)？
 *   → 虽然每个节点只访问一次 O(n)，但每次收集答案时
 *     res.push_back(path) 会拷贝整条路径，长度最多为 H（树高）。
 *     最坏情况下每个叶子都是合法路径，叶子数 O(n)，每次拷贝 O(H)，
 *     总共 O(n*H)。对于平衡树 H=logn，总时间 O(nlogn)。
 */
