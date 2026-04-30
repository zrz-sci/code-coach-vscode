// 【题目本质】
// 对两棵四叉树做按位 OR 运算，返回结果四叉树。
// 核心：递归合并 + 叶节点剪枝 + 合并后压缩。

// 【解法总览】
// Solution1: 递归合并（推荐）— O(n) 时间，O(log n) 栈空间 ⭐
// Solution2: 还原矩阵暴力法 — O(n^2) 时间空间，不推荐

// ===================== Solution1: 递归合并 =====================
// 思路：
//   - 叶节点 val=true OR 任何 = true叶节点（剪枝）
//   - 叶节点 val=false OR x = x（返回对方）
//   - 非叶 OR 非叶 → 递归四个象限，尝试压缩
class Solution1 {
public:
    Node* intersect(Node* t1, Node* t2) {
        // 叶节点剪枝
        if (t1->isLeaf) return t1->val ? t1 : t2;
        if (t2->isLeaf) return t2->val ? t2 : t1;

        // 递归合并四个象限
        Node* tl = intersect(t1->topLeft, t2->topLeft);
        Node* tr = intersect(t1->topRight, t2->topRight);
        Node* bl = intersect(t1->bottomLeft, t2->bottomLeft);
        Node* br = intersect(t1->bottomRight, t2->bottomRight);

        // 压缩：四个子节点都是相同值叶节点 → 合并为一个叶节点
        if (tl->isLeaf && tr->isLeaf && bl->isLeaf && br->isLeaf &&
            tl->val == tr->val && tr->val == bl->val && bl->val == br->val) {
            return new Node(tl->val, true);
        }
        return new Node(false, false, tl, tr, bl, br);
    }
};

// ===================== Solution2: 还原矩阵暴力法 =====================
// 思路：将两棵树展开为矩阵 → 逐元素 OR → 重建四叉树
// 时间 O(n^2)，空间 O(n^2)，面试不推荐
class Solution2 {
public:
    Node* intersect(Node* t1, Node* t2) {
        // 省略：需要 decode + OR + rebuild，代码冗长且效率低
        return nullptr;
    }
};

// 【解法对比】
// | 解法 | 时间 | 空间 | 优势 |
// |------|------|------|------|
// | Solution1 递归合并 | O(n) | O(log n) | 直接树操作，高效 |
// | Solution2 暴力还原 | O(n^2) | O(n^2) | 简单但低效 |

// 【易错点】
// 1. 叶节点 false OR 非叶节点 → 返回非叶节点（不是返回 false）
// 2. 压缩条件必须四个子节点都是叶节点且值相同
// 3. 内部节点的 val 可以设为任意值（题目允许），isLeaf 必须为 false

// 【面试追问】
// Q1: 改为 AND？→ false叶节点 AND 任何 = false；true叶节点 AND x = x
// Q2: 时间复杂度？→ O(min(n1, n2))，剪枝使很多子树不需要递归
// Q3: 内存管理？→ 实际项目用 shared_ptr，LeetCode 不需要
