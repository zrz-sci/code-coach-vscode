/*
 * LeetCode 236: 二叉树的最近公共祖先 (Lowest Common Ancestor of a Binary Tree)
 *
 * 【题目本质】
 * 在一棵二叉树中，找到两个节点 p 和 q 的最近公共祖先。
 * 本质是：自底向上，确定 p 和 q 分别在当前节点的哪一侧。
 *
 * 【解法总览】
 * 解法1: 记录路径 + 比较       — O(n) / O(n) — 最直觉
 * 解法2: 递归后序遍历（⭐面试首选）— O(n) / O(h) — 最优雅
 * 解法3: 迭代（父指针 + 祖先集合）— O(n) / O(n) — 不用递归的替代方案
 */


// ============================================================
// 解法1: 记录路径 + 比较
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 最直觉的方法：分别找到从根到 p、从根到 q 的完整路径，
// 然后从头对比两条路径，最后一个相同的节点就是 LCA。
//
// 这就像两个人从同一个起点出发走不同的路，
// 他们"分道扬镳"之前的最后一个路口就是答案。
//
//         3
//        / \
//       5   1
//      / \ / \
//     6  2 0  8
//       / \
//      7   4
//
// 路径到 p=5: [3, 5]
// 路径到 q=4: [3, 5, 2, 4]
// 比较: 3==3 ✓, 5==5 ✓, 然后路径长度不同/值不同 → LCA = 5
// ============================================================
class Solution1 {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        vector<TreeNode*> pathP, pathQ;
        findPath(root, p, pathP);
        findPath(root, q, pathQ);
        
        TreeNode* ans = root;
        for (int i = 0; i < (int)min(pathP.size(), pathQ.size()); i++) {
            if (pathP[i] == pathQ[i]) {
                ans = pathP[i];  // 持续更新，最后一个相同的就是 LCA
            } else {
                break;           // 分叉了，前一个就是答案
            }
        }
        return ans;
    }
    
private:
    // 回溯法找从 root 到 target 的路径
    // 为什么用回溯？因为走错路需要撤销选择
    bool findPath(TreeNode* root, TreeNode* target, vector<TreeNode*>& path) {
        if (!root) return false;
        
        path.push_back(root);          // 做选择：把当前节点加入路径
        
        if (root == target) return true; // 找到了，路径完成
        
        // 在左右子树中继续找
        if (findPath(root->left, target, path) || 
            findPath(root->right, target, path)) {
            return true;
        }
        
        path.pop_back();               // 撤销选择：此路不通，回退
        return false;
    }
};

// ============================================================
// 解法2: 递归后序遍历 ⭐ 面试首选
// 时间: O(n)  空间: O(h)，h 为树高，最坏 O(n)
//
// 【思路】
// 解法1 需要两次遍历 + 路径存储，能否一次遍历搞定？
//
// 关键洞察：用后序遍历（先处理左右子树，再处理当前节点），
// 让信息"自底向上"汇报。每个节点问自己：
//   "p 和 q 在我的左子树还是右子树？"
//
// 三种情况：
// (1) 左右子树各找到一个 → p、q 分居两侧 → 我就是 LCA
// (2) 只有左子树找到 → LCA 在我的左子树中
// (3) 只有右子树找到 → LCA 在我的右子树中
//
// 递归返回值的含义（多义的，这是精妙之处）：
// - null: 这棵子树中没有 p 也没有 q
// - 非null: 找到了 p、q、或它们的 LCA
//
//  示例1: p=5, q=1
//
//           3             ← left=5, right=1, 都非空 → 返回3 (LCA!)
//          / \
//   返回5← 5   1 →返回1
//        / \ / \
//       6  2 0  8         ← 这些都返回 null
//         / \
//        7   4
//
//  示例2: p=5, q=4
//
//           3             ← left=5, right=null → 返回5 (LCA!)
//          / \
//   返回5← 5   1 →返回null
//        / \ / \
//       6  2 0  8
//  null↑ / \
//       7   4             ← 节点5: root==p → 直接返回5
//           ↑             (不继续往下搜索，因为即使q在下面，p就是LCA)
// ============================================================
class Solution2 {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // 终止条件1: 空节点 → 没找到
        // 终止条件2: 找到了 p 或 q → 直接返回（不再深入）
        // 为什么找到 p 就直接返回？因为：
        //   - 如果 q 在 p 的子树中，p 本身就是 LCA
        //   - 如果 q 不在 p 的子树中，另一个分支会找到 q
        if (!root || root == p || root == q) return root;
        
        // 后序遍历：先递归左右子树，收集信息
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        
        // 汇总阶段：根据左右返回值判断
        if (left && right) return root;  // p、q 分居两侧 → 当前节点是 LCA
        
        return left ? left : right;      // 只有一侧找到 → LCA 在那一侧
    }
};

