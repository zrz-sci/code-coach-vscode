# 101. 对称二叉树

## 核心思路

这道题本质上是问：一棵树的左子树和右子树是否互为镜像？镜像意味着**交叉配对**——左子树的左孩子对应右子树的右孩子，左子树的右孩子对应右子树的左孩子。

## 思维链

1. **读完题第一反应**：对称 = 左右镜像。那"镜像"到底意味着什么？画一下示例就能发现：把右子树翻转后，应该和左子树完全相同。

2. **怎么判断两棵树镜像？** 直觉上想到递归——两棵树 `p` 和 `q` 镜像的条件是：
   - `p.val == q.val`
   - `p` 的左子树和 `q` 的右子树镜像（交叉配对！）
   - `p` 的右子树和 `q` 的左子树镜像

3. **和 100 题"相同的树"有什么关系？** 100 题是判断两棵树**相同**，递归时是同侧配对 `(left, left)` 和 `(right, right)`。本题是判断**镜像**，只需把配对方式改成交叉：`(left, right)` 和 `(right, left)`。**仅此一行改动**。

4. **能否不用递归？** 题目 Follow-up 要求迭代。递归本质是用系统栈存储待比较的节点对，我们可以用队列/栈显式存储这些"待比较的节点对"，每次取出一对来比较。

5. **选队列还是栈？** 都可以！队列是 BFS 风格（层层比较），栈是 DFS 风格（深入比较），逻辑完全一样，只是遍历顺序不同。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 递归 (DFS) | 交叉配对递归比较 | O(n) | O(h) | ⭐ 必须写出 |
| 迭代 (BFS/队列) | 队列存节点对，逐对比较 | O(n) | O(n) | ⭐ Follow-up 必须写出 |
| 迭代 (DFS/栈) | 栈存节点对，逐对比较 | O(n) | O(n) | 能说出即可 |

## 关键提示

- **提示1**：不要试图"层序遍历然后判断每层是否回文"——这个思路在遇到 null 节点时很容易出 bug，而且本质上更复杂。直接想"两棵子树是否镜像"更自然。

- **提示2（可视化交叉配对）**：
  ```
  //     对称树:                    比较过程:
  //         1                      isMirror(2, 2)
  //        / \                       ├── isMirror(3, 3)  ← 左.左 vs 右.右
  //       2   2                      └── isMirror(4, 4)  ← 左.右 vs 右.左
  //      / \ / \
  //     3  4 4  3
  ```

- **提示3**：递归终止条件要处理 3 种情况：都是 null（✓对称）、一个 null 一个非 null（✗不对称）、值不相等（✗不对称）。

- **提示4**：迭代解法的关键：每次从队列中取出的是**一对**节点（而非一个），入队时也是**成对**入队。

- **提示5（和 100 题的对比）**：
  ```
  // 100. 相同的树:     isSame(p->left, q->left) && isSame(p->right, q->right)
  // 101. 对称二叉树:   isMirror(p->left, q->right) && isMirror(p->right, q->left)
  //                    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //                    唯一区别：同侧配对 → 交叉配对
  ```

## 解法详解

### 解法1: 递归 (DFS) — O(n) / O(h) ⭐ 面试首选

**思考过程**：对称 = 左右子树互为镜像。两棵树镜像的递归定义很自然：根值相同，且"左的左 = 右的右"、"左的右 = 右的左"。这就是交叉配对。

```
//  示例1 — 对称:
//         1
//        / \
//       2   2          isMirror(左2, 右2):
//      / \ / \           val 相同 ✓
//     3  4 4  3          isMirror(3, 3) ✓  ← 左.left vs 右.right
//                        isMirror(4, 4) ✓  ← 左.right vs 右.left
//
//  示例2 — 不对称:
//         1
//        / \
//       2   2          isMirror(左2, 右2):
//        \   \           左.left=null, 右.right=null → isMirror(null,null) ✓
//         3   3          左.right=3, 右.left=null → isMirror(3,null) ✗ 一个null一个非null!
```

```cpp
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        return isMirror(root->left, root->right);
    }
    
    bool isMirror(TreeNode* p, TreeNode* q) {
        // 终止条件1: 两个都是 null — 对称 ✓
        if (!p && !q) return true;
        // 终止条件2: 一个 null 一个非 null — 不对称 ✗
        if (!p || !q) return false;
        // 终止条件3: 值不相等 — 不对称 ✗
        if (p->val != q->val) return false;
        
        // 递归: 交叉配对（这是和"相同的树"唯一的区别）
        return isMirror(p->left, q->right)   // 左的左 vs 右的右
            && isMirror(p->right, q->left);   // 左的右 vs 右的左
    }
};
```

**关键点**：
- 递归三要素 — 返回值: bool；终止条件: 三种 null/值的判断；单层逻辑: 交叉配对递归
- 和 100 题（相同的树）对比，仅 `(left,left)(right,right)` 改成了 `(left,right)(right,left)`

### 解法2: 迭代 (BFS/队列) — O(n) / O(n)

**从解法1转化**：递归用系统栈存储"待比较的节点对"，我们用一个队列显式存储即可。每次取出一对比较，然后把它们的孩子**交叉入队**。

```
//  队列变化过程 (示例1):
//  初始:     [(2,2)]
//  取出(2,2): 值相同 ✓ → 入队 (3,3) 和 (4,4)
//  队列:     [(3,3), (4,4)]
//  取出(3,3): 值相同 ✓ → 孩子都是null，不入队
//  队列:     [(4,4)]
//  取出(4,4): 值相同 ✓ → 孩子都是null，不入队
//  队列空 → 返回 true
```

