# 617. 合并二叉树

## 核心思路

本质上是**同步遍历两棵树**：两棵树的指针同时走，位置相同的节点值相加，某一侧为空则直接取另一侧。这是"双参数递归"的典型应用。

## 思维链

1. **读完题第一反应**：两棵树结构可能不同，需要"对齐"处理——同一个位置上，可能两边都有节点、只有一边有、或者两边都没有。
2. **最朴素的做法**：递归地同时遍历两棵树，对每个位置分情况讨论。
3. **分情况讨论**：
   - 两个都为 null → 返回 null
   - 只有一个为 null → 返回非空的那棵子树
   - 两个都非 null → 值相加，递归合并左右子树
4. **关键发现**：这三种情况天然构成了递归的终止条件和递推逻辑，不需要额外数据结构。
5. **迭代思路**：递归能做的，栈/队列也能做——用队列同步遍历两棵树（BFS），或用栈（DFS）。
6. **是否需要新建树？**：可以修改 root1 作为结果（省空间），也可以新建节点（不修改输入）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 递归(修改原树) | 同步DFS，把结果合并到root1上 | O(min(m,n)) | O(min(h1,h2)) | ⭐ 必须写出 |
| 递归(新建树) | 同步DFS，每次new新节点 | O(min(m,n)) | O(min(h1,h2)) | 能说出即可 |
| 迭代BFS | 队列同步层序遍历 | O(min(m,n)) | O(min(m,n)) | 加分项 |

> m, n 为两棵树节点数；h1, h2 为树高。遍历范围是两棵树重叠的部分。

## 关键提示

