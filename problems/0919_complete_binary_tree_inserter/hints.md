[file://solution.cpp](./solution.cpp)

# 919. 完全二叉树插入器

## 核心思路

设计一个数据结构 CBTInserter，维护一棵完全二叉树，支持插入新节点并保持完全性，以及返回根节点。完全二叉树的定义是除最后一层外每层都满，最后一层的节点尽量靠左。关键观察：完全二叉树的节点可以用 BFS 层序遍历排列，插入新节点时只需要找到第一个"还没满两个孩子"的节点作为父节点。用一个队列维护这些"候选父节点"即可。

## 思维链

1. **读完题第一反应**：完全二叉树的插入——新节点应该放在哪里？放在层序遍历中第一个空位。即找到第一个没有左孩子或没有右孩子的节点，把新节点插入为其孩子。
2. **暴力解的方法**：每次 insert 时从根开始做 BFS，找到第一个缺少孩子的节点。时间 O(n) per insert。
3. **瓶颈分析**：每次 insert 都要从头 BFS 一遍太浪费。能否维护一个"当前候选父节点"的列表？
4. **队列优化——核心设计**：
   - 初始化时对树做一次 BFS，把所有"还没满两个孩子"的节点放入队列
   - insert 时：队首就是下一个要接收孩子的节点。如果队首还没左孩子，插入为左孩子；如果已有左孩子但没右孩子，插入为右孩子并弹出队首（因为它已满）。新插入的节点本身也加入队列末尾（它未来也可以有孩子）。
   - 每次 insert 只需 O(1) 操作。

## 解法概览

| 解法 | 思路 | 初始化 | insert | 空间 | 面试 |
|------|------|--------|--------|------|------|
| 暴力 BFS | 每次插入时 BFS 找空位 | O(n) | O(n) | O(n) | 能说出即可 |
| 数组模拟 | 用数组存储完全二叉树 | O(n) | O(1) | O(n) | 可行方案 |
| 队列维护候选父节点 | 初始化时 BFS 建队列，insert O(1) | O(n) | O(1) | O(n) | 面试首选 |

## 关键提示

1. **完全二叉树的 BFS 特性**：层序遍历完全二叉树，节点是连续的。第一个缺少孩子的节点一定是队列中最先出现的"不满"节点。
2. **队列中只保留"不满"的节点**：初始化时做 BFS，跳过已经有两个孩子的节点，只把"有空位"的节点入队。队首永远是下一个应该接收新孩子的节点。
3. **insert 的精确逻辑**：
   - 看队首节点：如果没有左孩子 → 新节点作为左孩子
   - 如果已有左孩子（说明没有右孩子，因为它还在队列中）→ 新节点作为右孩子 → 弹出队首（它满了）
   - 新节点自己也入队（未来它可以有孩子）
4. **get_root 非常简单**：直接返回构造时保存的根节点引用即可。

```
// 队列维护示意（初始树 [1, 2]，即只有根1和左孩子2）:
//
//     1
//    /
//   2
//
// 初始化 BFS:
//   处理 1: 有左孩子(2)，没右孩子 → 入队
//   处理 2: 没左孩子，没右孩子 → 入队
//   队列: [1, 2]  （1缺右孩子，2缺两个孩子）
//
// insert(3):
//   队首 = 1，已有左孩子 → 3 作为 1 的右孩子
//   弹出 1（已满），3 入队
//   队列: [2, 3]
//   返回 1（父节点的值）
//
//     1
//    / \
//   2   3
//
// insert(4):
//   队首 = 2，没有左孩子 → 4 作为 2 的左孩子
//   2 还没满（没右孩子），不弹出。4 入队
//   队列: [2, 3, 4]
//   返回 2
//
//       1
//      / \
//     2   3
//    /
//   4
```

## 解法详解

### 解法1: 暴力 BFS — 每次 insert O(n)

**思考过程**：最直觉——每次 insert 时做一次完整的 BFS，找到第一个缺少孩子的节点。

```cpp
class CBTInserter {
    TreeNode* root;
public:
    CBTInserter(TreeNode* root) : root(root) {}
    
    int insert(int val) {
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();
            
            if (!node->left) {
                node->left = new TreeNode(val);
                return node->val;
            }
            q.push(node->left);
            
            if (!node->right) {
                node->right = new TreeNode(val);
                return node->val;
            }
            q.push(node->right);
        }
        
        return -1;
    }
    
    TreeNode* get_root() { return root; }
};
```

**瓶颈**：每次 insert 都从根开始 BFS，时间 O(n)。插入 m 次总共 O(m * n)。

---

### 解法2: 数组模拟完全二叉树 — insert O(1)

**不同角度**：完全二叉树可以用数组表示，节点 i 的左孩子是 2i+1，右孩子是 2i+2，父节点是 (i-1)/2。初始化时 BFS 把所有节点存入数组，insert 时直接 append 并建立父子关系。

```cpp
class CBTInserter {
    vector<TreeNode*> tree;  // 数组模拟完全二叉树
public:
    CBTInserter(TreeNode* root) {
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
        
        int childIdx = tree.size() - 1;
        int parentIdx = (childIdx - 1) / 2;
        TreeNode* parent = tree[parentIdx];
        
        if (childIdx % 2 == 1) {
            // 奇数索引 → 左孩子
            parent->left = newNode;
        } else {
            // 偶数索引 → 右孩子
            parent->right = newNode;
        }
        
        return parent->val;
    }
    
    TreeNode* get_root() { return tree[0]; }
};
```

**关键点**：利用数组索引的数学关系直接定位父节点，无需搜索。insert 是 O(1)。

---

### 解法3: 队列维护候选父节点 — insert O(1) 面试首选

**核心设计**：用一个 deque/queue 维护"还能接收孩子"的节点。初始化时 BFS 一遍树，把不满的节点入队。insert 时直接用队首作为父节点。

```cpp
class CBTInserter {
    TreeNode* root;
    deque<TreeNode*> candidates;  // 还能接收孩子的节点
    
public:
    CBTInserter(TreeNode* root) : root(root) {
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
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    
    int insert(int val) {
        TreeNode* newNode = new TreeNode(val);
        TreeNode* parent = candidates.front();
        
        if (!parent->left) {
            // 父节点没有左孩子，新节点作为左孩子
            parent->left = newNode;
            // 父节点还可以接收右孩子，不弹出
        } else {
            // 父节点已有左孩子，新节点作为右孩子
            parent->right = newNode;
            // 父节点已满，弹出
            candidates.pop_front();
        }
        
        // 新节点本身也是候选父节点（未来可以有孩子）
        candidates.push_back(newNode);
        
        return parent->val;
    }
    
    TreeNode* get_root() {
        return root;
    }
};
```

**关键点**：
- 队列的 FIFO 特性保证了按层序顺序选择父节点，维持完全二叉树的性质。
- insert 操作只涉及队首的检查和弹出，以及新节点的入队，全部 O(1)。
- 初始化时的 BFS 是一次性的 O(n) 操作。

## 解法对比

| | 暴力 BFS | 数组模拟 | 队列维护 |
|---|---|---|---|
| 初始化 | O(1) | O(n) | O(n) |
| insert | O(n) | O(1) | O(1) |
| get_root | O(1) | O(1) | O(1) |
| 空间 | O(n) BFS临时 | O(n) 数组 | O(n) 队列 |
| 面试推荐 | 口述 | 可行 | **首选** |

**什么时候选哪个？**
- 面试首选队列维护（解法3），思路清晰且面试官常见。
- 数组模拟（解法2）利用了完全二叉树的数组表示，也是好方案。

## 易错点

1. **初始化时忘记把"有左孩子但没右孩子"的节点入队**
   - 只有没有右孩子的节点才需要入队，但容易只检查"没有左孩子"。
   - 正确条件：`!node->left || !node->right`

2. **insert 时弹出队首的时机**
   - 只有在父节点的右孩子被填上之后才弹出（因为此时它已满两个孩子）。
   - 如果在左孩子被填上时就弹出，会跳过右孩子的位置。

3. **数组模拟时索引从 0 开始**
   - 节点 i 的左孩子是 2i+1，右孩子是 2i+2，父节点是 (i-1)/2。
   - 如果用 1-indexed 则是 2i, 2i+1, i/2。两种都可以但不能混用。

4. **get_root 忘记保存根节点引用**
   - 构造函数中必须保存 root 的引用，get_root 直接返回。

## 面试追问

**Q1: 为什么用队列而不是优先队列？**
> 完全二叉树的节点按层序排列，先入队的节点一定先被填满。不需要比较优先级，普通队列的 FIFO 就足够了。

**Q2: 如果初始树不是完全二叉树怎么办？**
> 题目保证初始树是完全二叉树。如果不保证，则初始化时的 BFS 候选节点集合可能不正确——完全二叉树的性质是队列方案的前提条件。

**Q3: insert 的均摊复杂度是多少？**
> 每次 insert 是严格 O(1)——不是均摊而是最坏情况 O(1)。初始化是一次性 O(n)。

**Q4: 如果要支持删除节点呢？**
> 删除完全二叉树的节点要保持完全性，通常做法是删除最后一个节点（层序遍历的最后一个），然后用它替换要删除的节点。这需要维护双端队列以快速找到最后一个节点。

## 相关题型

- **102. Binary Tree Level Order Traversal** — BFS 层序遍历的基础。本题的初始化阶段就是标准的层序遍历。
- **222. Count Complete Tree Nodes** — 同样利用完全二叉树的结构特性来优化。222 用二分 + 完全二叉树性质实现 O(log^2 n)。
- **297. Serialize and Deserialize Binary Tree** — 序列化/反序列化也常用 BFS 层序遍历，和完全二叉树的数组表示思路相关。
