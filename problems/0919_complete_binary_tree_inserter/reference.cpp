/*
 * LeetCode 919: 完全二叉树插入器 (Complete Binary Tree Inserter)
 *
 * 【题目本质】
 * 设计数据结构维护完全二叉树，支持 O(1) 插入新节点。
 * 核心技巧：用队列维护"还能接收孩子"的候选父节点，
 * 队首永远是下一个要接收新孩子的节点。
 *
 * 【解法总览】
 * 解法1: 暴力 BFS         — 初始化 O(1) / insert O(n) — 最直觉
 * 解法2: 数组模拟         — 初始化 O(n) / insert O(1) — 利用数组索引
 * 解法3: 队列维护 ⭐      — 初始化 O(n) / insert O(1) — 面试首选
 */

// ============================================================
// 解法1: 暴力 BFS — 每次 insert 都做完整的 BFS 找空位
// 初始化: O(1)  Insert: O(n)  get_root: O(1)
//
// 【思路】
// 最直觉：每次 insert 时从根开始 BFS，找到第一个缺少孩子的
// 节点，把新节点插入为其左/右孩子。
//
// 瓶颈：每次 insert 都要遍历 O(n) 个节点。
// 如果有 m 次 insert，总时间 O(m * n)。
// ============================================================
class CBTInserter1 {
    TreeNode* root;
public:
    CBTInserter1(TreeNode* root) : root(root) {}

    int insert(int val) {
        // 每次 insert 都从根做 BFS
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();

            // 优先填左孩子
            if (!node->left) {
                node->left = new TreeNode(val);
                return node->val;
            }
            q.push(node->left);

            // 左孩子已有，填右孩子
            if (!node->right) {
                node->right = new TreeNode(val);
                return node->val;
            }
            q.push(node->right);
        }

        return -1;  // 不会到这里
    }

    TreeNode* get_root() { return root; }
};

// ============================================================
// 解法2: 数组模拟完全二叉树 — 利用索引关系直接定位父节点
// 初始化: O(n)  Insert: O(1)  get_root: O(1)  空间: O(n)
//
// 【思路】
// 完全二叉树可以用数组存储（层序排列）：
//   节点 i 的左孩子:  2*i + 1
//   节点 i 的右孩子:  2*i + 2
//   节点 i 的父节点:  (i - 1) / 2
//
// 初始化时 BFS 把所有节点按层序存入数组。
// insert 时直接 append 新节点到数组末尾，
// 用公式算出父节点索引，建立父子关系。
//
// 示意（树 [1, 2, 3]）:
//   数组: [1, 2, 3]
//   insert(4): 数组变成 [1, 2, 3, 4]
//     childIdx=3, parentIdx=(3-1)/2=1
//     tree[1]=2, 3是奇数索引 → 4 是 2 的左孩子
// ============================================================
class CBTInserter2 {
    vector<TreeNode*> tree;  // 数组模拟完全二叉树

public:
    CBTInserter2(TreeNode* root) {
        // BFS 把所有节点按层序存入数组
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();
            tree.push_back(node);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }

    int insert(int val) {
        TreeNode* newNode = new TreeNode(val);
        tree.push_back(newNode);

        // 利用数组索引的数学关系定位父节点
        int childIdx = tree.size() - 1;
        int parentIdx = (childIdx - 1) / 2;
        TreeNode* parent = tree[parentIdx];

        // 奇数索引是左孩子，偶数索引是右孩子
        if (childIdx % 2 == 1) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        return parent->val;
    }

    TreeNode* get_root() { return tree[0]; }
};

