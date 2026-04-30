# 536. 从字符串生成二叉树

## 核心思路

字符串格式为 `val(left)(right)`，其中括号内递归包含子树。核心是用**递归**或**栈**来解析嵌套结构：遇到数字构建节点，遇到 `(` 进入子树，遇到 `)` 返回上层。

## 思维链

1. 字符串结构：`数字(左子树)(右子树)`，天然递归
2. 先解析根节点的数字（注意负号）
3. 遇到第一个 `(` → 递归构建左子树
4. 遇到第二个 `(` → 递归构建右子树
5. 遇到 `)` 或字符串末尾 → 返回当前节点
6. 栈解法：遇到 `(` 表示即将进入子节点，遇到 `)` 表示子树构建完毕弹栈

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|-----------|-----------|------|
| 递归（下标引用） | O(n) | O(h) | ⭐⭐⭐ |
| 栈模拟 | O(n) | O(h) | ⭐⭐ |

## 关键提示

1. 解析数字时必须处理**负号**和**多位数**
2. 递归时用**引用传递下标** `int& i`，避免返回后下标不同步
3. 第一个 `(...)` 是左子树，第二个 `(...)` 是右子树
4. 空字符串直接返回 nullptr

## 解法详解

### 解法一：递归（推荐）

```cpp
class Solution {
public:
    TreeNode* str2tree(string s) {
        int i = 0;
        return helper(s, i);
    }

    TreeNode* helper(string& s, int& i) {
        if (i >= s.size()) return nullptr;

        // 解析数字（含负号）
        int sign = 1;
        if (s[i] == '-') { sign = -1; i++; }
        int num = 0;
        while (i < s.size() && isdigit(s[i])) {
            num = num * 10 + (s[i] - '0');
            i++;
        }
        TreeNode* node = new TreeNode(sign * num);

        // 左子树
        if (i < s.size() && s[i] == '(') {
            i++; // 跳过 '('
            node->left = helper(s, i);
            i++; // 跳过 ')'
        }
        // 右子树
        if (i < s.size() && s[i] == '(') {
            i++; // 跳过 '('
            node->right = helper(s, i);
            i++; // 跳过 ')'
        }
        return node;
    }
};
```

### 解法二：栈模拟

```cpp
class Solution {
public:
    TreeNode* str2tree(string s) {
        if (s.empty()) return nullptr;
        stack<TreeNode*> stk;
        int i = 0;

        while (i < s.size()) {
            if (s[i] == ')') {
                stk.pop();
                i++;
            } else if (s[i] == '(') {
                i++;
            } else {
                // 解析数字
                int sign = 1;
                if (s[i] == '-') { sign = -1; i++; }
                int num = 0;
                while (i < s.size() && isdigit(s[i])) {
                    num = num * 10 + (s[i] - '0');
                    i++;
                }
                TreeNode* node = new TreeNode(sign * num);
                if (!stk.empty()) {
                    TreeNode* parent = stk.top();
                    if (!parent->left) parent->left = node;
                    else parent->right = node;
                }
                stk.push(node);
            }
        }
        return stk.empty() ? nullptr : stk.top();
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `int i` 按值传递 | `int& i` 按引用传递 | 递归返回后下标必须同步 |
| 忽略负号直接解析数字 | 先检查 `-` 再解析 | 节点值可为负数 |
| 栈解法中 `)` 时不弹栈 | 遇到 `)` 必须 pop | 表示当前子树构建完毕 |
| 空字符串不做特判 | `if (s.empty()) return nullptr` | 边界条件 |

## 面试追问

**Q1: 如何反向将二叉树序列化为这种字符串格式？**
→ 前序遍历：输出 val，若有左子树输出 `(left)`，若有右子树输出 `(right)`。

**Q2: 如果只有右子树没有左子树怎么表示？**
→ 题目保证优先构建左子树，所以 `val()(right)` 表示空左子树。

**Q3: 递归深度过大怎么优化？**
→ 用栈模拟递归，将递归转为迭代避免栈溢出。

## 相关题型

- [LeetCode 297. Serialize and Deserialize Binary Tree](https://leetcode.com/problems/serialize-and-deserialize-binary-tree/) - 二叉树序列化
- [LeetCode 606. Construct String from Binary Tree](https://leetcode.com/problems/construct-string-from-binary-tree/) - 反向操作
- [LeetCode 105. Construct Binary Tree from Preorder and Inorder Traversal](https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/) - 构建二叉树
