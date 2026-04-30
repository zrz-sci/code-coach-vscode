/*
 * LeetCode 1650: 二叉树的最近公共祖先 III (Lowest Common Ancestor of a Binary Tree III)
 *
 * 【题目本质】
 * 每个节点有 parent 指针，从 p 和 q 沿 parent 上走形成两条"链表"，
 * 求这两条链表的交点（即 LCA）。本质就是 LeetCode 160 相交链表。
 *
 * 【解法总览】
 * 解法1: 哈希集合         — O(h) / O(h) — 最直觉
 * 解法2: 双指针(链表交点) — O(h) / O(1) — 面试首选 ⭐
 * 解法3: 先求深度再对齐   — O(h) / O(1) — 逻辑清晰的替代方案
 */

/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* parent;
};
*/

// ============================================================
// 解法1: 哈希集合 — 存 p 的祖先，q 往上找第一个匹配
// 时间: O(h)  空间: O(h)
//
// 【思路】
// 最直觉的做法：把 p 到 root 路径上的所有节点存到集合中，
// 然后 q 沿 parent 上走，第一个出现在集合里的节点就是 LCA。
//
// 为什么正确？LCA 是 p 和 q 的最深公共祖先。
// p 路径上的节点从深到浅排列在集合中，q 从深到浅往上找，
// 第一个匹配的一定是最深的公共祖先。
//
//       3          p=5 的祖先集合: {5, 3}
//      / \         q=4 往上走: 4→2→5 ← 5在集合中! 返回5
//     5   1
//    / \
//   6   2
//      / \
//     7   4
// ============================================================
class Solution1 {
public:
    Node* lowestCommonAncestor(Node* p, Node* q) {
        unordered_set<Node*> ancestors;

        // 把 p 到 root 的整条路径存入集合
        Node* curr = p;
        while (curr) {
            ancestors.insert(curr);
            curr = curr->parent;
        }

        // q 往上走，第一个在集合中的节点就是 LCA
        curr = q;
        while (curr) {
            if (ancestors.count(curr)) return curr;
            curr = curr->parent;
        }

        return nullptr; // 题目保证 p、q 都在树中，不会走到这里
    }
};

// ============================================================
// 解法2: 双指针 (链表交点法) — O(h) / O(1) ⭐ 面试首选
// 时间: O(h)  空间: O(1)
//
// 【思路】
// 关键洞察：从 p 沿 parent 走到 null，就像一条单链表；
// 从 q 沿 parent 走到 null，是另一条单链表。
// LCA 就是两条链表的交点！
//
// 这就是 LeetCode 160（相交链表）的经典双指针技巧：
// - 指针 a 从 p 出发沿 parent 走，走到 null 后跳到 q 继续走
// - 指针 b 从 q 出发沿 parent 走，走到 null 后跳到 p 继续走
// - 两者最终在 LCA 处相遇
//
// 为什么能 work？
// 设 p 到 LCA 的距离为 dP，q 到 LCA 的距离为 dQ，
// LCA 到 root(再到null) 的距离为 C。
// 
// 指针 a 总路程: dP + C + dQ (先走p路径，再走q路径到LCA)
// 指针 b 总路程: dQ + C + dP (先走q路径，再走p路径到LCA)
// 两者相等! 所以一定同时到达 LCA。
//
// 示例: p=5, q=4
//   p的路径: 5 → 3 → null             (dP=1, C=2)
//   q的路径: 4 → 2 → 5 → 3 → null     (dQ=3, C=2)
//
//   a: 5 → 3 → null → 跳到q → 4 → 2 → [5]  步数=5
//   b: 4 → 2 → 5 → 3 → null → 跳到p → [5]  步数=5
//   在节点5相遇! LCA=5 ✓
// ============================================================
class Solution2 {
public:
    Node* lowestCommonAncestor(Node* p, Node* q) {
        Node* a = p;
        Node* b = q;

        while (a != b) {
            // a 到达 null(root的parent) 后跳到 q 的起点
            // 否则继续沿 parent 上走
            a = (a != nullptr) ? a->parent : q;

            // b 到达 null 后跳到 p 的起点
            b = (b != nullptr) ? b->parent : p;
        }

        // a == b 时就是 LCA
        return a;
    }
};

