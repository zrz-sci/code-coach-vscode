/*
 * LeetCode 116: 填充每个节点的下一个右侧节点指针
 * Populating Next Right Pointers in Each Node
 * 
 * 【题目本质】
 * 把完美二叉树的每一层节点从左到右用 next 指针串成链表。
 * 
 * 【解法总览】
 * 解法1: BFS 层序遍历      — O(n) / O(n)     — 最直觉
 * 解法2: 递归 DFS           — O(n) / O(log n) — 最简洁
 * 解法3: 逐层推进(利用next) — O(n) / O(1)     — 最优 (Follow-up)
 */

// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;
    Node() : val(0), left(nullptr), right(nullptr), next(nullptr) {}
    Node(int _val) : val(_val), left(nullptr), right(nullptr), next(nullptr) {}
    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};

// ============================================================
// 解法1: BFS 层序遍历 — 最直觉的做法
// 时间: O(n)  空间: O(n) — 队列最多存一整层 ≈ n/2
//
// 【思路】
// 读完题第一反应：这就是层序遍历，只是在遍历每层时，
// 把前一个节点的 next 指向后一个节点。
//
// 完美二叉树示例:
//         1               处理过程:
//        / \              层0: [1] → 1.next=NULL
//       2   3             层1: [2,3] → 2.next=3, 3.next=NULL
//      / \ / \            层2: [4,5,6,7] → 4→5→6→7→NULL
//     4  5 6  7
// ============================================================
class Solution1 {
public:
    Node* connect(Node* root) {
        if (!root) return nullptr;
        
        queue<Node*> q;
        q.push(root);
        
        while (!q.empty()) {
            int size = q.size();
            
            for (int i = 0; i < size; i++) {
                Node* node = q.front();
                q.pop();
                
                // 本层非最后一个节点 → next 指向队列前端（即同层下一个）
                // 为什么 q.front() 就是同层下一个？因为我们是按层入队的
                if (i < size - 1) {
                    node->next = q.front();
                }
                // 本层最后一个节点的 next 保持 nullptr（默认值）
                
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        return root;
    }
};

// ============================================================
// 解法2: 递归 DFS — 最简洁的写法
// 时间: O(n)  空间: O(log n) — 递归栈深度 = 树高 = log n
//
// 【思路】
// 对每个非叶节点做两件事：
// ① 同父连接: left->next = right
// ② 跨父连接: right->next = parent->next->left (如果 parent->next 存在)
//
// 为什么 ② 能工作？因为 parent 的 next 是在处理 grandparent 时就连好的。
// 递归的自顶向下性质保证了：处理 node 时，node->next 已经正确。
//
// 示例过程 (前序遍历顺序):
//
//         1                
//        / \               
//       2 → 3 → NULL       ← 处理 node=1 时连好
//      / \ / \             
//     4→5→6→7 → NULL       ← 处理 node=2 时: 4→5, 5→6(利用2→3)
//                            处理 node=3 时: 6→7
// ============================================================
class Solution2 {
public:
    Node* connect(Node* root) {
        // 空树或叶子节点 → 无需处理
        if (!root || !root->left) return root;
        
        // ① 同父连接: 左孩子指向右孩子（这是必然存在的，完美二叉树）
        root->left->next = root->right;
        
        // ② 跨父连接: 右孩子指向父节点右邻居的左孩子
        // 为什么 root->next 已经连好？因为 root 的 next 是在上一层递归中设置的
        if (root->next) {
            root->right->next = root->next->left;
        }
        
        connect(root->left);
        connect(root->right);
        
        return root;
    }
};

// ============================================================
// 解法3: 利用已建 next 逐层推进 — O(1) 空间最优解 (Follow-up)
// 时间: O(n)  空间: O(1)
//
// 【思路】
// BFS 解法的空间瓶颈是队列。核心洞察：
// 如果当前层的 next 已经全部连好，当前层本身就是一个"链表"！
// 我们可以沿着这个链表遍历当前层，同时把下一层的 next 全部连好。
// 
// 这样从第0层开始逐层下降，每层做的事和解法2一模一样，
// 只是用 while 循环代替递归。
//
// 逐层处理过程:
//
//   leftmost → 1                  （第0层只有root，天然是"链表"）
//              ↓
//   处理第0层: curr=1
//     1.left→next = 1.right  → 2→3
//     1.next==NULL，跳过
//     curr = curr→next = NULL → 内循环结束
//
//   leftmost → 2 → 3 → NULL      （第1层的 next 已连好）
//              ↓
//   处理第1层: curr=2
//     2.left→next = 2.right    → 4→5
//     2.right→next = 2.next.left → 5→6    (利用 2→next = 3)
//     curr = curr→next = 3
//   处理第1层: curr=3
//     3.left→next = 3.right    → 6→7
//     3.next==NULL，跳过
//     curr = NULL → 内循环结束
//
//   leftmost → 4→5→6→7→NULL       （第2层连好）
//   leftmost→left == NULL → 叶子层，外循环结束
// ============================================================
class Solution3 {
public:
    Node* connect(Node* root) {
        if (!root) return nullptr;
        
        // leftmost: 每一层的最左节点，作为内循环的入口
        Node* leftmost = root;
        
        // 外循环：逐层下降。叶子层没有下一层，所以条件是 leftmost->left 存在
        while (leftmost->left) {
            
            // 内循环：沿着当前层的 next 链表遍历，连接下一层
            Node* curr = leftmost;
            while (curr) {
                // ① 同父连接：当前节点的左孩子 → 右孩子
                curr->left->next = curr->right;
                
                // ② 跨父连接：当前节点的右孩子 → 右邻居的左孩子
                if (curr->next) {
                    curr->right->next = curr->next->left;
                }
                
                // 沿 next 移到同层下一个节点
                curr = curr->next;
            }
            
            // 下降到下一层：下一层最左节点 = 当前层最左节点的左孩子
            leftmost = leftmost->left;
        }
        
        return root;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法      | 时间  | 空间      | 核心思想              |
// |-----------|-------|-----------|-----------------------|
// | BFS 队列  | O(n)  | O(n)      | 标准层序遍历          |
// | 递归 DFS  | O(n)  | O(log n)  | 自顶向下，两条连接规则 |
// | 逐层推进  | O(n)  | O(1)      | 已建 next 当链表遍历  |
//
// 解法2 和 解法3 的连接逻辑完全相同（都是同父+跨父两步），
// 区别是：解法2 用递归遍历每个节点，解法3 用嵌套 while 循环。
// 解法3 显式体现了"逐层"的概念，且不使用任何额外空间。
//
// 面试建议：先口述 BFS 思路，然后写出解法3，
// 如果面试官问递归就补充解法2。
//
// ============================================================
// 【易错点】
//
// 1. 跨父连接忘判空:
//    ✗ curr->right->next = curr->next->left;
//      → 当 curr 是本层最右节点时，curr->next==nullptr，空指针崩溃
//    ✓ if (curr->next) { curr->right->next = curr->next->left; }
//
// 2. 外循环条件写成 while(leftmost):
//    ✗ while (leftmost) — 叶子层 leftmost 不为空，但 leftmost->left 为空，
//      进入循环后 curr->left->next 会访问 nullptr 的 next
//    ✓ while (leftmost->left) — 叶子层无需处理
//
// 3. 递归版漏掉空树判断:
//    ✗ if (!root->left) return root;  — root==nullptr 时崩溃
//    ✓ if (!root || !root->left) return root;
//
// 4. BFS 版连接了不同层的节点:
//    ✗ node->next = q.front(); (对每个节点都做)
//    ✓ if (i < size - 1) node->next = q.front(); (只在非末尾时)
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1(基础): 最直觉的做法和复杂度？
//   → BFS 层序遍历，O(n) 时间 O(n) 空间。
//
// Q2(优化): Follow-up 要求 O(1) 空间，怎么做？
//   → 利用已建好的 next 把当前层当链表遍历，同时连接下一层。
//      两个指针 leftmost + curr 就够了。
//
// Q3(变体→117题): 如果不是完美二叉树而是任意二叉树？
//   → 解法3 的两条规则不再直接适用（节点可能没有左/右孩子）。
//     需要用 dummy 头节点构建下一层链表：遍历当前层时，
//     把每个节点的非空子节点依次追加到 dummy 链表上。
//     详见 LeetCode 117。
//
// Q4(深入): 递归栈空间算不算 O(1)？
//   → 题目明确说递归栈不算。但严格来说完美二叉树递归深度
//     O(log n)，不是 O(1)。面试中最好主动提及这一点。
//
// ============================================================
// 【相关题型】
//
// 117. 填充每个节点的下一个右侧节点指针 II
//   — 本题是完美二叉树，117 是任意二叉树
//   — 解法3 的逐层推进框架可复用，但内循环需要改为
//     用 dummy 节点串联下一层的所有非空子节点
//   — 代码改动：去掉固定的 left/right 连接，改为通用的
//     "有子节点就追加到链表"逻辑
//
// 199. 二叉树的右视图
//   — 复用 BFS 层序模板，每层只记录最后一个节点的值
//
// 102. 二叉树的层序遍历
//   — 本题解法1 就是 102 的 BFS 模板 + next 连接
// ============================================================
