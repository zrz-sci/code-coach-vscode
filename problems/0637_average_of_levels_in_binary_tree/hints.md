# 637. 二叉树的层平均值

## 核心思路

标准的 **BFS 层序遍历**问题：逐层处理二叉树节点，每层计算节点值的平均值。这是 BFS 模板题中最经典的变种之一。

## 思维链

1. **读完题第一反应** → "每一层的平均值"，说明需要逐层处理。BFS（层序遍历）天然按层处理，非常适合。
2. **BFS 怎么区分"层"？** → 关键技巧：每轮循环开始时记录队列大小 `levelSize = q.size()`，然后恰好处理 `levelSize` 个节点。处理完后队列里剩下的全是下一层的节点。
3. **怎么算平均值？** → 每层累加所有节点值得到 `sum`，然后 `sum / levelSize`。
4. **有什么坑？** → **整数溢出**！题目说 `Node.val` 范围是 `[-2^31, 2^31-1]`，如果一层有很多大值节点，int 求和会溢出。必须用 `long long` 或 `double` 累加。
5. **能用 DFS 做吗？** → 当然可以。DFS 时记录每个节点的层级，用两个数组分别存每层的节点值之和与节点数，最后逐层算平均值。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS 层序遍历 | 队列逐层处理，每层算平均 | O(n) | O(w) 最大宽度 | ⭐ 面试首选 |
| DFS 递归 | 记录每层的总和与节点数 | O(n) | O(h) 树高 | 可讲替代方案 |

## 关键提示

1. **BFS 分层的标准模板**（几乎所有"逐层处理"的树题都用这个模板）：

   ```
   ASCII 图示 — BFS 逐层处理过程:
   
   树结构:
           3          ← Level 0
          / \
         9   20       ← Level 1
            /  \
          15    7      ← Level 2
   
   BFS 过程:
   ┌──────────────────────────────────────────────────────┐
   │ 初始: queue = [3]                                     │
   │                                                       │
   │ Level 0: levelSize = 1                                │
   │   处理 3 → sum = 3                                    │
   │   入队: 9, 20                                         │
   │   avg = 3/1 = 3.0                                    │
   │   queue = [9, 20]                                     │
   │                                                       │
   │ Level 1: levelSize = 2                                │
   │   处理 9  → sum = 9                                   │
   │   处理 20 → sum = 9 + 20 = 29                         │
   │   入队: 15, 7  (9 没有子节点)                           │
   │   avg = 29/2 = 14.5                                   │
   │   queue = [15, 7]                                     │
   │                                                       │
   │ Level 2: levelSize = 2                                │
   │   处理 15 → sum = 15                                  │
   │   处理 7  → sum = 15 + 7 = 22                         │
   │   入队: 无 (叶子节点)                                   │
   │   avg = 22/2 = 11.0                                   │
   │   queue = []  ← 空，BFS 结束                           │
   │                                                       │
   │ 结果: [3.0, 14.5, 11.0]                               │
   └──────────────────────────────────────────────────────┘
   ```

2. **sum 必须用 double 或 long long**：如果用 int 累加，一层有 10000 个值为 `2^31 - 1` 的节点时会溢出。

3. **levelSize 在循环开始时就要记录**：不能在 for 循环条件中写 `i < q.size()`，因为循环体内会改变队列大小（入队子节点）。

4. **DFS 解法的核心**：用 `sums[level]` 和 `counts[level]` 两个 vector 记录每层的累计和与节点数。DFS 遍历时传入当前层级。

   ```
   DFS 过程图示:
   
           3(L0)
          / \
       9(L1) 20(L1)
              / \
          15(L2) 7(L2)
   
   DFS 遍历顺序: 3 → 9 → 20 → 15 → 7
   
   sums:   [3, 29, 22]      ← 各层节点值之和
   counts: [1,  2,  2]      ← 各层节点数
   result: [3.0, 14.5, 11.0]
   ```

## 解法详解

### 解法1: BFS 层序遍历 — O(n) / O(w) ⭐ 面试首选

**思考过程**: "每一层"的信息 → BFS 的 `levelSize` 技巧是标准做法。

```cpp
class Solution {
public:
    vector<double> averageOfLevels(TreeNode* root) {
        vector<double> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int levelSize = q.size();  // 当前层的节点数
            double sum = 0;            // 用 double 避免溢出
            
            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();
                sum += node->val;
                
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            result.push_back(sum / levelSize);
        }
        
        return result;
    }
};
```

**关键点**:
- `levelSize = q.size()` 必须在 for 循环之前保存，因为循环体内 push 会改变队列大小。
- `double sum = 0` 直接用 double 类型累加，避免 int/long 到 double 的精度转换问题。
- 空间 O(w)，w 是树的最大宽度。完全二叉树的最后一层约 n/2 个节点，所以最坏 O(n)。

---

### 解法2: DFS 递归 — O(n) / O(h)

**思考过程**: DFS 也能获取"层信息"——只要在递归时传入当前层级 depth。

