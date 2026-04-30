/*
 * LeetCode 430: 扁平化多级双向链表 (Flatten a Multilevel Doubly Linked List)
 *
 * 【题目本质】
 * 多级双向链表本质上是一棵"多叉树"：child 是第一个孩子，next 是右兄弟。
 * 扁平化 = 按前序遍历顺序把这棵树串成一条双向链表。
 *
 * 【解法总览】
 * 解法1: 递归 DFS      — O(n) / O(d) — 最直觉，递归处理 child 后拼接
 * 解法2: 迭代 + 栈      — O(n) / O(d) — 用显式栈替代递归
 * 解法3: 迭代原地拼接   — O(n) / O(1) — 找子链表尾直接拼，无额外空间
 *
 * 多级链表结构示意（示例1）:
 *
 *  1 ↔ 2 ↔ 3 ↔ 4 ↔ 5 ↔ 6
 *           |
 *           7 ↔ 8 ↔ 9 ↔ 10
 *               |
 *               11 ↔ 12
 *
 *  扁平化后:
 *  1 ↔ 2 ↔ 3 ↔ 7 ↔ 8 ↔ 11 ↔ 12 ↔ 9 ↔ 10 ↔ 4 ↔ 5 ↔ 6
 *
 *  等价的多叉树视角（child=第一个孩子, next=右兄弟）:
 *         1
 *         |
 *    2 — 3 — 4 — 5 — 6
 *         |
 *    7 — 8 — 9 — 10
 *         |
 *   11 — 12
 *
 *  前序遍历: 1, 2, 3, 7, 8, 11, 12, 9, 10, 4, 5, 6 ✓
 */

// Definition for a Node.
class Node {
public:
    int val;
    Node* prev;
    Node* next;
    Node* child;
};

// ============================================================
// 解法1: 递归 DFS — 递归处理 child 链表，返回尾节点用于拼接
// 时间: O(n)  空间: O(d)，d 为最大嵌套深度（最坏 O(n)）
//
// 【思路】
// 多级链表天然是递归结构：child 链表内部还可能有 child。
// 设计一个辅助函数 flattenDFS(head)，功能是：
//   1. 将以 head 为头的多级链表扁平化
//   2. 返回扁平化后的尾节点（因为上层需要它来做拼接）
//
// 遍历当前层链表，遇到有 child 的节点 curr：
//   - 递归扁平化 child 链表，得到子链表尾 childTail
//   - 四步拼接：curr ↔ child头, childTail ↔ nextNode
//   - 清除 curr->child
//   - 继续处理 nextNode
//
// 递归过程:
//   flattenDFS(1→2→3→4→5→6)
//     curr=3 有 child → flattenDFS(7→8→9→10)
//       curr=8 有 child → flattenDFS(11→12)
//         无 child，返回 tail=12
//       拼接: 8↔11↔12↔9, 继续走到10, 返回 tail=10
//     拼接: 3↔7...10↔4, 继续走到6, 返回 tail=6
// ============================================================
class Solution1 {
public:
    Node* flatten(Node* head) {
        flattenDFS(head);
        return head;
    }

private:
    // 返回扁平化后链表的尾节点
    Node* flattenDFS(Node* head) {
        Node* curr = head;
        Node* tail = nullptr;

        while (curr) {
            // 先保存 next！拼接会覆盖 curr->next
            Node* nextNode = curr->next;

            if (curr->child) {
                // 递归扁平化子链表
                Node* childTail = flattenDFS(curr->child);

                // 拼接步骤1: curr ↔ child 链表头
                curr->next = curr->child;
                curr->child->prev = curr;

                // 拼接步骤2: child 链表尾 ↔ 原来的 next
                childTail->next = nextNode;
                if (nextNode) {          // nextNode 可能为 null！
                    nextNode->prev = childTail;
                }

                // 清除 child 指针（题目要求）
                curr->child = nullptr;

                // 更新 tail：如果 nextNode 为 null，当前尾就是 childTail
                tail = childTail;
            } else {
                tail = curr;
            }

            curr = nextNode;  // 用之前保存的 next 继续
        }

        return tail;
    }
};

// ============================================================
// 解法2: 迭代 + 显式栈 — 用栈模拟递归的"保存现场"
// 时间: O(n)  空间: O(d)，栈最多存 d 个节点
//
// 【思路】
// 递归时，系统调用栈帮我们保存了"被 child 打断的 next"。
// 我们可以用显式栈手动完成同样的事情：
//   - 遍历链表，遇到有 child 的 curr：
//     - 如果 curr->next 存在，压栈（先保存，之后处理）
//     - 把 child 接到 curr 后面，清除 child
//   - 当走到链末尾（curr->next == null）且栈非空：
//     - 从栈弹出之前保存的 next，接到 curr 后面
//
// 指针变化示意（示例1部分过程）:
//
//  curr=3, child=7, next=4
//  栈: [4]           ← 把4压栈
//  拼接: 3→7, 清除3.child
//
//  curr=8, child=11, next=9
//  栈: [4, 9]        ← 把9压栈
//  拼接: 8→11, 清除8.child
//
//  curr=12, next=null, 栈非空
//  弹出9: 12→9
//  ...
//  curr=10, next=null, 栈非空
//  弹出4: 10→4
//  ...走到6结束
// ============================================================
class Solution2 {
public:
    Node* flatten(Node* head) {
        if (!head) return nullptr;

        // 栈中保存"被 child 链表打断的后续 next 节点"
        stack<Node*> stk;
        Node* curr = head;

        while (curr) {
            if (curr->child) {
                // 如果有 next，先压栈保存
                if (curr->next) {
                    stk.push(curr->next);
                }
                // child 接到 curr 后面
                curr->next = curr->child;
                curr->child->prev = curr;
                curr->child = nullptr;  // 清除 child
            }

            // 走到当前链末尾，从栈中恢复之前的 next
            if (!curr->next && !stk.empty()) {
                Node* nextNode = stk.top();
                stk.pop();
                curr->next = nextNode;
                nextNode->prev = curr;
            }

            curr = curr->next;
        }

        return head;
    }
};

