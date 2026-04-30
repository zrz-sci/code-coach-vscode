# 103. 二叉树的锯齿形层序遍历

## 核心思路

本质上就是**普通的 BFS 层序遍历**，唯一区别是：偶数层（0-indexed）从左到右读，奇数层从右到左读。关键问题是**怎么处理方向交替**。

## 思维链

1. **读完题第一反应**：这和 LeetCode 102（普通层序遍历）几乎一样，只是每层的输出顺序交替变化——第0层左→右，第1层右→左，第2层左→右...
2. **最朴素的做法**：先做一遍标准 BFS 层序遍历，收集每层结果，然后对奇数层做 `reverse`。这能 work，但每层多了一次 O(k) 的反转操作。
3. **能不能省掉 reverse？** 如果我们知道当前层的方向，可以在**收集节点值的时候就放到正确的位置**——用一个 deque 或者直接控制插入位置（从前插还是从后插）。
4. **还有没有其他角度？** 可以用两个栈交替工作：一个栈先左后右压入子节点，另一个先右后左压入——栈的后进先出天然实现了反向。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS + Reverse | 标准层序遍历，奇数层反转 | O(n) | O(n) | 能说出即可 |
| BFS + 方向控制 | 用 deque 控制每层插入方向 | O(n) | O(n) | ⭐ 必须写出 |
| 双栈交替 | 两个栈交替处理，天然实现锯齿 | O(n) | O(n) | 加分项 |
| DFS 递归 | 递归按层收集，奇数层头插 | O(n) | O(n) | 加分项 |

## 关键提示

1. **从 102 题出发**：如果你会写普通层序遍历，这道题只需要加一个"方向"的处理。先想想怎么在 102 的基础上改最少的代码。

2. **ASCII 示意图 — 锯齿遍历过程**：
```
        3           Level 0: 左→右  → [3]
       / \
      9   20        Level 1: 右→左  → [20, 9]
         / \
        15   7      Level 2: 左→右  → [15, 7]
```

3. **方向交替的关键**：不需要真的改变 BFS 的遍历顺序（队列里始终左→右），只需要改**收集值时的插入方向**——奇数层从前面插入即可。

4. **deque 的妙用**：`push_back` vs `push_front` 可以在 O(1) 时间控制插入方向，避免了 O(k) 的 reverse。

5. **双栈为什么能 work**：栈是 LIFO，当前层从左到右弹出时，下一层按右→左压入（先右后左），弹出时自然变成左→右——方向就交替了。

## 解法详解

### 解法1: BFS + Reverse — O(n) / O(n)

**思考过程**：最直接的思路——先做标准层序遍历（和 102 题一模一样），得到每层的结果后，把奇数层（第1层、第3层...）reverse 一下就行了。

```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int size = q.size();
            vector<int> level;
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                level.push_back(node->val);
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            // 奇数层反转：result.size() 就是当前层号（0-indexed）
            if (result.size() % 2 == 1) {
                reverse(level.begin(), level.end());
            }
            result.push_back(level);
        }
        return result;
    }
};
```

**关键点**：这就是 102 题的代码加了两行 reverse 逻辑。简单易懂，面试时可以先说这个方案再优化。reverse 的额外开销其实是 O(n) 总共（所有层节点加起来就是 n），不影响总体复杂度。

---

### 解法2: BFS + Deque 方向控制 — O(n) / O(n) ⭐ 面试首选

**从解法1优化**：解法1的 reverse 虽然不影响总复杂度，但在面试中面试官可能追问"能不能不 reverse"。核心观察：**BFS 队列里的顺序不变（始终左→右），只改变收集值时的插入方向**。

```
// BFS 队列始终保持正常顺序：
//
// Level 0: queue=[3]     → 左→右收集 → push_back  → [3]
// Level 1: queue=[9,20]  → 右→左收集 → push_front → [20,9]
// Level 2: queue=[15,7]  → 左→右收集 → push_back  → [15,7]
//
// 注意：queue 里子节点的入队顺序始终是先左后右！
// 只是在 "读" 的时候改方向，不是在 "入队" 时改方向。
```

```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        bool leftToRight = true;
        
        while (!q.empty()) {
            int size = q.size();
            deque<int> levelDeque; // 用 deque 控制插入方向
            
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                // 根据方向决定从前插还是从后插
                if (leftToRight) {
                    levelDeque.push_back(node->val);
                } else {
                    levelDeque.push_front(node->val);
                }
                
                // 子节点入队顺序永远不变：先左后右
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            result.push_back(vector<int>(levelDeque.begin(), levelDeque.end()));
            leftToRight = !leftToRight; // 方向交替
        }
        return result;
    }
};
```

**关键点**：
- BFS 的核心逻辑（入队顺序）完全不变，只改了"收集值"的方式
- `deque` 的 `push_front` 和 `push_back` 都是 O(1)
- 也可以不用 deque，用 `vector<int> level(size)` 然后用下标控制写入位置：`leftToRight ? i : size-1-i`

---

### 解法3: 双栈交替 — O(n) / O(n)

**不同角度**：用两个栈交替工作。栈是 LIFO，正好可以天然反向。

```
// 双栈原理图：
//
//         3
//        / \
//       9   20
//          / \
//         15   7
//
// stack1 弹出: 3           → 子节点压入 stack2: 先左(9)后右(20)
// stack2 弹出: 20, 9       → 子节点压入 stack1: 先右后左
//   弹出20: 先压右(7)后压左(15)
//   弹出9: 无子节点
// stack1 弹出: 15, 7       → 正好是左→右！
//
// 规律：
// - 从 stack1 弹出(左→右)时，子节点先左后右压入 stack2
// - 从 stack2 弹出(右→左)时，子节点先右后左压入 stack1
```

