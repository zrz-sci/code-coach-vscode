# 2236. Root Equals Sum of Children - 提示与详解

## 难度：Easy | 标签：Tree, Binary Tree

---

## 一、核心思路

本题的核心极其简单：**直接比较根节点的值是否等于左右子节点值之和**。

树固定只有 3 个节点（根、左孩子、右孩子），无需递归、无需遍历，一行代码即可解决。但这道题是很好的 **二叉树基础入门题**，可以用来理解树的结构定义和节点访问方式。

**一句话总结**：`return root->val == root->left->val + root->right->val;`

---

## 二、思维链：从题目到解法

### Step 1：理解树的结构

```
题目保证：树恰好有 3 个节点

        root (10)
       /         \
   left (4)    right (6)

问题：root->val == left->val + right->val ?
      10       ==   4       +   6        ?
      10       ==   10                    ? => true
```

### Step 2：ASCII 图示 —— 两个示例

```
示例 1:                    示例 2:
        [10]                      [5]
       /    \                    /    \
     [4]    [6]               [3]    [1]

  10 == 4 + 6               5 == 3 + 1
  10 == 10  => TRUE         5 == 4  => FALSE
```

### Step 3：思考边界情况

虽然题目保证了结构，但面试中可能被追问：
- 节点值可以为负数（-100 <= Node.val <= 100）
- 如果 root 为 nullptr 呢？题目保证不会，但防御性编程可以加判断
- 如果左或右孩子为 nullptr 呢？题目保证不会，但通用版本需要处理

### Step 4：一行搞定

```cpp
return root->val == root->left->val + root->right->val;
```

---

## 三、解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|------|-----------|-----------|----------|
| ⭐ 直接比较 | 一行表达式 | O(1) | O(1) | 本题最佳 |
| 通用递归版 | 递归计算子树和 | O(n) | O(h) | 扩展到任意树 |
| BFS 版 | 层序遍历验证 | O(n) | O(w) | 教学演示 |

---

## 四、关键提示（逐步递进）

<details>
<summary>提示 1：树的结构是什么样的？</summary>

题目保证树恰好有 3 个节点：root、root->left、root->right。这意味着你不需要递归遍历，直接访问这三个节点即可。

</details>

<details>
<summary>提示 2：如何访问子节点的值？</summary>

通过 root->left->val 和 root->right->val 分别访问左右子节点的值。因为题目保证了左右子节点一定存在，所以不会空指针异常。

</details>

<details>
<summary>提示 3：需要考虑负数吗？</summary>

Node.val 范围是 [-100, 100]，值可以为负。但这不影响比较逻辑，直接用 == 比较即可（整数比较，没有浮点精度问题）。

</details>

<details>
<summary>提示 4：如何扩展到通用树？（面试追问）</summary>

如果树不是固定 3 节点，而是要验证"每个非叶节点的值 == 所有子节点值之和"，就需要递归：后序遍历，先算左右子树的和，再验证当前节点。

</details>

---

## 五、解法详解

### 解法一：直接比较（本题最佳）

```cpp
/**
 * 树的结构（题目保证）：
 *
 *        root
 *       /    \
 *     left   right
 *
 * 只需比较 root->val == left->val + right->val
 */
class Solution {
public:
    bool checkTree(TreeNode* root) {
        return root->val == root->left->val + root->right->val;
    }
};
```

**执行过程**：

```
输入：root = [10, 4, 6]

        [10]         root->val = 10
       /    \
     [4]    [6]      left->val = 4, right->val = 6

10 == 4 + 6 => 10 == 10 => return true
```

### 解法二：防御性编程版本

```cpp
/**
 * 加入 nullptr 检查，适用于面试中展示代码健壮性
 */
class Solution {
public:
    bool checkTree(TreeNode* root) {
        // 防御性检查：虽然题目保证不会发生
        if (!root || !root->left || !root->right) {
            return false;
        }
        return root->val == root->left->val + root->right->val;
    }
};
```

### 解法三：通用递归版本（面试追问扩展）

```cpp
/**
 * 扩展问题：验证树中"每个非叶节点的值等于其子节点值之和"
 *
 *       26
 *      /  \
 *    10    3
 *   / \     \
 *  4   6     3
 *
 * 后序遍历：先计算子树和，再验证当前节点
 */
class Solution {
public:
    bool checkTree(TreeNode* root) {
        // 对于只有 3 节点的树，这等价于直接比较
        return getSum(root) != -1;
    }

private:
    // 返回子树的和；如果不满足条件，返回 -1 表示无效
    // 注意：实际实现中 -1 可能是有效值，用 optional 更好
    int getSum(TreeNode* node) {
        if (!node) return 0;

        // 叶节点直接返回自身值
        if (!node->left && !node->right) {
            return node->val;
        }

        int leftSum = getSum(node->left);
        int rightSum = getSum(node->right);

        // 如果子树已经不满足条件
        if (leftSum == INT_MIN || rightSum == INT_MIN) {
            return INT_MIN;
        }

        // 验证当前节点
        int childSum = leftSum + rightSum;
        if (node->val != childSum) {
            return INT_MIN; // 不满足条件
        }

        return node->val;
    }
};
```

### 解法四：BFS 教学版本

```cpp
/**
 * 用层序遍历的思路（纯教学，本题过度设计）
 * 展示 BFS 模板在树问题中的应用
 */
class Solution {
public:
    bool checkTree(TreeNode* root) {
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();

            // 如果是非叶节点，检查条件
            if (node->left || node->right) {
                int sum = 0;
                if (node->left) {
                    sum += node->left->val;
                    q.push(node->left);
                }
                if (node->right) {
                    sum += node->right->val;
                    q.push(node->right);
                }
                if (node->val != sum) return false;
            }
        }
        return true;
    }
};
```