// ============================================================
// 解法3: 队列维护候选父节点 — 核心设计 ⭐ 面试首选
// 初始化: O(n)  Insert: O(1)  get_root: O(1)  空间: O(n)
//
// 【思路】
// 从暴力 BFS 的瓶颈出发：每次 insert 都从头搜索空位太浪费。
// 能否记住"下一个应该接收孩子的节点"？
//
// 用一个 deque 维护"候选父节点"——还能接收新孩子的节点。
// 队列的 FIFO 特性保证按层序顺序选择父节点，维持完全性。
//
// 初始化: BFS 遍历树，把"缺少左孩子或右孩子"的节点入队。
// insert: 队首就是下一个父节点。
//   - 如果队首没有左孩子 → 新节点作为左孩子（队首不弹出）
//   - 如果队首已有左孩子 → 新节点作为右孩子（弹出队首，它满了）
//   - 新节点自己入队（未来它也可以有孩子）
//
// 完整示意（初始树 [1, 2]）:
//
//     1
//    /
//   2
//
// 初始化后 candidates: [1, 2]
//  (1: 缺右孩子; 2: 缺两个孩子)
//
// insert(3):
//   parent = candidates.front() = 1
//   1 已有左孩子(2) → 3 作为右孩子
//   弹出 1（已满）, 3 入队
//   candidates: [2, 3]
//   返回 1
//
//     1          insert(4):
//    / \           parent = 2, 没有左孩子
//   2   3          → 4 作为左孩子, 2 不弹出, 4 入队
//                  candidates: [2, 3, 4]
//       1          返回 2
//      / \
//     2   3        insert(5):
//    /               parent = 2, 已有左孩子(4)
//   4                → 5 作为右孩子, 弹出 2, 5 入队
//                    candidates: [3, 4, 5]
//       1            返回 2
//      / \
//     2   3
//    / \
//   4   5
// ============================================================
class CBTInserter3 {
    TreeNode* root;
    deque<TreeNode*> candidates;  // 还能接收孩子的节点

public:
    CBTInserter3(TreeNode* root) : root(root) {
        // BFS 遍历，找到所有"还没满两个孩子"的节点
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();

            // 如果左孩子或右孩子缺失，该节点是候选父节点
            if (!node->left || !node->right) {
                candidates.push_back(node);
            }

            // 继续 BFS
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }

    int insert(int val) {
        TreeNode* newNode = new TreeNode(val);
        TreeNode* parent = candidates.front();

        if (!parent->left) {
            // 父节点没有左孩子 → 新节点作为左孩子
            parent->left = newNode;
            // 父节点还可以接收右孩子，不弹出
        } else {
            // 父节点已有左孩子（缺右孩子）→ 新节点作为右孩子
            parent->right = newNode;
            // 父节点已满两个孩子，从候选列表移除
            candidates.pop_front();
        }

        // 新节点自身也是候选父节点（未来可以有孩子）
        candidates.push_back(newNode);

        return parent->val;
    }

    TreeNode* get_root() {
        return root;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 初始化 | Insert | get_root | 空间  | 核心思想                |
// |---------------|--------|--------|----------|-------|------------------------|
// | 暴力 BFS      | O(1)  | O(n)   | O(1)     | O(n)  | 每次搜索空位            |
// | 数组模拟      | O(n)  | O(1)   | O(1)     | O(n)  | 利用完全二叉树的数组表示  |
// | 队列维护 ⭐   | O(n)  | O(1)   | O(1)     | O(n)  | 候选父节点队列的 FIFO   |
//
// 面试中解法2和解法3都是好选择。解法3更直观体现"树的操作"思维。
//
// ============================================================
// 【易错点】
//
// 1. 初始化时入队条件错误
//    ✗ 只入队"没有左孩子"的节点 → 遗漏"有左孩子但没右孩子"的节点
//    ✓ 入队条件: !node->left || !node->right
//
// 2. insert 时弹出队首的时机
//    ✗ 每次 insert 都弹出队首
//    → 错误：如果父节点只填了左孩子，右孩子位置被跳过
//    ✓ 只有右孩子被填上后才弹出（此时父节点已满）
//
// 3. 数组模拟时索引起始混淆
//    0-indexed: 左孩子=2i+1, 右孩子=2i+2, 父=(i-1)/2
//    1-indexed: 左孩子=2i, 右孩子=2i+1, 父=i/2
//    不能混用！
//
// 4. 忘记把新节点自身入队
//    新插入的节点未来也需要接收孩子，必须 push_back 到候选列表。
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么用队列（FIFO）而不是栈（LIFO）？
// → 完全二叉树按层序填充：同一层先填满再到下一层。
//   FIFO 保证了层序顺序，LIFO 会导致倒序填充破坏完全性。
//
// Q2: 数组模拟和队列维护哪个更好？
// → 数组模拟更简洁（利用数学关系），但需要理解索引公式。
//   队列维护更直观（不依赖数组表示），面试表达更容易。
//
// Q3: 如果要支持 delete 操作呢？
// → 删除完全二叉树的节点需要保持完全性。通常做法：
//   1. 找到最后一个节点（队列末尾）
//   2. 用它替换要删除的节点
//   3. 删除最后一个节点并更新队列
//
// ============================================================
// 【相关题型】
//
// - 102. Binary Tree Level Order Traversal
//   标准 BFS 层序遍历，本题初始化阶段的核心操作。
//
// - 222. Count Complete Tree Nodes
//   同样利用完全二叉树的结构特性优化。222 用二分+高度计算。
//
// - 297. Serialize and Deserialize Binary Tree
//   BFS 层序遍历用于序列化，和完全二叉树的数组表示思路相关。
// ============================================================