```cpp
class Solution {
public:
    vector<double> averageOfLevels(TreeNode* root) {
        vector<double> sums;    // sums[i] = 第 i 层的节点值之和
        vector<int> counts;     // counts[i] = 第 i 层的节点数
        
        dfs(root, 0, sums, counts);
        
        vector<double> result;
        for (int i = 0; i < sums.size(); i++) {
            result.push_back(sums[i] / counts[i]);
        }
        return result;
    }
    
    void dfs(TreeNode* node, int depth, 
             vector<double>& sums, vector<int>& counts) {
        if (!node) return;
        
        // 如果是第一次访问这一层，初始化
        if (depth == sums.size()) {
            sums.push_back(0);
            counts.push_back(0);
        }
        
        sums[depth] += node->val;
        counts[depth]++;
        
        dfs(node->left,  depth + 1, sums, counts);
        dfs(node->right, depth + 1, sums, counts);
    }
};
```

**关键点**:
- `depth == sums.size()` 判断是否首次访问这一层。由于 DFS 先左后右，第一次到达新层级时 sums 的大小恰好等于当前层级。
- `sums` 用 `double` 类型避免溢出。
- 空间 O(h)，h 是树高。对于平衡二叉树 h = O(log n)，对于链状树 h = O(n)。

## 解法对比

|  | BFS 层序 | DFS 递归 |
|--|---------|---------|
| 时间 | O(n) | O(n) |
| 空间 | O(w) 最大宽度 | O(h) 树高 |
| 宽树（扁平） | 空间大 | 空间小 ✓ |
| 深树（瘦长） | 空间小 ✓ | 空间大 |
| 代码直觉 | 非常直觉 | 需要额外的辅助数组 |
| 面试推荐 | ⭐ 首选 | 作为替代方案讲 |

**面试选择**: BFS 更直觉，"逐层处理"和 BFS 是天然匹配。DFS 可以作为 follow-up 讲，展示你能灵活选择遍历方式。

## 易错点

1. **sum 用 int 导致溢出**：
   - ✗ `int sum = 0;` → 10000 个值为 `2^31-1` 的节点会溢出
   - ✓ `double sum = 0;` 或 `long long sum = 0;`

2. **for 循环条件写 q.size()**：
   ```
   ✗ for (int i = 0; i < q.size(); i++) {  // q.size() 在循环中变化！
       ...
       q.push(child);  // 这里改变了 q.size()
   }
   ✓ int levelSize = q.size();             // 先保存
     for (int i = 0; i < levelSize; i++) {
       ...
     }
   ```

3. **DFS 中 depth 判断错误**：
   - ✗ `if (depth >= sums.size())` → 不精确，可能跳层
   - ✓ `if (depth == sums.size())` → 恰好到达新层时初始化

4. **忘记 null 检查**：
   - BFS: 入队前检查 `if (node->left)` 和 `if (node->right)`
   - DFS: 函数开头 `if (!node) return;`
   - 两者都要注意 root 为 null 的边界（虽然题目说非空）

5. **结果类型搞错**：
   - 返回类型是 `vector<double>`，不是 `vector<int>`
   - 平均值可能是小数，如 `14.5`

## 面试追问

**Q1: BFS 和 DFS 在这题中的空间复杂度哪个更优？**
> 取决于树的形状。完全二叉树（宽树）：BFS 空间 O(n/2)，DFS 空间 O(log n)，DFS 更优。链状树（深树）：BFS 空间 O(1)，DFS 空间 O(n)，BFS 更优。一般情况下差别不大。

**Q2: 如果树非常大放不进内存怎么办？**
> 如果树存储在磁盘上，BFS 更好，因为它的访问模式是广度优先的，可以按层读取。DFS 需要反复跳到不同子树，可能导致大量随机磁盘访问。

**Q3: 如何修改来求每层的最大值/最小值/中位数？**
> 最大/最小值：把 `sum += val` 改成 `maxVal = max(maxVal, val)` 即可。中位数：需要收集每层的所有值，排序后取中间值。BFS 天然可以做到（for 循环内收集值）。

**Q4: 如果精度要求更高（不能用 double），怎么处理？**
> 用分子/分母形式存储（`long long sum` 和 `int count`），最后统一转为高精度小数。或者使用 Python 的 `Fraction` 类型。

## 相关题型

- **102. 二叉树的层序遍历** — BFS 分层模板的基础版，返回每层节点列表而非平均值。本题是其直接变种。
- **107. 二叉树的层序遍历 II** — 102 的变种，结果从底层到顶层。只需 reverse 结果即可。
- **103. 二叉树的锯齿形层序遍历** — BFS 分层 + 奇偶层翻转。同样的 levelSize 技巧。
- **199. 二叉树的右视图** — BFS 分层，每层只取最后一个节点。
- **515. 在每个树行中找最大值** — 本题的"平均值"改成"最大值"，模板几乎一样。
- **116. 填充每个节点的下一个右侧节点指针** — BFS 分层，同层节点用 next 指针串联。