```cpp
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        
        queue<TreeNode*> q;
        // 成对入队: 左子树根 和 右子树根
        q.push(root->left);
        q.push(root->right);
        
        while (!q.empty()) {
            // 成对取出
            TreeNode* p1 = q.front(); q.pop();
            TreeNode* p2 = q.front(); q.pop();
            
            // 两个都是 null — 这对没问题，继续看下一对
            if (!p1 && !p2) continue;
            // 一个 null 或值不等 — 不对称
            if (!p1 || !p2) return false;
            if (p1->val != p2->val) return false;
            
            // 交叉配对入队（核心！）
            q.push(p1->left);   // 左的左
            q.push(p2->right);  // 右的右  → 这一对应该镜像
            q.push(p1->right);  // 左的右
            q.push(p2->left);   // 右的左  → 这一对应该镜像
        }
        
        return true;
    }
};
```

**关键点**：
- 入队顺序必须是**交叉配对**：`(p1->left, p2->right)` 和 `(p1->right, p2->left)` 各自成对
- `continue` 处理 `(null, null)` 的情况——两个都是 null 不报错，继续检查下一对

### 解法3: 迭代 (DFS/栈) — O(n) / O(n)

**和解法2的区别**：把队列换成栈，仅此而已。遍历顺序变成 DFS 风格，但正确性完全相同。

```cpp
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        
        stack<TreeNode*> stk;
        stk.push(root->left);
        stk.push(root->right);
        
        while (!stk.empty()) {
            TreeNode* p2 = stk.top(); stk.pop();  // 注意栈是 LIFO
            TreeNode* p1 = stk.top(); stk.pop();
            
            if (!p1 && !p2) continue;
            if (!p1 || !p2) return false;
            if (p1->val != p2->val) return false;
            
            // 交叉配对入栈
            stk.push(p1->left);
            stk.push(p2->right);
            stk.push(p1->right);
            stk.push(p2->left);
        }
        
        return true;
    }
};
```

## 解法对比

| | 递归 (DFS) | 迭代 (BFS/队列) | 迭代 (DFS/栈) |
|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(h)，h为树高 | O(n)，最坏一层全入队 | O(n) |
| 代码量 | 最简洁 | 稍长 | 和 BFS 几乎一样 |
| 适用场景 | 面试首选，简洁清晰 | Follow-up 要求迭代时 | 和 BFS 等价 |

**什么时候选哪个？**
- 面试先写递归（30秒就能写完），面试官问 Follow-up 再写迭代版
- 递归空间 O(h) 在平衡树时是 O(log n)，比迭代的 O(n) 更优

## 易错点

1. **✗ 递归终止条件漏掉 `(!p && !q)` 的情况**
   ```cpp
   // ✗ 错误: 只检查了一个为 null
   if (!p || !q) return false;  // 当两个都是 null 时也返回了 false!
   
   // ✓ 正确: 先检查都为 null（返回 true），再检查只有一个为 null
   if (!p && !q) return true;
   if (!p || !q) return false;
   ```

2. **✗ 迭代时忘记处理 `(null, null)` 对**
   ```cpp
   // ✗ 错误: 对 null 节点调用 ->val 导致段错误
   if (p1->val != p2->val) return false;
   
   // ✓ 正确: 先用 continue 跳过 (null, null)，再判断单 null
   if (!p1 && !p2) continue;
   if (!p1 || !p2) return false;
   ```

3. **✗ 交叉配对写成同侧配对**
   ```cpp
   // ✗ 错误: 这是判断"相同的树"，不是"对称的树"
   isMirror(p->left, q->left) && isMirror(p->right, q->right);
   
   // ✓ 正确: 交叉配对
   isMirror(p->left, q->right) && isMirror(p->right, q->left);
   ```

## 面试追问

**Q1: 递归的空间复杂度是多少？最坏情况是什么？**
→ O(h)，h 是树高。最坏情况是退化成链表（每个节点只有一个孩子），此时 h = n，空间 O(n)。平衡树时 h = log n。

**Q2: 能否用迭代实现？（Follow-up 原题）**
→ 用队列或栈存储"待比较的节点对"，每次取一对比较，再把孩子交叉入队/入栈。见解法2和解法3。

**Q3: 如果不是判断对称，而是"把一棵树变成对称的"，最少改几个节点？**
→ 这是一个变体思考题。可以递归比较左右子树，遇到不匹配的节点计数。实质上是把 `return false` 改成"计数+1"，并继续递归下去统计所有不匹配的位置。

**Q4: 这题和 100 题（相同的树）有什么关系？**
→ 100 题是判断两棵树相同（同侧配对），101 题是判断一棵树对称（交叉配对）。代码框架完全一致，仅递归调用的参数配对方式不同。如果你会 100 题，改一行就能做出 101 题。

## 相关题型

- **100. 相同的树** — 和本题框架完全一致，区别仅是"同侧配对 `(left,left)+(right,right)`"vs"交叉配对 `(left,right)+(right,left)`"
- **226. 翻转二叉树** — 翻转后的树和原树互为镜像，可以先翻转再用 100 题判相同，但这样改了原树不推荐
- **572. 另一棵树的子树** — 复用"判断两棵树是否相同"的子函数，在主树的每个节点调用