---

## 六、二叉树节点结构回顾

```cpp
/**
 * 标准 LeetCode 二叉树节点定义：
 *
 * struct TreeNode {
 *     int val;                    // 节点存储的值
 *     TreeNode *left;             // 指向左子节点的指针
 *     TreeNode *right;            // 指向右子节点的指针
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right)
 *         : val(x), left(left), right(right) {}
 * };
 *
 * 内存布局示意：
 *
 *  TreeNode root          TreeNode left         TreeNode right
 *  +----------+          +----------+          +----------+
 *  | val: 10  |          | val: 4   |          | val: 6   |
 *  | left:  -------->    | left: nil|          | left: nil|
 *  | right: -------->    | right:nil|          | right:nil|
 *  +----------+          +----------+          +----------+
 */
```

---

## 七、解法对比

| 维度 | 直接比较 | 防御版 | 通用递归 | BFS |
|------|---------|--------|---------|-----|
| 代码行数 | 1 行 | 4 行 | 20+ 行 | 15+ 行 |
| 时间复杂度 | O(1) | O(1) | O(n) | O(n) |
| 空间复杂度 | O(1) | O(1) | O(h) | O(w) |
| 是否过度设计 | 否 | 稍微 | 是（但面试加分） | 是 |
| 面试推荐度 | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐（追问时） | ⭐ |

---

## 八、易错点

### Bug 1：忘记 val 是值不是节点

```cpp
// 错误：比较的是指针，不是值
return root == root->left + root->right;
// 正确：比较 val 字段
return root->val == root->left->val + root->right->val;
```

### Bug 2：溢出风险（本题不会发生但要了解）

```cpp
// Node.val 范围 [-100, 100]
// left->val + right->val 最大 200，不会溢出 int
// 但如果范围是 [-2^31, 2^31-1]，两个 int 相加可能溢出
// 安全做法：
return root->val == (long long)root->left->val + root->right->val;
```

### Bug 3：空指针解引用

```cpp
// 如果左或右子节点为 nullptr（本题不会发生）
// root->left->val 会崩溃
// 面试中最好加防御性检查
if (!root->left || !root->right) return false;
```

### Bug 4：通用版本中用特殊值标记无效

```cpp
// 如果用 -1 标记"不满足条件"，但 -1 可能是合法的子树和
// 应该用 optional<int> 或 INT_MIN 等不可能出现的值
// 或者用额外的 bool 变量
```

---

## 九、面试追问（递进链）

### Q1：如果树不是 3 节点，而是任意二叉树，如何验证每个节点的值等于其子节点值之和？

需要后序遍历（DFS），自底向上计算子树和并验证：

```cpp
bool isValid(TreeNode* root) {
    return dfs(root).second;
}

// 返回 {子树和, 是否有效}
pair<int, bool> dfs(TreeNode* node) {
    if (!node) return {0, true};
    if (!node->left && !node->right) return {node->val, true};

    auto [leftSum, leftOk] = dfs(node->left);
    auto [rightSum, rightOk] = dfs(node->right);

    bool valid = leftOk && rightOk && (node->val == leftSum + rightSum);
    return {node->val, valid};
}
```

### Q2：如果要修改树使得每个节点满足这个性质（Children Sum Property），怎么做？

```
策略：自顶向下修改
- 如果 parent < leftChild + rightChild：增大 parent
- 如果 parent > leftChild + rightChild：增大较小的 child
- 然后递归处理子树
- 回溯时更新 parent = left + right
```

### Q3：时间复杂度 O(1) 是否意味着这题在面试中"太简单"？

不完全是。面试官可能用这道题作为 warm-up，然后追问：
1. 如何扩展到 N-ary tree？
2. 如果值是浮点数，如何处理精度？
3. 如何验证整棵树是否是 "Sum Tree"（每个节点 = 左右子树所有节点之和）？

### Q4：什么是 Sum Tree？这道题和 Sum Tree 有什么关系？

Sum Tree 中每个非叶节点的值 = 左子树所有节点值之和 + 右子树所有节点值之和。
本题是 Sum Tree 的最简单特例（只有一层子节点）。

### Q5：如何用迭代（非递归）验证通用版本？

用栈模拟后序遍历，或者用两次遍历：
1. 第一次遍历计算每个节点的子树和
2. 第二次遍历验证每个非叶节点

---

## 十、相关题型

| 题号 | 题名 | 关联点 |
|------|------|--------|
| 100 | Same Tree | 树的基础比较 |
| 104 | Maximum Depth of Binary Tree | 树的递归入门 |
| 112 | Path Sum | 树的值比较 + 递归 |
| 226 | Invert Binary Tree | 树的结构操作 |
| 617 | Merge Two Binary Trees | 树的值操作 |
| 404 | Sum of Left Leaves | 树的值累加 |
| 508 | Most Frequent Subtree Sum | 子树和统计 |

---

## 十一、总结

```
本题知识图谱：

   [二叉树基础]
       |
   [节点访问] -----> root->val, root->left, root->right
       |
   [本题: 3节点比较] -----> O(1) 一行搞定
       |
   [扩展: Sum Tree 验证] -----> 后序遍历 O(n)
       |
   [扩展: Children Sum 修改] -----> 自顶向下 + 回溯
```

**记忆要点**：
- 本题就是一行代码：`return root->val == root->left->val + root->right->val;`
- 面试价值在于引出 Sum Tree 相关的递归讨论
- 展示防御性编程和对边界条件的思考
