# 117. 填充每个节点的下一个右侧节点指针 II

## 核心思路
对任意二叉树（非完美），将每层节点用 next 指针串联成链表。关键难点是用 O(1) 空间处理非完美二叉树中缺失的子节点。

## 思维链
1. **对比 LC 116**：116 是完美二叉树，可以利用"左子连右子，右子连兄弟的左子"的规律；本题是任意二叉树，某些子节点可能缺失
2. **BFS 层序遍历**：用队列做层序遍历，同一层节点依次连接。简单但空间 O(n)
3. **利用已建立的 next 指针**：上一层的 next 已经连好，可以用它遍历上一层来连接下一层
4. **dummy 节点技巧**：用一个 dummy 节点作为下一层链表的头，避免繁琐的边界判断
5. **最终方案**：利用 next 指针 + dummy 节点，O(1) 额外空间

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS 层序遍历 | 队列逐层连接 | O(n) | O(n) | 基础 |
| 利用 next 指针 + dummy | 上层指针遍历连下层 | O(n) | O(1) | ⭐ 必知 |

## 关键提示
1. **核心思想**：当前层的 next 已经连好了，用它来遍历当前层，同时连接下一层的子节点
2. **dummy 节点**：在每层开始时创建 dummy，用 tail 指针追踪下一层链表的末尾
3. **处理顺序**：必须先处理左子再处理右子，保证 next 连接的顺序正确

```
输入树：         连接后：
      1               1 -> NULL
     / \              / \
    2   3            2 -> 3 -> NULL
   / \   \          / \    \
  4   5   7        4-> 5 -> 7 -> NULL

利用 next 指针遍历上层来连接下层：
当前层: 2 -> 3 -> NULL (已连好)
下层构建过程:
  遍历2: 左子4 -> 右子5
  遍历3: (无左子) -> 右子7
  结果: 4 -> 5 -> 7 -> NULL
```

## 解法详解

### 解法1：BFS 层序遍历

**思考过程**：
- 最直观的方法：用队列做层序遍历
- 每层开始时记录队列大小，依次取出节点并用 next 连接
- 空间需要 O(n) 存队列

```cpp
class Solution {
public:
    Node* connect(Node* root) {
        if (!root) return nullptr;
        
        queue<Node*> q;
        q.push(root);
        
        while (!q.empty()) {
            int size = q.size();
            Node* prev = nullptr;
            
            for (int i = 0; i < size; i++) {
                Node* curr = q.front();
                q.pop();
                
                if (prev) prev->next = curr;
                prev = curr;
                
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
            // 每层最后一个节点 next 默认为 NULL
        }
        
        return root;
    }
};
```

### 解法2：利用 next 指针 + dummy 节点（O(1) 空间）

**思考过程**：
- 关键洞察：处理第 k+1 层时，第 k 层的 next 已经连好了
- 可以用 next 指针遍历第 k 层，同时将第 k+1 层的子节点串成链表
- 用 dummy 节点简化链表头的处理

```cpp
class Solution {
public:
    Node* connect(Node* root) {
        Node* curr = root;  // 当前层的遍历指针
        
        while (curr) {
            Node dummy(0);       // 下一层的虚拟头节点
            Node* tail = &dummy; // 下一层链表的尾指针
            
            // 遍历当前层（利用已建好的 next 指针）
            Node* p = curr;
            while (p) {
                if (p->left) {
                    tail->next = p->left;
                    tail = tail->next;
                }
                if (p->right) {
                    tail->next = p->right;
                    tail = tail->next;
                }
                p = p->next;
            }
            
            // 移动到下一层的第一个节点
            curr = dummy.next;
        }
        
        return root;
    }
};
```

**关键点**：
- dummy 节点避免了判断"下一层第一个节点是谁"的复杂逻辑
- 每层结束后 `dummy.next` 就是下一层的头节点
- 外层循环 `while(curr)` 在某层没有子节点时自然终止

## 易错点

1. **忘记重置 dummy**
```cpp
// ✗ 错误：dummy 在循环外创建，第二层之后 dummy.next 还是旧值
Node dummy(0);
while (curr) {
    Node* tail = &dummy;
    // ...
    curr = dummy.next;  // 可能拿到旧层的头！
}

// ✓ 正确：每层都重新创建 dummy
while (curr) {
    Node dummy(0);
    Node* tail = &dummy;
    // ...
    curr = dummy.next;
}
```

2. **遍历方向错误（先右后左）**
```cpp
// ✗ 错误：先处理右子再处理左子，next 连接顺序反了
if (p->right) { tail->next = p->right; tail = tail->next; }
if (p->left)  { tail->next = p->left;  tail = tail->next; }

// ✓ 正确：必须先左后右
if (p->left)  { tail->next = p->left;  tail = tail->next; }
if (p->right) { tail->next = p->right; tail = tail->next; }
```

3. **BFS 方法中忘记每层最后一个节点 next 为 NULL**
```cpp
// ✗ 错误：没有处理最后一个节点
prev->next = curr;  // 最后一个节点的 next 没设为 NULL

// ✓ 正确：Node 构造函数默认 next=NULL，所以不需要额外处理
// 但如果 next 初始不为 NULL，需要显式设置
```

## 面试追问

**Q1: 为什么需要 dummy 节点？不用行吗？**
→ 可以不用，但需要额外变量记录下一层第一个节点（nextHead），以及处理 tail 初始为 nullptr 的边界。dummy 让代码更简洁统一。

**Q2: 这道题和 LC 116 的区别在哪？代码为什么不同？**
→ LC 116 是完美二叉树，每个非叶节点都有两个子节点，所以可以直接 `left->next = right`，`right->next = parent->next->left`。本题任意二叉树可能缺子节点，必须用通用的 dummy 链表法。

**Q3: 如果树不是二叉树而是 N 叉树，怎么修改？**
→ 把处理左右子节点的两个 if 改成遍历 children 数组：`for (auto child : p->children) { tail->next = child; tail = tail->next; }`

## 相关题型
- [116. Populating Next Right Pointers in Each Node](https://leetcode.com/problems/populating-next-right-pointers-in-each-node/) - 完美二叉树版本
- [102. Binary Tree Level Order Traversal](https://leetcode.com/problems/binary-tree-level-order-traversal/) - BFS 层序遍历基础
- [199. Binary Tree Right Side View](https://leetcode.com/problems/binary-tree-right-side-view/) - 层序遍历变体