// ============================================================
// 解法3: 先求深度再对齐 — O(h) / O(1)
// 时间: O(h)  空间: O(1)
//
// 【思路】
// 和链表题"先量长度再对齐"的思路一致：
// 1. 分别求 p 和 q 的深度（沿 parent 到 root 的距离）
// 2. 让更深的节点先往上走 |depthP - depthQ| 步，对齐到同一层
// 3. 两指针同步上走，相遇处就是 LCA
//
// 这个解法比解法2更好理解，但代码稍长（3遍遍历 vs 1个循环）。
//
// 示例: p=4(depth=3), q=1(depth=1)
//       3           depth=0
//      / \
//     5   1         depth=1  ← q 在这层
//    / \
//   6   2           depth=2
//      / \
//     7   4         depth=3  ← p 在这层
//
// Step1: depthP=3, depthQ=1, diff=2
// Step2: p 先走2步: 4→2→5, 现在 p=5, q=1, 都在 depth=1
// Step3: 同步走: p:5→3, q:1→3, 相遇在3! LCA=3 ✓
// ============================================================
class Solution3 {
public:
    Node* lowestCommonAncestor(Node* p, Node* q) {
        // 第一步：求深度
        int depthP = getDepth(p);
        int depthQ = getDepth(q);

        // 第二步：对齐 — 让深的先走
        while (depthP > depthQ) {
            p = p->parent;
            depthP--;  // 易错：忘记更新深度导致死循环
        }
        while (depthQ > depthP) {
            q = q->parent;
            depthQ--;
        }

        // 第三步：同步上走，相遇即 LCA
        while (p != q) {
            p = p->parent;
            q = q->parent;
        }
        return p;
    }

private:
    int getDepth(Node* node) {
        int depth = 0;
        while (node) {
            node = node->parent;
            depth++;
        }
        return depth;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间 | 空间 | 特点                        |
// |-------------|------|------|-----------------------------|
// | 哈希集合    | O(h) | O(h) | 最直觉，代码简单            |
// | 双指针      | O(h) | O(1) | 代码最短，面试首选          |
// | 深度对齐    | O(h) | O(1) | 逻辑更直观，代码稍长        |
//
// 解法2 vs 解法3: 
//   本质相同——都是让两指针走相同的总路程来保证在交点相遇。
//   解法2 把"对齐"隐含在跳转逻辑中，解法3 显式地先对齐再同步。
//   面试中建议先说解法1展示思路，再给解法2展示优化能力。
//
// 与 LeetCode 236 的区别:
//   236 没有 parent 指针，只给 root，必须用自顶向下递归 DFS。
//   本题有 parent 指针，可以从下往上走，转化为链表交点问题。
//
// ============================================================
// 【易错点】
//
// 1. 双指针跳转条件写错:
//    ✗ a = (a->parent) ? a->parent : q;
//      → 当 a 是 root 时 a->parent == nullptr，a 跳到 q，看似对
//      → 但当 a 已经是 nullptr 时会空指针解引用!
//    ✓ a = (a != nullptr) ? a->parent : q;
//      → 先判断 a 本身是否为 nullptr
//
// 2. 跳转目标搞反:
//    ✗ a 走完 p 路径后跳到 p（又走回自己的路径）
//    ✓ a 从 p 出发，走完后跳到 q
//    ✓ b 从 q 出发，走完后跳到 p
//    → 交叉跳转才能保证总路程相等
//
// 3. 解法3 忘记更新深度计数器:
//    ✗ while (depthP > depthQ) { p = p->parent; }  // 死循环!
//    ✓ while (depthP > depthQ) { p = p->parent; depthP--; }
//
// ============================================================
// 【面试追问】
//
// Q1: 如果没有 parent 指针，只给了 root，怎么找 LCA？
// A1: 这就是 LeetCode 236。用后序遍历递归：
//     如果当前节点是 p 或 q 返回它，否则看左右子树的返回值。
//     左右都非空说明当前节点是 LCA，只有一边非空就返回那边。
//
// Q2: 如果 p 或 q 可能不在树中呢？(LeetCode 1644)
// A2: 哈希集合法天然支持：如果 q 走到 root 都不在集合中，返回 null。
//     双指针法需要额外处理：两指针都跳转过之后如果同时到 nullptr，
//     说明不存在交点。或者改用标记法验证两个节点是否都被访问过。
//
// Q3: 如果要频繁查询大量不同的 (p,q) 对，如何预处理？
// A3: 使用 Binary Lifting（倍增法）预处理:
//     O(n log n) 时间预处理，之后每次查询只需 O(log n)。
//     核心思想：预计算每个节点的第 2^k 个祖先，
//     查询时用二进制拆分快速跳到同一层，再同步跳找 LCA。
// ============================================================
