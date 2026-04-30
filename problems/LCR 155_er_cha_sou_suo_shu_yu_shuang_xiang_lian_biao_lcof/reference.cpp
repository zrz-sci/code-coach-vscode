/*
 * LCR 155: 将二叉搜索树转化为排序的双向链表
 * (同主站 426. Convert BST to Sorted Doubly Linked List)
 *
 * 【题目本质】
 * 将 BST 就地转为有序的双向循环链表。
 * left 指针作前驱 (prev)，right 指针作后继 (next)。
 * 返回链表头（最小节点）。
 *
 * 【核心观察】
 * BST 中序遍历 = 有序序列。在遍历过程中修改指针即可原地完成转换。
 *
 * 【解法总览】
 * 解法1: 递归中序遍历   — O(n) / O(h) — DFS + 原地修改 ⭐面试首选
 * 解法2: 迭代中序遍历   — O(n) / O(h) — 显式栈模拟递归
 * 解法3: Morris遍历     — O(n) / O(1) — 无需栈，线索化遍历
 */



// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;

    Node() : val(0), left(nullptr), right(nullptr) {}
    Node(int _val) : val(_val), left(nullptr), right(nullptr) {}
    Node(int _val, Node* _left, Node* _right) : val(_val), left(_left), right(_right) {}
};

// ============================================================
// 解法1: 递归中序遍历 — DFS + 原地修改指针
// 时间: O(n)  空间: O(h) 递归栈深度, h=树高
//
// 【思路】
// 中序遍历 BST 得到递增序列。在"访问节点"这一步修改指针：
//   cur->left = prev  (当前节点的前驱)
//   prev->right = cur (上一节点的后继)
//
// 维护两个变量:
//   head: 链表头节点（中序第一个，即BST最左叶子）
//   prev: 上一个访问的节点
//
// 转换过程示意 (BST: [4,2,5,1,3]):
//
//   原始 BST:         中序遍历: 1->2->3->4->5
//         4
//        / \           步骤1: 访问1, prev=null -> head=1
//       2   5          步骤2: 访问2, 1<->2
//      / \             步骤3: 访问3, 2<->3
//     1   3            步骤4: 访问4, 3<->4
//                      步骤5: 访问5, 4<->5
//   转换后:            步骤6: 首尾相连 1<->5 (循环)
//
//   +------------------------------------------+
//   |                                          |
//   +->[1] <-> [2] <-> [3] <-> [4] <-> [5]<---+
//      head                             tail(prev)
//
// ============================================================
class Solution1 {
public:
    Node* head = nullptr;  // 链表头（最小值节点）
    Node* prev = nullptr;  // 中序遍历中上一个访问的节点

    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;

        // 中序遍历，在过程中修改指针
        dfs(root);

        // 遍历结束后，首尾相连形成循环链表
        // head = 最小值节点, prev = 最大值节点
        head->left = prev;
        prev->right = head;

        return head;
    }

private:
    void dfs(Node* cur) {
        if (!cur) return;

        // 1. 遍历左子树
        dfs(cur->left);

        // 2. 访问当前节点: 修改指针
        if (prev) {
            // prev 存在：双向链接 prev 和 cur
            prev->right = cur;  // prev 的后继 = cur
            cur->left = prev;   // cur 的前驱 = prev
        } else {
            // prev 为空：cur 是中序第一个节点（最小值）
            // 记录为链表头
            head = cur;
        }
        // 更新 prev 为当前节点
        prev = cur;

        // 3. 遍历右子树
        dfs(cur->right);
    }
};

// ============================================================
// 解法1b: 递归中序遍历 (用引用参数代替成员变量)
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 与解法1完全相同，但 head 和 prev 通过引用参数传递，
// 避免使用成员变量，更符合函数式编程风格。
// 面试中两种写法都可以，看个人习惯。
// ============================================================
class Solution1b {
public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;

        Node* head = nullptr;
        Node* prev = nullptr;

        dfs(root, head, prev);

        // 首尾相连
        head->left = prev;
        prev->right = head;

        return head;
    }

private:
    void dfs(Node* cur, Node*& head, Node*& prev) {
        if (!cur) return;

        dfs(cur->left, head, prev);

        if (prev) {
            prev->right = cur;
            cur->left = prev;
        } else {
            head = cur;
        }
        prev = cur;

        dfs(cur->right, head, prev);
    }
};

