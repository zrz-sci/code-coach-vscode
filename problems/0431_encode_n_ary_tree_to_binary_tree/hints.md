# 431. Encode N-ary Tree to Binary Tree

## 核心思路
将 N 叉树编码为二叉树的经典方法是 **"左孩子右兄弟"（Left-Child Right-Sibling）** 表示法：二叉树节点的 **左子节点** 存储 N 叉树的 **第一个孩子**，**右子节点** 存储 N 叉树的 **下一个兄弟**。

## 思维链
1. N 叉树每个节点可能有任意多个 children
2. 二叉树每个节点只有 left 和 right 两个指针
3. 关键映射：`left` = 第一个 child，`right` = 下一个 sibling
4. encode：DFS 遍历 N 叉树，第一个孩子放 left，剩余孩子链成 right 链
5. decode：反向操作，left 是第一个孩子，沿 right 收集所有兄弟还原 children 列表

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ DFS 递归（左孩子右兄弟） | O(n) | O(h) | 最直观，面试首选 |
| BFS 迭代 | O(n) | O(n) | 用队列层序处理 |

## 关键提示
1. **映射规则**：N-ary node 的 `children[0]` → binary node 的 `left`；`children[1..k]` → 沿 `right` 指针串成链表
2. **递归终止**：node 为 null 时返回 null
3. **decode 时**：从 binary node 的 `left` 出发，沿 `right` 方向遍历收集所有 children
4. **无状态要求**：不能用全局/静态变量，只能用函数参数和局部变量

## 解法详解

### 解法一：DFS 递归（左孩子右兄弟）⭐

**思路**：
- Encode：创建相同 val 的 TreeNode，将第一个 child 递归编码后放 left，剩余 children 依次编码后挂在 right 链上
- Decode：创建相同 val 的 Node，从 TreeNode 的 left 开始，沿 right 方向递归解码每个节点作为 children

```cpp
// Encode
TreeNode* encode(Node* root) {
    if (!root) return nullptr;
    TreeNode* bNode = new TreeNode(root->val);
    if (!root->children.empty()) {
        bNode->left = encode(root->children[0]);
    }
    TreeNode* cur = bNode->left;
    for (int i = 1; i < root->children.size(); i++) {
        cur->right = encode(root->children[i]);
        cur = cur->right;
    }
    return bNode;
}

// Decode
Node* decode(TreeNode* root) {
    if (!root) return nullptr;
    Node* nNode = new Node(root->val);
    TreeNode* cur = root->left;
    while (cur) {
        nNode->children.push_back(decode(cur));
        cur = cur->right;
    }
    return nNode;
}
```

**复杂度**：时间 O(n)，空间 O(h)，h 为树高

## 易错点
- ✗ encode 时忘记处理 children 为空的情况 → ✓ 检查 `children.empty()` 再设置 left
- ✗ decode 时只取了 left 的第一个孩子，忘了沿 right 遍历 → ✓ 用 while 循环沿 right 收集所有兄弟
- ✗ 混淆了 N-ary Node 的 `children` 字段名（有的题用 `neighbors`） → ✓ 确认题目定义用 `children`

## 面试追问

**Q1: 这种编码方式的二叉树高度最坏是多少？**
最坏 O(n)。当 N 叉树退化为一条链（每个节点只有一个 child），编码后二叉树也是一条链。当某个节点有 k 个 children 时，right 链长度为 k-1，总高度 = 原树深度 + 最大 children 数。

**Q2: 能否用 BFS 实现 encode/decode？**
可以。用队列层序遍历 N 叉树，对每个节点的 children 列表建立 right 链。decode 也用队列，从 left 出发沿 right 还原 children。但代码更复杂，DFS 更简洁。

**Q3: 如果节点值可能重复，这种编码还能正确还原吗？**
可以。编码/解码依赖的是树的结构（left/right 指针关系），而非节点值。值重复不影响结构的恢复。

## 相关题型
- [428. Serialize and Deserialize N-ary Tree](https://leetcode.com/problems/serialize-and-deserialize-n-ary-tree/) - N 叉树序列化
- [297. Serialize and Deserialize Binary Tree](https://leetcode.com/problems/serialize-and-deserialize-binary-tree/) - 二叉树序列化
- [559. Maximum Depth of N-ary Tree](https://leetcode.com/problems/maximum-depth-of-n-ary-tree/) - N 叉树深度
