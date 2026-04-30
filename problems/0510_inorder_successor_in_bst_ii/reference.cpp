/*
 * 【题目本质】
 * 给定 BST 中的一个节点（有 parent 指针，无 root），找到其中序后继。
 * 核心是分两种情况：有右子树 vs 无右子树。
 *
 * 【解法总览】
 * 解法一：分情况讨论（推荐）       O(h) / O(1)
 * 解法二：利用值比较（非 Follow-up）O(h) / O(1)
 */

// ===================== 解法一：分情况讨论（推荐） =====================
// 思路：
//   1. 有右子树 → 后继 = 右子树的最左节点
//   2. 无右子树 → 沿 parent 向上走，直到当前节点是某祖先的左孩子
class Solution1 {
public:
    Node* inorderSuccessor(Node* node) {
        // 情况1：有右子树，找右子树最左节点
        if (node->right) {
            Node* cur = node->right;
            while (cur->left) cur = cur->left;
            return cur;
        }

        // 情况2：无右子树，沿 parent 向上
        // 跳过所有"当前节点是右孩子"的祖先
        while (node->parent && node == node->parent->right) {
            node = node->parent;
        }
        // node->parent 就是后继（可能为 null）
        return node->parent;
    }
};

// ===================== 解法二：利用值比较 =====================
// 思路：先找到 root，再从 root 出发利用 BST 性质查找
// 注意：这不满足 Follow-up 的要求
class Solution2 {
public:
    Node* inorderSuccessor(Node* node) {
        // 先找 root
        Node* root = node;
        while (root->parent) root = root->parent;

        Node* successor = nullptr;
        int target = node->val;

        // 从 root 开始搜索
        while (root) {
            if (root->val > target) {
                successor = root;   // 当前节点值更大，记录为候选
                root = root->left;  // 尝试找更小的
            } else {
                root = root->right; // 当前节点值 <= target，右走
            }
        }
        return successor;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优点 | 缺点 |
 * |------|------|------|------|------|
 * | 分情况讨论 | O(h) | O(1) | 不依赖值，满足 Follow-up | 需理解中序性质 |
 * | 值比较     | O(h) | O(1) | 思路直观 | 不满足 Follow-up，需先找 root |
 *
 * 【易错点】
 * 1. 向上走的循环条件：跳过的是"右孩子"关系，不是"左孩子"
 * 2. 最大节点没有后继，返回 null（循环自然处理）
 * 3. 区分 LC 285（给 root + val）和 LC 510（给 node + parent）
 *
 * 【面试追问】
 * Q1: 如何找中序前驱？→ 对称：有左子树取最右；无左子树向上找右孩子祖先
 * Q2: 如果只给 root 不给 parent？→ LC 285，从 root 出发利用 BST 性质
 * Q3: 时间复杂度最坏？→ O(n)（退化为链表），平衡时 O(log n)
 */