// ============================================================
// 解法3: 迭代（父指针 + 祖先集合）
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 面试官追问"不用递归能不能做？"时的方案。
//
// 思路很像"两条链表找交点"（LeetCode 160）：
// Step1: 用迭代 DFS 遍历树，建立每个节点的"父指针"映射
// Step2: 从 p 沿父指针一路走到根，收集 p 的所有祖先
// Step3: 从 q 沿父指针往上走，第一个出现在 p 祖先集合中的就是 LCA
//
//  用示例树说明 (p=5, q=4):
//
//         3              父指针表:
//        / \             5→3, 1→3, 6→5, 2→5,
//       5   1            0→1, 8→1, 7→2, 4→2
//      / \ / \
//     6  2 0  8          p=5 的祖先链: 5 → 3 → null
//       / \              祖先集合: {5, 3}
//      7   4
//                        q=4 往上走: 4 → 2 → 5 ← 在集合中！→ LCA=5
// ============================================================
class Solution3 {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // Step1: 迭代 DFS，建立父指针映射
        unordered_map<TreeNode*, TreeNode*> parent;
        parent[root] = nullptr;  // 根节点没有父亲
        
        stack<TreeNode*> stk;
        stk.push(root);
        
        // 遍历直到 p 和 q 的父指针都已记录
        // 为什么用 ||？因为必须两个都找到才能停
        while (!parent.count(p) || !parent.count(q)) {
            TreeNode* node = stk.top(); stk.pop();
            if (node->left) {
                parent[node->left] = node;   // 记录左孩子的父亲
                stk.push(node->left);
            }
            if (node->right) {
                parent[node->right] = node;  // 记录右孩子的父亲
                stk.push(node->right);
            }
        }
        
        // Step2: 收集 p 的所有祖先（包括 p 自己）
        unordered_set<TreeNode*> ancestors;
        TreeNode* cur = p;
        while (cur) {
            ancestors.insert(cur);
            cur = parent[cur];  // 沿父指针往上走
        }
        
        // Step3: 从 q 往上走，第一个在 p 祖先集合中的就是 LCA
        cur = q;
        while (!ancestors.count(cur)) {
            cur = parent[cur];
        }
        return cur;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度     | 解法1: 记录路径  | 解法2: 递归后序 ⭐ | 解法3: 迭代父指针 |
// |----------|-----------------|-------------------|------------------|
// | 时间     | O(n)            | O(n)              | O(n)             |
// | 空间     | O(n) 路径       | O(h) 递归栈       | O(n) 哈希表      |
// | 代码量   | 中等            | 极少（6行核心）    | 较多             |
// | 核心思想 | 找路径→比较      | 后序汇报→交汇     | 建父指针→回溯     |
// | 面试推荐 | 说思路即可       | ⭐ 首选           | 追问不用递归时写   |
//
// 解法2 是面试标准答案：代码极简，体现对递归和后序遍历的深刻理解。
// 解法3 在面试官要求"不用递归"时是最佳替代方案。
// 解法1 适合用来解释思路，实际编码不如解法2简洁。
//
// ============================================================
// 【易错点】
//
// 1. 遗漏 root==p || root==q 的终止条件
//    ✗ if (!root) return nullptr;  // 只判断了空
//    ✓ if (!root || root == p || root == q) return root;
//    漏掉后永远找不到 p 和 q，所有节点都返回 null。
//
// 2. 误以为找到 p 后还需要继续搜索 p 的子树
//    ✗ 想法："root==p 时不能返回，q 可能在 p 下面"
//    ✓ 正确理解：如果 q 在 p 下面，p 本身就是 LCA，返回 p 正确。
//    如果 q 不在 p 下面，另一个分支的递归会找到 q。
//
// 3. 解法3 的 while 条件用 && 而不是 ||
//    ✗ while (!parent.count(p) && !parent.count(q))
//    → 只要找到一个就停了，另一个可能还没录入父指针
//    ✓ while (!parent.count(p) || !parent.count(q))
//    → 必须两个都找到才停
//
// 4. 递归返回值理解错误
//    ✗ 以为返回值始终代表 LCA
//    ✓ 返回值是"多义的"：可能是 p、q、LCA 或 null
//    只有在最外层调用者那里，返回值才一定是 LCA
//
// ============================================================
// 【面试追问】
//
// Q1: 递归函数的返回值到底代表什么？(基础理解)
//     → 在以 root 为根的子树中找到的目标节点或 LCA。
//       null 表示没找到任何目标，非 null 是 p/q/LCA 之一。
//       调用者通过组合左右返回值来判断最终 LCA。
//
// Q2: 如果 p 或 q 可能不在树中怎么办？(变体)
//     → 不能在遇到 p/q 时立即返回。需要完整遍历，
//       用两个 bool 标记是否真正找到了 p 和 q。
//       只有两个都找到，LCA 才有效，否则返回 null。
//       这就是 LeetCode 1644 的做法。
//
// Q3: 如果是 BST 呢？(优化)
//     → 利用 BST 有序性：p,q 都 < root 去左边，
//       都 > root 去右边，一大一小则 root 就是 LCA。
//       时间 O(h) 而非 O(n)。这就是 LeetCode 235。
//
// Q4: 如果有大量查询 (多对 p,q) 怎么优化？(进阶)
//     → Euler Tour + Sparse Table: O(n) 预处理，O(1) 查询。
//       或 Tarjan 离线 LCA 算法（基于并查集）。
// ============================================================
