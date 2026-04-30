# 606. 根据二叉树创建字符串

## 核心思路

前序遍历二叉树，将节点值和子树用括号包裹。关键规则是**省略不必要的空括号**：只有当节点有右子树但无左子树时，才保留左子树的空括号 `()`，以维持字符串与树的一对一映射关系。

## 思维链

1. **前序遍历** — 先访问根，再左子树，再右子树
2. **括号规则分析** — 每个子树用 `()` 包裹
   - 有左有右：`root(left)(right)`
   - 只有左：`root(left)` — 省略右边的 `()`
   - 只有右：`root()(right)` — 必须保留左边的 `()` 作为占位
   - 叶子节点：`root` — 省略两边的 `()`
3. **递归构建** — 递归处理左右子树，根据上述规则拼接字符串
4. **字符串优化** — 可以用 string 引用避免大量拼接开销

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|------|-----------|-----------|--------|
| 递归 DFS | 前序遍历 + 条件括号 | O(n) | O(h) 栈深 | ⭐⭐⭐⭐⭐ |
| 迭代 DFS（栈） | 显式栈模拟前序遍历 | O(n) | O(n) | ⭐⭐⭐ |
| 递归 + string 引用优化 | 避免字符串拷贝 | O(n) | O(h) | ⭐⭐⭐⭐ |

## 关键提示

- **四种情况** — 叶子、只有左子、只有右子、左右都有，括号处理各不同
- **只有右子时必须加 `()`** — 这是唯一保留空括号的情况，否则无法区分左子和右子
- **负数值** — 节点值范围 [-1000, 1000]，负数值会自然包含负号，不影响解析
- **递归终止** — 节点为 nullptr 时返回空字符串

## 解法详解

### 解法一：递归 DFS（推荐）

```cpp
class Solution {
public:
    string tree2str(TreeNode* root) {
        if (!root) return "";

        string result = to_string(root->val);

        // 有左子树, 或有右子树(此时左边要加空括号占位)
        if (root->left || root->right) {
            result += "(" + tree2str(root->left) + ")";
        }
        // 有右子树
        if (root->right) {
            result += "(" + tree2str(root->right) + ")";
        }

        return result;
    }
};
```

**四种情况分析：**
```
叶子节点 (无左无右):  "val"
只有左子树:           "val(left)"        -- 右边省略
只有右子树:           "val()(right)"     -- 左边空括号占位
左右都有:             "val(left)(right)"
```

### 解法二：递归 + 引用优化（减少字符串拷贝）

```cpp
class Solution {
public:
    string tree2str(TreeNode* root) {
        string result;
        dfs(root, result);
        return result;
    }

private:
    void dfs(TreeNode* node, string& s) {
        if (!node) return;

        s += to_string(node->val);

        if (node->left || node->right) {
            s += '(';
            dfs(node->left, s);
            s += ')';
        }
        if (node->right) {
            s += '(';
            dfs(node->right, s);
            s += ')';
        }
    }
};
```

### 解法三：迭代 DFS（显式栈）

```cpp
class Solution {
public:
    string tree2str(TreeNode* root) {
        if (!root) return "";
        stack<TreeNode*> stk;
        stk.push(root);
        unordered_set<TreeNode*> visited;
        string result;

        while (!stk.empty()) {
            TreeNode* node = stk.top();
            if (visited.count(node)) {
                stk.pop();
                result += ')';
            } else {
                visited.insert(node);
                result += '(' + to_string(node->val);
                if (!node->left && node->right) result += "()";
                if (node->right) stk.push(node->right);
                if (node->left) stk.push(node->left);
            }
        }
        // 去掉最外层多余的括号
        return result.substr(1, result.size() - 2);
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 只有右子树时不加空括号 | ✓ `if (root->left \|\| root->right)` 都要处理左边 | 无左有右时 `()` 是必须的占位符 |
| ✗ 叶子节点也加 `()()` | ✓ 叶子节点只输出值 | 叶子不需要任何括号 |
| ✗ 忘记处理 nullptr 参数 | ✓ `if (!root) return ""` | 递归终止条件 |
| ✗ 条件写成 `if (root->left)` 再单独处理右 | ✓ 用 `if (root->left \|\| root->right)` 统一处理 | 右子存在时即使左子为空也要加括号 |

## 面试追问

**Q1: 如何从字符串反向构建二叉树？（逆问题）**
> 这就是 LeetCode 536 - Construct Binary Tree from String。用递归 + 指针解析：读取数字作为节点值，遇到 `(` 递归解析子树，遇到 `)` 返回。先解析的是左子树，再解析右子树。空括号 `()` 对应 nullptr 的左子树。

**Q2: 字符串拼接的性能问题如何优化？**
> 递归中每次 `+` 操作都会创建新字符串，O(n^2) 总开销。优化方案：(1) 传引用 `string& s`，用 `+=` 追加；(2) 用 `ostringstream`；(3) 预估结果长度后 `reserve`。引用方案最简单且高效。

**Q3: 这道题的时间和空间复杂度是多少？**
> 时间 O(n)：每个节点访问一次。空间取决于方案：递归栈深 O(h)，h 为树高，最坏 O(n)（链状树）。字符串拼接方案（非引用）的拷贝开销可能使时间退化到 O(n^2)，引用优化后为 O(n)。

## 相关题型

- [536. 从字符串构造二叉树](../0536_construct_binary_tree_from_string/) — 本题的逆操作
- [144. 二叉树的前序遍历](../0144_binary_tree_preorder_traversal/) — 前序遍历基础
- [297. 二叉树的序列化与反序列化](../0297_serialize_and_deserialize_binary_tree/) — 更通用的序列化
- [572. 另一棵树的子树](../0572_subtree_of_another_tree/) — 树的字符串表示 + 比较