// ============================================================
// 解法3: 迭代原地拼接 — O(1) 额外空间
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法2的栈是为了保存"被打断的 next"。
// 但如果我们在拼接时就把子链表的尾连到 next，就不需要栈了：
//   - 遇到有 child 的 curr，先沿 next 找到子链表尾 childTail
//   - childTail.next = curr.next（子链表尾直接连上原来的 next）
//   - curr.next = child（子链表头接到 curr 后面）
//   - 清除 child
//   - 继续 curr = curr.next（自然走进子链表）
//
// 子链表中如果还有 child，后续遍历到时同样处理。
// 因为子链表已经被嵌入主链，不需要任何额外空间。
//
// 为什么总时间还是 O(n)？
//   每个节点最多被"找尾"访问一次 + 被"主遍历"访问一次 = O(2n) = O(n)
//
// 完整过程示意（示例1）:
//
//  初始: 1↔2↔3↔4↔5↔6
//                |
//                7↔8↔9↔10
//                   |
//                   11↔12
//
//  curr=1: 无child, 继续
//  curr=2: 无child, 继续
//  curr=3: 有child!
//    找子链表尾: 7→8→9→10, childTail=10（只沿next，不进8的child）
//    拼接: 10↔4, 3↔7, 清除3.child
//    主链变为: 1↔2↔3↔7↔8↔9↔10↔4↔5↔6
//                        |
//                        11↔12
//  curr=7: 无child, 继续
//  curr=8: 有child!
//    找子链表尾: 11→12, childTail=12
//    拼接: 12↔9, 8↔11, 清除8.child
//    主链变为: 1↔2↔3↔7↔8↔11↔12↔9↔10↔4↔5↔6
//  之后都无child，遍历到6结束。✓
// ============================================================
class Solution3 {
public:
    Node* flatten(Node* head) {
        Node* curr = head;

        while (curr) {
            if (curr->child) {
                // 找子链表的尾节点（只沿 next，不深入子链表的 child）
                Node* childTail = curr->child;
                while (childTail->next) {
                    childTail = childTail->next;
                }

                // 子链表尾 ↔ curr 的原 next
                childTail->next = curr->next;
                if (curr->next) {            // curr->next 可能为 null
                    curr->next->prev = childTail;
                }

                // curr ↔ 子链表头
                curr->next = curr->child;
                curr->child->prev = curr;

                // 清除 child 指针
                curr->child = nullptr;
            }

            curr = curr->next;  // 自然进入子链表或继续主链
        }

        return head;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法           | 时间 | 空间 | 特点                              |
// |----------------|------|------|-----------------------------------|
// | 递归 DFS       | O(n) | O(d) | 最直觉，返回尾节点设计很巧妙      |
// | 迭代 + 栈      | O(n) | O(d) | 递归的等价迭代形式                |
// | 迭代原地拼接   | O(n) | O(1) | 最优空间，找尾后直接拼接          |
//
// 三种方法本质都是 DFS 前序遍历，区别在于"保存被打断的next"的方式：
//   - 递归: 系统调用栈自动保存
//   - 显式栈: 手动压栈保存
//   - 原地拼接: 不保存，直接连好再继续
//
// ============================================================
// 【易错点】
//
// 1. 忘记清除 child 指针:
//    ✗ 拼接完就忘了 curr->child = nullptr
//    ✓ 题目要求所有 child 必须为 null，漏了会被判错
//
// 2. 忘记 nextNode 可能为 null:
//    ✗ childTail->next = nextNode; nextNode->prev = childTail;
//    ✓ 必须 if(nextNode) nextNode->prev = childTail; 否则空指针崩溃
//
// 3. 递归中拼接前忘记保存 next:
//    ✗ 先 curr->next = curr->child; 然后取 nextNode = curr->next;
//       此时 nextNode 已经变成 child 了！
//    ✓ 必须在拼接前 Node* nextNode = curr->next;
//
// 4. 解法3找尾时误入 child:
//    ✗ while(childTail->next || childTail->child) 递归进 child
//    ✓ 只 while(childTail->next)，因为 child 中的 child 会在后续主循环处理
//
// 5. 双向链表忘记维护 prev:
//    ✗ 只设 a->next = b; 不设 b->prev = a;
//    ✓ 每次改 next 必须同时改对应的 prev
//
// ============================================================
// 【面试追问】
//
// Q1: 这道题的数据结构等价于什么？扁平化的顺序是什么遍历？
// → 等价于多叉树，child=第一个孩子，next=右兄弟。
//   扁平化顺序 = 前序遍历（根→child子树→next兄弟）。
//
// Q2: 递归深度最坏是多少？什么样的输入会触发？
// → O(n)。每个节点都只有 child 没有 next，形成一条"竖链"：
//   1→child→2→child→3→...→n，嵌套 n 层。
//
// Q3: 能否 O(1) 空间？
// → 解法3：找子链表尾直接拼入主链，不需要栈。
//   "找尾"看似额外遍历，但每个节点总共只被访问常数次。
//
// Q4: 这题和 114. 二叉树展开为链表 有什么关系？
// → 几乎同构！114 的 left/right = 这里的 child/next。
//   解法3 的"找尾拼接"思路完全可以复用到 114。
//   区别：114 是单向链表（只用 right），这里是双向（要维护 prev）。
// ============================================================
