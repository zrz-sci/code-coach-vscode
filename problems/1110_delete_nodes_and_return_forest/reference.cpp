// ============================================================
// 1110. Delete Nodes And Return Forest
// ============================================================
// 标签: Tree, DFS, Array, Hash Table, Binary Tree
// 难度: Medium
//
// 核心思路:
//   DFS 遍历，自顶向下传递 isRoot 信息，自底向上返回节点/null。
//   删除节点时返回 null 断开父子连接，子节点被提升为新根。
//
// ASCII 示例:
//       1              删除{3,5}后:     树1: 1     树2: 6   树3: 7
//      / \                                /
//     2   3                              2
//    / \ / \                            /
//   4  5 6  7                          4
// ============================================================


// 树节点定义

// ============================================================
// 解法1: DFS + isRoot 标记 (推荐)
//
// 核心设计:
//   dfs(node, isRoot) 返回:
//     - nullptr: 当前节点被删除，父节点断开连接
//     - node:    当前节点保留
//
//   isRoot 参数:
//     - true:  当前节点可能是森林中某棵树的根
//              (原始根 或 父节点被删除)
//     - false: 当前节点不是新根
//
// 遍历过程 (root=[1,2,3,4,5,6,7], delete=[3,5]):
//
//   dfs(1, isRoot=true)
//     1 不删 且 isRoot → res.add(1)
//     ├─ dfs(2, isRoot=false)  [1不删→子节点非新根]
//     │   2 不删
//     │   ├─ dfs(4, false) → return 4
//     │   └─ dfs(5, false)
//     │       5 要删!
//     │       ├─ dfs(null, true) → null
//     │       └─ dfs(null, true) → null
//     │       return null  [断开2→5的连接]
//     │   2->right = null
//     │   return 2
//     │
//     └─ dfs(3, isRoot=false)
//         3 要删!
//         ├─ dfs(6, true)   [3被删→子节点是新根]
//         │   6 不删 且 isRoot → res.add(6)
//         │   return 6
//         └─ dfs(7, true)
//             7 不删 且 isRoot → res.add(7)
//             return 7
//         return null  [断开1→3的连接]
//     1->right = null
//
//   最终: res = [1, 6, 7]
//
// 时间: O(n)   每个节点访问一次
// 空间: O(n)   递归栈 + HashSet
// ============================================================
class Solution {
public:
    vector<TreeNode*> delNodes(TreeNode* root, vector<int>& to_delete) {
        // 将待删除列表转为 HashSet，O(1) 查找
        unordered_set<int> deleteSet(to_delete.begin(), to_delete.end());
        vector<TreeNode*> result;

        // 从根节点开始 DFS，根节点本身是"根候选"
        dfs(root, true, deleteSet, result);

        return result;
    }

private:
    // 返回值: 当前节点(保留) 或 nullptr(删除)
    // isRoot: 当前节点是否应作为新树根的候选
    TreeNode* dfs(TreeNode* node, bool isRoot,
                  const unordered_set<int>& deleteSet,
                  vector<TreeNode*>& result) {
        if (!node) return nullptr;

        // 判断当前节点是否需要删除
        bool shouldDelete = deleteSet.count(node->val);

        // ---- 收集根节点 ----
        // 条件: (1) 是根候选 且 (2) 不被删除
        if (isRoot && !shouldDelete) {
            result.push_back(node);
        }

        // ---- 递归处理子树 ----
        // 如果当前节点被删除，子节点成为新的根候选 (isRoot=true)
        // 如果当前节点保留，子节点不是根候选 (isRoot=false)
        //
        // 同时，用返回值更新子指针:
        //   如果子节点被删除，dfs返回null → 自动断开连接
        //   如果子节点保留，dfs返回自身 → 连接不变
        node->left  = dfs(node->left,  shouldDelete, deleteSet, result);
        node->right = dfs(node->right, shouldDelete, deleteSet, result);

        // ---- 返回值断开连接 ----
        // 被删除 → 返回 nullptr，父节点的指针变为 null
        // 被保留 → 返回自身，父节点的指针不变
        return shouldDelete ? nullptr : node;
    }
};

// ============================================================
// 解法2: BFS 层序遍历 (替代方案)
//
// 思路:
//   1. 先用 BFS 遍历建立父子关系的 map
//   2. 对每个待删除的节点:
//      a. 将其子节点标记为新根(如果子节点不被删除)
//      b. 让父节点断开与它的连接
//   3. 收集所有新根
//
// 优势: 不需要递归，适合树很深时
// 劣势: 代码更复杂，需要额外的 parent map
//
// 时间: O(n)
// 空间: O(n)
// ============================================================

class Solution_BFS {
public:
    vector<TreeNode*> delNodes(TreeNode* root, vector<int>& to_delete) {
        if (!root) return {};

        unordered_set<int> deleteSet(to_delete.begin(), to_delete.end());
        vector<TreeNode*> result;

        // BFS 遍历，对每个节点判断
        queue<pair<TreeNode*, bool>> q;  // (node, isRoot)
        q.push({root, true});

        while (!q.empty()) {
            auto [node, isRoot] = q.front();
            q.pop();

            bool shouldDelete = deleteSet.count(node->val);

            // 收集根
            if (isRoot && !shouldDelete) {
                result.push_back(node);
            }

            // 处理子节点
            if (node->left) {
                q.push({node->left, shouldDelete});
                // 如果左子需要删除，提前断开
                if (deleteSet.count(node->left->val)) {
                    node->left = nullptr;
                }
            }
            if (node->right) {
                q.push({node->right, shouldDelete});
                if (deleteSet.count(node->right->val)) {
                    node->right = nullptr;
                }
            }

            // 如果当前节点被删除，断开其子节点连接（已经入队了）
            // 注意: 子节点已经被加入队列，这里只是帮父节点断开
        }

        return result;
    }
};

