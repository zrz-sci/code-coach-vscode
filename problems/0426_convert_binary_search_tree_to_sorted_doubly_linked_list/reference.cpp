// 【题目本质】
// BST 原地转换为有序的循环双向链表。left=前驱, right=后继, 首尾相连。
// 核心：中序遍历 BST 得到有序序列，遍历中用 prev 指针逐步链接，最后首尾成环。

// 【解法总览】
// Solution1: 递归中序遍历 O(n) / O(h) ⭐推荐 - 最简洁
// Solution2: 迭代中序遍历(栈) O(n) / O(h) - 显式栈，面试加分

// ===================== Solution1: 递归中序遍历 =====================
// 维护全局 prev 和 head，中序遍历时逐步链接
class Solution1 {
    Node* prev = nullptr;
    Node* head = nullptr;

    void inorder(Node* cur) {
        if (!cur) return;

        // 1. 先处理左子树（此时 cur->left 还未被修改）
        inorder(cur->left);

        // 2. 处理当前节点：与 prev 建立双向链接
        if (prev) {
            prev->right = cur;  // prev 的后继 = cur
            cur->left = prev;   // cur 的前驱 = prev
        } else {
            head = cur;  // 第一个节点（最小值）作为链表头
        }
        prev = cur;  // 更新 prev 为当前节点

        // 3. 处理右子树
        inorder(cur->right);
    }

public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;

        inorder(root);

        // 首尾相连形成循环
        head->left = prev;   // head 的前驱 = 最后一个节点
        prev->right = head;  // 最后一个节点的后继 = head

        return head;
    }
};

// ===================== Solution2: 迭代中序遍历 =====================
// 用显式栈模拟递归，逻辑完全等价
class Solution2 {
public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;

        stack<Node*> stk;
        Node* cur = root;
        Node* prev = nullptr;
        Node* head = nullptr;

        while (cur || !stk.empty()) {
            // 一路向左压栈
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }

            // 弹出栈顶（中序位置）
            cur = stk.top();
            stk.pop();

            // 链接
            if (prev) {
                prev->right = cur;
                cur->left = prev;
            } else {
                head = cur;
            }
            prev = cur;

            // 转向右子树
            cur = cur->right;
        }

        // 首尾成环
        head->left = prev;
        prev->right = head;

        return head;
    }
};

// 【解法对比】
// | 解法 | 时间 | 空间 | 特点 |
// |------|------|------|------|
// | Solution1 | O(n) | O(h) | 递归，代码最简洁 |
// | Solution2 | O(n) | O(h) | 迭代，无栈溢出风险 |
// h 为树高，平衡树 h=logn，最坏 h=n

// 【易错点】
// 1. 空树特判：root==nullptr 时直接返回 nullptr，不能执行后续首尾链接
// 2. 忘记首尾成环：题目要求循环链表，不是普通双向链表
// 3. 修改 left 不影响遍历：中序遍历中，先完成左子树递归再修改 cur->left，
//    此时左子树已遍历完毕，不会被影响
// 4. prev 和 head 必须是类成员变量（Solution1）或外层变量（Solution2），
//    不能是递归函数的局部变量

// 【面试追问】
// Q1: 为什么 BST 中序遍历一定是有序的？
// A1: BST 性质保证 left < root < right，中序(左-根-右)自然按升序访问。
//
// Q2: 如果树极度不平衡（退化为链表），递归会有问题吗？
// A2: 会有 O(n) 层递归，可能栈溢出。此时用迭代版本（Solution2）更安全。
//
// Q3: 能否用分治法（递归返回子链表的头尾）？
// A3: 可以。每次返回子树转换后的链表头和尾，在当前节点合并左右子链表。
//     实现稍复杂但逻辑也清晰，时间空间复杂度相同。