// ============================================================
// 解法2: 迭代中序遍历 — 用显式栈模拟递归
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 经典的迭代中序遍历模板：
//   1. 当前节点不断向左走并压栈
//   2. 弹栈 = 访问节点（修改指针）
//   3. 转向右子树
//
// 模拟 [4,2,5,1,3]:
//
//   初始: cur=4, stack=[]
//
//   向左: cur=4->2->1->null, stack=[4,2,1]
//   弹出 1: 处理1, head=1, prev=1
//     cur=1.right=null
//   弹出 2: 处理2, 1<->2, prev=2
//     cur=2.right=3
//   向左: cur=3->null, stack=[4,3]
//   弹出 3: 处理3, 2<->3, prev=3
//     cur=3.right=null
//   弹出 4: 处理4, 3<->4, prev=4
//     cur=4.right=5
//   向左: cur=5->null, stack=[5]
//   弹出 5: 处理5, 4<->5, prev=5
//     cur=5.right=null
//   栈空 -> 结束
//
//   首尾相连: 1<->5
// ============================================================
class Solution2 {
public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;

        Node* head = nullptr;
        Node* prev = nullptr;

        stack<Node*> stk;
        Node* cur = root;

        while (cur || !stk.empty()) {
            // 一路向左，全部压栈
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }

            // 弹出栈顶 = 中序访问
            cur = stk.top();
            stk.pop();

            // 处理当前节点：修改指针
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

        // 首尾相连
        head->left = prev;
        prev->right = head;

        return head;
    }
};

// ============================================================
// 解法3: Morris 中序遍历 — O(1) 空间
// 时间: O(n)  空间: O(1)
//
// 【思路】
// Morris 遍历通过"线索化"实现无栈的中序遍历。
// 利用叶子节点的空 right 指针临时指向中序后继，
// 省去了栈或递归。
//
// Morris 规则:
//   if (cur->left == null):
//     访问 cur, 移到 cur->right
//   else:
//     找到 cur 左子树的最右节点 (predecessor)
//     if (predecessor->right == null):
//       建立线索: predecessor->right = cur
//       进入左子树: cur = cur->left
//     if (predecessor->right == cur):
//       左子树遍历完毕, 访问 cur
//       移到 cur->right
//
// 注意: 本题就是要修改指针，所以 Morris 遍历在"访问节点"时
// 的修改与前面的解法相同。面试中通常不要求这种写法。
// ============================================================
class Solution3 {
public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;

        Node* head = nullptr;
        Node* prev = nullptr;
        Node* cur = root;

        while (cur) {
            if (!cur->left) {
                // 无左子树 -> 直接处理当前节点
                if (prev) {
                    prev->right = cur;
                    cur->left = prev;
                } else {
                    head = cur;
                }
                prev = cur;
                cur = cur->right;
            } else {
                // 找左子树的最右节点 (中序前驱)
                Node* predecessor = cur->left;
                while (predecessor->right && predecessor->right != cur) {
                    predecessor = predecessor->right;
                }

                if (!predecessor->right) {
                    // 线索不存在 -> 建立线索，进入左子树
                    predecessor->right = cur;
                    cur = cur->left;
                } else {
                    // 线索已存在 -> 左子树遍历完毕，处理当前节点
                    if (prev) {
                        prev->right = cur;
                        cur->left = prev;
                    } else {
                        head = cur;
                    }
                    prev = cur;
                    cur = cur->right;
                }
            }
        }

        // 首尾相连
        head->left = prev;
        prev->right = head;

        return head;
    }
};

// ============================================================
// 辅助函数：构建 BST 和验证结果
// ============================================================

// 从数组构建 BST (层序)
Node* buildBST(const vector<int>& vals, int idx = 0) {
    if (idx >= (int)vals.size() || vals[idx] == -1) return nullptr;
    Node* node = new Node(vals[idx]);
    node->left = buildBST(vals, 2 * idx + 1);
    node->right = buildBST(vals, 2 * idx + 2);
    return node;
}

// 验证双向循环链表
void verifyList(Node* head, const vector<int>& expected) {
    if (!head && expected.empty()) {
        cout << "PASS (empty)" << endl;
        return;
    }
    if (!head) {
        cout << "FAIL: head is null but expected non-empty" << endl;
        return;
    }

    // 正向遍历
    vector<int> forward;
    Node* cur = head;
    for (int i = 0; i < (int)expected.size(); i++) {
        forward.push_back(cur->val);
        cur = cur->right;
    }

    // 检查循环 (cur 应该回到 head)
    bool cyclic = (cur == head);

    bool forwardMatch = (forward == expected);

    if (forwardMatch && cyclic) {
        cout << "PASS: [";
        for (int i = 0; i < (int)forward.size(); i++) {
            if (i) cout << ",";
            cout << forward[i];
        }
        cout << "]" << endl;
    } else {
        cout << "FAIL: match=" << forwardMatch << " cyclic=" << cyclic << endl;
    }
}