// ============================================================
// 解法3: DFS 后序遍历 (不用 isRoot 参数)
//
// 另一种实现: 不传 isRoot，而是在返回后由调用者判断
// 需要单独处理原始根节点
//
// 时间: O(n)
// 空间: O(n)
// ============================================================
class Solution_PostOrder {
public:
    vector<TreeNode*> delNodes(TreeNode* root, vector<int>& to_delete) {
        unordered_set<int> deleteSet(to_delete.begin(), to_delete.end());
        vector<TreeNode*> result;

        root = helper(root, deleteSet, result);

        // 如果原始根没被删除，需要手动加入结果
        if (root) {
            result.push_back(root);
        }

        return result;
    }

private:
    TreeNode* helper(TreeNode* node,
                     const unordered_set<int>& deleteSet,
                     vector<TreeNode*>& result) {
        if (!node) return nullptr;

        // 后序: 先处理子树
        node->left  = helper(node->left,  deleteSet, result);
        node->right = helper(node->right, deleteSet, result);

        // 再处理当前节点
        if (deleteSet.count(node->val)) {
            // 当前节点要删除
            // 其非空子节点成为新的树根
            if (node->left)  result.push_back(node->left);
            if (node->right) result.push_back(node->right);
            return nullptr;  // 告诉父节点断开连接
        }

        return node;  // 保留
    }
};

// ============================================================
// 测试代码
// ============================================================

// 辅助: 构建测试树
TreeNode* buildTestTree1() {
    // [1,2,3,4,5,6,7]
    auto* n4 = new TreeNode(4);
    auto* n5 = new TreeNode(5);
    auto* n6 = new TreeNode(6);
    auto* n7 = new TreeNode(7);
    auto* n2 = new TreeNode(2, n4, n5);
    auto* n3 = new TreeNode(3, n6, n7);
    auto* n1 = new TreeNode(1, n2, n3);
    return n1;
}

TreeNode* buildTestTree2() {
    // [1,2,4,null,3]
    auto* n3 = new TreeNode(3);
    auto* n4 = new TreeNode(4);
    auto* n2 = new TreeNode(2, nullptr, n3);
    auto* n1 = new TreeNode(1, n2, n4);
    return n1;
}

void test() {
    // 测试1: [1,2,3,4,5,6,7], delete=[3,5] → 3 棵树
    {
        Solution sol;
        TreeNode* root = buildTestTree1();
        vector<int> del = {3, 5};
        auto result = sol.delNodes(root, del);
        assert(result.size() == 3);
        // 根应该是 1, 6, 7
    }

    // 测试2: [1,2,4,null,3], delete=[3] → 1 棵树
    {
        Solution sol;
        TreeNode* root = buildTestTree2();
        vector<int> del = {3};
        auto result = sol.delNodes(root, del);
        assert(result.size() == 1);
        assert(result[0]->val == 1);
    }

    // 测试 BFS 解法
    {
        Solution_BFS sol;
        TreeNode* root = buildTestTree1();
        vector<int> del = {3, 5};
        auto result = sol.delNodes(root, del);
        assert(result.size() == 3);
    }

    // 测试后序解法
    {
        Solution_PostOrder sol;
        TreeNode* root = buildTestTree1();
        vector<int> del = {3, 5};
        auto result = sol.delNodes(root, del);
        assert(result.size() == 3);
    }

    cout << "All tests passed!" << endl;
}

// int main() { test(); return 0; }

// ============================================================
// 总结
// ============================================================
/*
 * 【解法对比】
 *
 * | 解法                 | 时间  | 空间  | 特点                         |
 * |----------------------|-------|-------|------------------------------|
 * | DFS+isRoot(推荐)     | O(n)  | O(n)  | 最简洁，一次遍历搞定         |
 * | BFS层序遍历          | O(n)  | O(n)  | 不用递归，适合极深树         |
 * | DFS后序(无isRoot)    | O(n)  | O(n)  | 需单独处理根节点             |
 *
 * 【易错点】
 * 1. 忘记将 to_delete 转为 HashSet — 用 vector 查找是 O(m)，
 *    总时间退化为 O(n*m)
 * 2. 删除节点后忘记断开父子连接 — 必须用返回值 nullptr 让父节点
 *    的 left/right 置空，否则删除节点仍被引用
 * 3. 根节点的特殊处理 — 解法1用 isRoot=true 启动；解法3必须在
 *    递归结束后手动检查根是否被删除
 * 4. 先收集再递归 vs 先递归再收集 — 解法1(先序)先判断再递归，
 *    解法3(后序)先递归再判断，两者都正确但逻辑不同
 * 5. BFS 中删除节点的子节点必须先入队再断开 — 顺序颠倒会丢失子树
 *
 * 【面试追问】
 * Q1: 如果节点值不唯一（可能重复），方案需要怎么改？
 *     → 题目保证节点值唯一(1<=val<=1000, n<=1000)。若值重复，
 *       to_delete 需改为删除特定节点指针(而非值)，用
 *       unordered_set<TreeNode*> 替代 unordered_set<int>。
 *
 * Q2: 如果要求返回结果按特定顺序(如原树的层序)，怎么处理？
 *     → 当前解法收集顺序取决于遍历顺序。可以对 result 按根节点
 *       在原树中的深度排序，或在 BFS 解法中用层序天然保证。
 *
 * Q3: 如果删除操作是动态的（边删边加新节点），如何设计数据结构？
 *     → 可以用"带父指针的树+lazy删除标记"维护森林。每次删除
 *       O(子节点数)更新。若需频繁查询"某节点属于哪棵树"，
 *       可用并查集维护连通分量。
 */