- **提示1**：这道题的递归函数需要**两个参数**（两棵树的当前节点），和 [100. 相同的树](https://leetcode.com/problems/same-tree/) 的双参数递归框架完全一样。
- **提示2**：终止条件有两个"提前返回"——当某一侧为 null 时，直接返回另一侧的整棵子树，不需要继续递归。
- **提示3**：迭代解法的关键是：队列里存的是**一对节点**（来自两棵树的同一位置）。
- **提示4**：ASCII 示意图——示例1的合并过程：

```
   root1:       root2:         merged:
      1            2              3         ← 1+2
     / \          / \            / \
    3   2        1   3          4   5       ← 3+1, 2+3
   /            / \   \        / \   \
  5            4   -   7      5   4   7     ← 5+0, 0+4, 0+7
```

## 解法详解

### 解法1: 递归(修改原树) — O(min(m,n)) / O(min(h1,h2)) ⭐ 面试首选

**思考过程**：树的题优先想递归。两棵树同步走，函数签名自然是 `merge(node1, node2)`。当两边都有节点时，值加在 node1 上，然后递归处理左右子树。当某一侧为 null，直接返回另一侧——这是一个极其优雅的"整棵子树嫁接"操作。

```cpp
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        // 终止条件：某一侧为空，直接返回另一侧（包括两侧都空的情况）
        if (!root1) return root2;
        if (!root2) return root1;
        
        // 两侧都非空：值相加，结果存在root1上
        root1->val += root2->val;
        
        // 递归合并左右子树
        root1->left = mergeTrees(root1->left, root2->left);
        root1->right = mergeTrees(root1->right, root2->right);
        
        return root1;
    }
};
```

**关键点**：
- `if (!root1) return root2;` 这一行处理了"root1为空但root2有一棵子树"的情况，把整棵子树直接嫁接过来，不需要逐节点复制。
- 这个解法**修改了原树 root1**，面试时要主动说明这一点。

---

### 解法2: 递归(新建树) — O(min(m,n)) / O(min(h1,h2))

**与解法1的区别**：不修改输入，每次创建新节点。在面试中如果面试官追问"能否不修改输入"，给出这个解法。

```cpp
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        if (!root1) return root2;  // 注意：严格不修改的话这里也应该深拷贝
        if (!root2) return root1;  // 但题目只要求返回合并后的树，此写法已被接受
        
        // 新建节点，值为两者之和
        TreeNode* merged = new TreeNode(root1->val + root2->val);
        merged->left = mergeTrees(root1->left, root2->left);
        merged->right = mergeTrees(root1->right, root2->right);
        
        return merged;
    }
};
```

**关键点**：如果严格要求不修改任何输入，`if (!root1) return root2;` 这里应该做深拷贝。但 LeetCode 和大多数面试场景下这样写是可接受的。

---

### 解法3: 迭代BFS(队列) — O(min(m,n)) / O(min(m,n))

**从递归到迭代**：递归本质是 DFS，我们用队列改成 BFS 层序遍历。队列中每个元素是**一对节点**（分别来自两棵树的同一位置）。

```cpp
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        if (!root1) return root2;
        if (!root2) return root1;
        
        // 队列存放一对对节点（两棵树中位置相同的节点）
        queue<pair<TreeNode*, TreeNode*>> q;
        q.push({root1, root2});
        
        while (!q.empty()) {
            auto [n1, n2] = q.front();
            q.pop();
            
            // 此时 n1 和 n2 一定都非空（入队时保证的）
            n1->val += n2->val;
            
            // 处理左子树
            if (n1->left && n2->left) {
                q.push({n1->left, n2->left});   // 两边都有，继续配对
            } else if (!n1->left) {
                n1->left = n2->left;             // n1没有左子树，直接嫁接n2的
            }
            // 如果n2->left为空，n1->left保持不变，不需要处理
            
            // 处理右子树（逻辑同上）
            if (n1->right && n2->right) {
                q.push({n1->right, n2->right});
            } else if (!n1->right) {
                n1->right = n2->right;
            }
        }
        
        return root1;
    }
};
```

**关键点**：
- 入队的条件是两边都非空，这样出队时不需要再判 null。
- 某一侧为空时直接嫁接，不入队——对应递归解法中 `if (!root1) return root2` 的逻辑。

---

## 解法对比

| | 递归(改原树) | 递归(新建) | 迭代BFS |
|---|---|---|---|
| 代码量 | 最少(6行) | 少(7行) | 较多(~20行) |
| 是否修改输入 | ✓ 修改root1 | ✗ 不修改（近似） | ✓ 修改root1 |
| 空间 | O(h) 栈深 | O(h) 栈深 | O(n) 队列 |
| 面试推荐 | ⭐ 首选 | 追问时给出 | 展示迭代能力 |

**选择建议**：面试先写递归解法1（简洁且思路清晰），被追问再给迭代。

## 易错点

1. **漏掉 `!root1` 的终止条件**：
   - ✗ 只写 `if (!root1 && !root2) return nullptr;` — 这漏掉了只有一侧为空的情况，后续会空指针访问。
   - ✓ 分别处理 `if (!root1) return root2;` 和 `if (!root2) return root1;`。

2. **迭代解法中入队条件写错**：
   - ✗ `q.push({n1->left, n2->left});` 不检查是否为空 → 出队后解引用空指针。
   - ✓ 只有两边都非空才入队，否则直接嫁接。

3. **迭代解法漏掉 `n2->left` 非空但 `n1->left` 为空的情况**：
   - ✗ 不写 `else if (!n1->left) { n1->left = n2->left; }` → 丢失了 root2 的子树。

## 面试追问

**Q1：你的解法修改了输入，能否不修改？**
→ 可以，每次 new 一个新节点（解法2）。如果严格不修改，null 分支也需要深拷贝。

**Q2：递归解法的最坏空间是多少？什么时候发生？**
→ O(min(h1, h2))，因为递归深度取决于两棵树重叠部分的高度。最坏情况是两棵树都退化为链表且长度相同，此时 O(n)。

**Q3：如果这不是二叉树而是 N 叉树，怎么改？**
→ 核心不变。把 `left/right` 改成遍历 `children` 数组，按索引配对。如果一棵树在某索引没有子节点，直接取另一棵树的。

**Q4：能否用迭代 DFS（栈）而不是 BFS？**
→ 可以，把队列换成栈即可，逻辑完全一样，只是遍历顺序不同。

## 相关题型

- **[100. 相同的树](https://leetcode.com/problems/same-tree/)** — 复用同样的"双参数递归同步遍历"框架，区别：100 比较值是否相等，617 把值相加。
- **[101. 对称二叉树](https://leetcode.com/problems/symmetric-tree/)** — 同样的双参数递归，区别：100/617 是同侧配对 `(left,left)`，101 是交叉配对 `(left,right)`。
- **[226. 翻转二叉树](https://leetcode.com/problems/invert-binary-tree/)** — 单树递归操作，617 是双树递归操作；都练习"递归三要素"。
- **[572. 另一棵树的子树](https://leetcode.com/problems/subtree-of-another-tree/)** — 也需要同步比较两棵树，但嵌套了两层递归（外层找起点，内层比较）。