int main() {
    cout << "=== 解法1: 递归中序遍历 ===" << endl;
    {
        Solution1 sol;
        Node* root = buildBST({4, 2, 5, 1, 3});
        Node* head = sol.treeToDoublyList(root);
        verifyList(head, {1, 2, 3, 4, 5});
    }
    {
        Solution1 sol;
        Node* root = buildBST({2, 1, 3});
        Node* head = sol.treeToDoublyList(root);
        verifyList(head, {1, 2, 3});
    }
    {
        Solution1 sol;
        Node* head = sol.treeToDoublyList(nullptr);
        verifyList(head, {});
    }
    {
        Solution1 sol;
        Node* root = new Node(1);
        Node* head = sol.treeToDoublyList(root);
        verifyList(head, {1});
    }

    cout << "\n=== 解法2: 迭代中序遍历 ===" << endl;
    {
        Solution2 sol;
        Node* root = buildBST({4, 2, 5, 1, 3});
        Node* head = sol.treeToDoublyList(root);
        verifyList(head, {1, 2, 3, 4, 5});
    }

    cout << "\n=== 解法3: Morris遍历 ===" << endl;
    {
        Solution3 sol;
        Node* root = buildBST({4, 2, 5, 1, 3});
        Node* head = sol.treeToDoublyList(root);
        verifyList(head, {1, 2, 3, 4, 5});
    }

    return 0;
}

/*
 * ============================================================
 * 总结
 * ============================================================
 *
 * 【解法对比】
 * ┌──────────────────────────┬────────┬────────┬──────────────────────────┐
 * │ 解法                     │ 时间   │ 空间   │ 适用场景                 │
 * ├──────────────────────────┼────────┼────────┼──────────────────────────┤
 * │ 1. 递归中序遍历 ⭐        │ O(n)   │ O(h)   │ 面试首选，简洁直观       │
 * │ 1b. 递归(引用参数)        │ O(n)   │ O(h)   │ 无成员变量，函数式风格   │
 * │ 2. 迭代中序遍历           │ O(n)   │ O(h)   │ 面试加分，显式栈更可控   │
 * │ 3. Morris遍历            │ O(n)   │ O(1)   │ 空间极致优化，少见要求   │
 * └──────────────────────────┴────────┴────────┴──────────────────────────┘
 *   (h = 树高，平衡BST为 O(logn)，最坏退化链表为 O(n))
 *
 * 【易错点】
 * 1. 忘记首尾相连: 遍历结束后必须 head->left=prev; prev->right=head;
 *    形成循环链表，否则只是普通双向链表。
 * 2. 空树判断: root==nullptr 时直接返回 nullptr，不能走后续的首尾相连逻辑
 *    (head/prev 都是 nullptr，解引用会段错误)。
 * 3. 成员变量 vs 局部变量: 解法1用成员变量 head/prev，如果在同一个 Solution
 *    对象上多次调用会残留上次状态。面试时要说明或用解法1b的引用参数方式。
 * 4. cur->left = prev 会破坏左子树: 在递归中这没问题因为左子树已遍历完，
 *    但迭代解法中如果顺序错了 (比如在左子树遍历前修改了指针) 会死循环。
 * 5. Morris遍历中 predecessor->right == cur 判断线索时，不能与链表修改
 *    冲突——本题修改的是 cur->left 和 prev->right，和线索的 predecessor->right
 *    是不同节点的不同指针，不冲突。
 *
 * 【面试追问】
 * Q1: 如果不要求循环链表，只要普通双向链表，代码怎么改?
 *     → 去掉最后两行首尾相连即可。head->left 保持 nullptr，
 *       最后一个节点的 right 保持 nullptr。
 *
 * Q2: 如果要求不修改原树结构 (non-destructive)，怎么做?
 *     → 中序遍历收集节点值到数组，然后新建链表节点逐个连接。
 *       时间 O(n)，空间 O(n)。或者深拷贝树再转换。
 *
 * Q3: 如果输入不是 BST 而是普通二叉树，要求按中序遍历顺序转成双向链表?
 *     → 代码完全一样，中序遍历对任何二叉树都适用。
 *       只是结果不一定有序 (BST 保证有序，普通树不保证)。
 */
