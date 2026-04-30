# 173. 二叉搜索树迭代器

## 核心思路
实现 BST 的中序遍历迭代器，核心挑战是将递归的中序遍历转化为可暂停/恢复的迭代形式——用栈模拟递归调用栈，实现 O(h) 空间的受控遍历。

## 思维链
1. **最简单方案**: 构造时一次性中序遍历，结果存入数组，next/hasNext 用下标访问 → O(n) 空间
2. **空间优化需求**: Follow-up 要求 O(h) 空间，h 是树高
3. **栈模拟递归**: 中序遍历的递归本质是"先走到最左，处理当前节点，再处理右子树"
4. **受控遍历**: 构造时只将根到最左节点的路径压栈，next() 弹出栈顶后处理右子树
5. **均摊分析**: 虽然单次 next() 最坏 O(h)，但每个节点恰好入栈出栈各一次，均摊 O(1)

## 解法概览
| 解法 | 思路 | 时间(均摊) | 空间 | 面试 |
|------|------|-----------|------|------|
| 栈模拟中序遍历 ⭐ | 受控遍历，栈存左链 | O(1) | O(h) | 必知 |
| 预存数组 | 构造时全部遍历 | O(1) | O(n) | 了解 |

## 关键提示
1. **pushAllLeft**: 核心辅助函数，将节点及其所有左子节点压栈，模拟"递归先走到最左"
2. **next() 逻辑**: 弹出栈顶(当前最小)，然后对其右子树调用 pushAllLeft
3. **均摊 O(1)**: 每个节点入栈一次出栈一次，n 次 next() 总共 O(n)，均摊 O(1)
4. **栈的含义**: 栈中存储的是"还没被访问的祖先节点"，即中序遍历中还没输出的节点

## 解法详解

### 解法1: 栈模拟中序遍历 (推荐)

**思路**: 用栈模拟递归中序遍历的调用栈。初始化时将根到最左叶子的路径压栈。每次 next() 弹出栈顶元素，若它有右子树则对右子树执行同样的"压入左链"操作。

```cpp
class BSTIterator {
    stack<TreeNode*> stk;
    
    void pushAllLeft(TreeNode* node) {
        while (node) {
            stk.push(node);
            node = node->left;
        }
    }
public:
    BSTIterator(TreeNode* root) {
        pushAllLeft(root);
    }
    
    int next() {
        TreeNode* top = stk.top();
        stk.pop();
        pushAllLeft(top->right);
        return top->val;
    }
    
    bool hasNext() {
        return !stk.empty();
    }
};
```

**思考过程**:
- 栈顶永远是当前未访问的最小元素（中序遍历的下一个）
- 弹出后需要处理右子树，因为中序是 左-根-右
- 右子树可能为空（直接跳过），可能有左子树（需要继续压栈）

**复杂度**: next() 均摊 O(1), hasNext() O(1), 空间 O(h)

### 解法2: 预存数组

**思路**: 在构造函数中完成整棵树的中序遍历，将结果存入数组，用指针索引访问。

```cpp
class BSTIterator {
    vector<int> inorder;
    int idx;
    
    void dfs(TreeNode* node) {
        if (!node) return;
        dfs(node->left);
        inorder.push_back(node->val);
        dfs(node->right);
    }
public:
    BSTIterator(TreeNode* root) : idx(0) {
        dfs(root);
    }
    
    int next() {
        return inorder[idx++];
    }
    
    bool hasNext() {
        return idx < inorder.size();
    }
};
```

**思考过程**:
- 实现最简单，但空间 O(n) 不满足 follow-up 要求
- 适合快速写出正确解后再优化

**复杂度**: next() O(1), hasNext() O(1), 空间 O(n)

## 易错点
- ✗ next() 中忘记处理右子树 → 遍历不完整
- ✓ 弹出栈顶后必须 `pushAllLeft(top->right)`
- ✗ pushAllLeft 中忘记处理 node 本身，只压了左子节点
- ✓ 先压 node 本身，再移动到 node->left
- ✗ hasNext() 检查 `stk.size() > 0` 但栈未正确维护
- ✓ 确保每次 next() 后栈的状态正确（弹出+压入右子树左链）

## 面试追问
- **Q1**: 为什么均摊 O(1)？
  - A: 每个节点恰好入栈一次出栈一次，n 次操作总共 2n 步，均摊 O(1)
- **Q2**: 如果还要支持 prev() (反向迭代) 怎么办？
  - A: 维护两个栈，一个正向一个反向，或用 Morris 遍历 + 双向线索
- **Q3**: 如果 BST 在迭代过程中可能被修改？
  - A: 需要 fail-fast 机制（版本号检测），或使用快照隔离

## 相关题型
- [94. Binary Tree Inorder Traversal](../0094_binary_tree_inorder_traversal/) - 基础中序遍历，本题的前置知识
- [285. Inorder Successor in BST](../0285_inorder_successor_in_bst/) - BST 中序后继，类似的栈模拟思路
- [341. Flatten Nested List Iterator](../0341_flatten_nested_list_iterator/) - 嵌套结构的迭代器，类似的栈模拟设计