```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;
        
        stack<TreeNode*> s1, s2; // s1处理左→右层，s2处理右→左层
        s1.push(root);
        
        while (!s1.empty() || !s2.empty()) {
            vector<int> level;
            
            if (!s1.empty()) {
                // 当前层：左→右（从s1弹出）
                while (!s1.empty()) {
                    TreeNode* node = s1.top(); s1.pop();
                    level.push_back(node->val);
                    // 先左后右压入s2 → s2弹出时就是右→左
                    if (node->left)  s2.push(node->left);
                    if (node->right) s2.push(node->right);
                }
            } else {
                // 当前层：右→左（从s2弹出）
                while (!s2.empty()) {
                    TreeNode* node = s2.top(); s2.pop();
                    level.push_back(node->val);
                    // 先右后左压入s1 → s1弹出时就是左→右
                    if (node->right) s1.push(node->right);
                    if (node->left)  s1.push(node->left);
                }
            }
            
            result.push_back(level);
        }
        return result;
    }
};
```

**关键点**：子节点压栈顺序和当前弹栈方向相关。这个解法不需要 reverse，也不需要 deque，纯用栈的 LIFO 性质实现锯齿。但代码稍长，面试时解法2更简洁。

---

### 解法4: DFS 递归 — O(n) / O(n)

**递归角度**：按前序遍历（或任何 DFS 顺序）递归，用层号 `depth` 决定往 result[depth] 的前面还是后面插入。

```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        dfs(root, 0, result);
        return result;
    }
    
private:
    void dfs(TreeNode* node, int depth, vector<vector<int>>& result) {
        if (!node) return;
        
        // 如果是新的一层，先创建空 vector
        if (depth >= result.size()) {
            result.push_back({});
        }
        
        // 偶数层从后插，奇数层从前插
        if (depth % 2 == 0) {
            result[depth].push_back(node->val);
        } else {
            result[depth].insert(result[depth].begin(), node->val);
        }
        
        dfs(node->left, depth + 1, result);
        dfs(node->right, depth + 1, result);
    }
};
```

**关键点**：
- 注意 `insert(begin(), val)` 是 O(k) 操作，不如 deque 的 push_front 高效
- 可以改成先收集再 reverse，或用 deque 作为中间容器
- DFS 解法体现了"树的递归 vs 迭代"的对应关系

## 解法对比

| | BFS+Reverse | BFS+Deque | 双栈 | DFS递归 |
|------|------------|-----------|------|---------|
| 核心思想 | 标准BFS后处理 | BFS中控制方向 | 栈LIFO天然反向 | 递归按层收集 |
| 代码简洁度 | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐ | ⭐⭐ |
| 额外操作 | reverse O(k)/层 | 无 | 无 | insert O(k)/层 |
| 面试推荐 | 先说这个 | 主力写这个 | 追问时说 | 展示递归思维 |

**面试策略**：先说解法1（30秒说清思路），然后写解法2（展示优化能力）。如果面试官问"还有其他方式吗"，说双栈。

## 易错点

1. **层号判断搞反**
   - ✗ `if (result.size() % 2 == 0) reverse(...)` — 第0层是偶数层，不需要反转！
   - ✓ `if (result.size() % 2 == 1) reverse(...)` — 第1、3、5...层（奇数层）才需要反转
   - 原因：层号从0开始，第0层左→右是正常顺序

2. **双栈解法中子节点压栈顺序搞反**
   - ✗ s1弹出(左→右)时先右后左压入s2 → s2弹出变成左→右，没有锯齿效果
   - ✓ s1弹出(左→右)时先左后右压入s2 → s2弹出变成右→左

3. **忘记判空**
   - ✗ 直接 `q.push(root)` 不判断 root 是否为 null
   - ✓ 开头加 `if (!root) return result;`

4. **DFS 解法中 insert 效率问题**
   - `vector.insert(begin(), val)` 是 O(n)，大量调用时性能差
   - 面试时如果用 DFS，要主动提到这个问题并说明可以用 deque 优化

## 面试追问

**Q1: 这和 102 题（普通层序遍历）有什么区别？你的代码改了哪里？**
→ 只改了收集值的方式。BFS 的入队逻辑完全不变（始终先左后右），区别只在于奇数层把值从前面插入（或最后 reverse）。

**Q2: 你的 reverse 方案总复杂度是多少？能不能避免 reverse？**
→ reverse 总复杂度还是 O(n)（所有层加起来），不影响渐进复杂度。但可以用 deque 的 push_front 在 O(1) 时间完成方向控制，省掉 reverse 常数开销。

**Q3: 如果不用队列（BFS），能不能用 DFS 实现？**
→ 可以。DFS 递归时带上层号 depth，用 depth 决定插入方向。但要注意 vector 头部插入是 O(n)，实际效率不如 BFS + deque。

**Q4: 如果树的节点数是 10^7 级别，你的方案需要注意什么？**
→ BFS 用队列，最坏情况下队列存满一层的节点（完全二叉树最后一层约 n/2 个）。空间上没问题，但要确保不做不必要的拷贝（用引用传递 result）。

## 相关题型

- **102. 二叉树的层序遍历** — 本题的基础版，去掉锯齿逻辑就是 102。先做 102 再做 103。
- **107. 二叉树的层序遍历 II** — 和 102 一样但结果从底层到顶层输出，只需最后 reverse result（或用 deque 存 result）。
- **199. 二叉树的右视图** — 同样是 BFS 层序遍历框架，每层只取最后一个节点。复用 BFS 模板，改收集逻辑。
- **637. 二叉树的层平均值** — BFS 框架，每层算平均值。复用同样的"按层处理"模板。
- **剑指 Offer 32-III** — 和本题完全相同。