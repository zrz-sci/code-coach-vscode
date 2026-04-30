# 427. 建立四叉树

## 核心思路

这道题本质上是一个**分治问题**：对一个 n×n 的网格，如果所有值相同就作为叶节点，否则将其等分为四个子网格，递归构建四叉树。

## 思维链

1. **读完题第一反应**：题目已经告诉了构建规则——检查当前区域是否全部相同，相同则为叶节点，不同则分成四块递归。最朴素的做法就是直接按照这个定义递归实现。

2. **暴力解的瓶颈在哪？** 每次递归都要遍历当前子网格来判断"是否全部相同"，最坏情况下（如棋盘格），递归层数为 log(n)，每层总共要扫描 n² 个元素，所以总时间是 O(n² log n)。

3. **能否更快判断"区域是否全部相同"？** 用**二维前缀和**！预处理后，O(1) 就能算出任意子矩形的元素和。如果和为 0，全是 0；如果和为 size²，全是 1；否则不全相同。

4. **递归框架不变**，只是把"遍历检查"替换成"前缀和查询"，整体时间从 O(n² log n) 降到 O(n²)。

5. **递归的边界**：当区域大小为 1×1 时，一定是叶节点，可以直接返回。

6. **合并优化（可选）**：递归构建四个子节点后，如果四个子节点都是叶节点且值相同，可以合并为一个叶节点。这在纯分治解法中可以替代"检查全部相同"的步骤。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 朴素分治 | 每次遍历子区域判断是否全同 | O(n² log n) | O(log n) 递归栈 | ⭐ 必须写出 |
| 前缀和 + 分治 | 预处理前缀和，O(1) 判断全同 | O(n²) | O(n²) 前缀和数组 | 加分项 |
| 先递归后合并 | 不预先检查，递归后尝试合并四个子节点 | O(n² log n) 最坏 | O(log n) 递归栈 | 能说出即可 |

## 关键提示

- **提示1**：递归函数需要什么参数？想想你需要描述"当前子网格"的哪些信息——左上角坐标 (r, c) 和边长 size 就够了。

- **提示2**：判断子网格是否全部相同，暴力就是双重循环遍历。但如果你预计算了前缀和，sum == 0 表示全 0，sum == size*size 表示全 1。

- **提示3**：四个子网格的坐标怎么算？设当前左上角 (r, c)，半边长 half = size/2：
```
topLeft:     (r, c)
topRight:    (r, c + half)
bottomLeft:  (r + half, c)
bottomRight: (r + half, c + half)
```

- **提示4**：有一种不需要预先检查的写法——先递归到底（1×1 一定是叶节点），然后在合并时检查：如果四个子节点都是叶节点且值相同，就合并为一个叶节点。

- **示意图**（以示例2的 8×8 网格为例）：
```
整个 8×8 网格不全同 → 分成四块:
+-------+-------+
| 1 1 1 1| 0 0 0 0|   topLeft(全1→叶)    topRight(不全同→继续分)
| 1 1 1 1| 0 0 0 0|
| 1 1 1 1| 1 1 1 1|
| 1 1 1 1| 1 1 1 1|
+-------+-------+
| 1 1 1 1| 0 0 0 0|   bottomLeft(全1→叶)  bottomRight(全0→叶)
| 1 1 1 1| 0 0 0 0|
| 1 1 1 1| 0 0 0 0|
| 1 1 1 1| 0 0 0 0|
+-------+-------+

topRight 继续分:
+---+---+
|0 0|0 0|  全0→叶   全0→叶
|0 0|0 0|
+---+---+
|1 1|1 1|  全1→叶   全1→叶
|1 1|1 1|
+---+---+
```

## 解法详解

### 解法1: 朴素分治 — O(n² log n) / O(log n) ⭐ 面试首选

**思考过程**：题目的定义本身就是递归的——检查当前网格是否全同，全同就做叶节点，否则分四块递归。直接翻译成代码即可。

这是面试中最容易写对、最容易解释的版本。面试官期望你能快速写出这个。

```cpp
/*
// Definition for a Quad-Tree node.
class Node {
public:
    bool val;
    bool isLeaf;
    Node* topLeft;
    Node* topRight;
    Node* bottomLeft;
    Node* bottomRight;
    
    Node() : val(false), isLeaf(false), topLeft(NULL), topRight(NULL), bottomLeft(NULL), bottomRight(NULL) {}
    Node(bool _val, bool _isLeaf) : val(_val), isLeaf(_isLeaf), topLeft(NULL), topRight(NULL), bottomLeft(NULL), bottomRight(NULL) {}
    Node(bool _val, bool _isLeaf, Node* _topLeft, Node* _topRight, Node* _bottomLeft, Node* _bottomRight)
        : val(_val), isLeaf(_isLeaf), topLeft(_topLeft), topRight(_topRight), bottomLeft(_bottomLeft), bottomRight(_bottomRight) {}
};
*/

class Solution {
public:
    Node* construct(vector<vector<int>>& grid) {
        return build(grid, 0, 0, grid.size());
    }
    
private:
    Node* build(vector<vector<int>>& grid, int r, int c, int size) {
        // 检查当前子网格是否所有值相同
        if (allSame(grid, r, c, size)) {
            // 全部相同 → 叶节点，val = 网格中任意一个值
            return new Node(grid[r][c] == 1, true);
        }
        
        // 不全相同 → 分成四块递归
        int half = size / 2;
        Node* topLeft     = build(grid, r,        c,        half);
        Node* topRight    = build(grid, r,        c + half, half);
        Node* bottomLeft  = build(grid, r + half, c,        half);
        Node* bottomRight = build(grid, r + half, c + half, half);
        
        // isLeaf=false, val可以是任意值(这里设为true)
        return new Node(true, false, topLeft, topRight, bottomLeft, bottomRight);
    }
    
    bool allSame(vector<vector<int>>& grid, int r, int c, int size) {
        int first = grid[r][c];
        for (int i = r; i < r + size; i++) {
            for (int j = c; j < c + size; j++) {
                if (grid[i][j] != first) return false;
            }
        }
        return true;
    }
};
```

**关键点**：
- `allSame` 可以提前 return false，实际中很多情况不需要遍历完整个子区域
- 递归终止不需要显式判断 `size == 1`，因为 1×1 的网格一定 allSame，会自然返回叶节点

### 解法2: 前缀和 + 分治 — O(n²) / O(n²) 加分项

**从解法1优化**：解法1每次递归都要遍历子区域来判断是否全同，这是 O(size²) 的。如果用二维前缀和预处理，任意子矩形的和可以 O(1) 算出。和为 0 说明全 0，和为 size² 说明全 1，否则不全同。

```cpp
class Solution {
public:
    Node* construct(vector<vector<int>>& grid) {
        int n = grid.size();
        // prefix[i][j] = grid[0..i-1][0..j-1] 的元素和
        // 使用 1-indexed 方便边界处理
        vector<vector<int>> prefix(n + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                prefix[i][j] = grid[i-1][j-1] 
                             + prefix[i-1][j] 
                             + prefix[i][j-1] 
                             - prefix[i-1][j-1];
            }
        }
        return build(grid, prefix, 0, 0, n);
    }
    
private:
    // 查询 grid[r..r+size-1][c..c+size-1] 的元素和
    int regionSum(vector<vector<int>>& prefix, int r, int c, int size) {
        // 转换到 1-indexed
        int r1 = r + 1, c1 = c + 1;
        int r2 = r + size, c2 = c + size;
        return prefix[r2][c2] - prefix[r1-1][c2] - prefix[r2][c1-1] + prefix[r1-1][c1-1];
    }
    
    Node* build(vector<vector<int>>& grid, vector<vector<int>>& prefix, 
                int r, int c, int size) {
        int sum = regionSum(prefix, r, c, size);
        
        // O(1) 判断是否全同
        if (sum == 0) {
            return new Node(false, true);  // 全0
        }
        if (sum == size * size) {
            return new Node(true, true);   // 全1
        }
        
        // 不全同 → 四分递归
        int half = size / 2;
        return new Node(true, false,
            build(grid, prefix, r,        c,        half),
            build(grid, prefix, r,        c + half, half),
            build(grid, prefix, r + half, c,        half),
            build(grid, prefix, r + half, c + half, half)
        );
    }
};
```

**关键点**：
- 前缀和数组使用 1-indexed 避免了大量的边界判断
- 总时间 O(n²)：前缀和预处理 O(n²)，递归部分每个节点只做 O(1) 工作，四叉树最多 O(n²) 个节点

### 解法3: 先递归后合并 — O(n² log n) / O(log n)

**不同角度**：不预先检查区域是否全同，而是直接递归到 1×1（一定是叶节点），然后在回溯时尝试合并：如果四个子节点都是叶节点且值相同，就合并为一个叶节点。

```cpp
class Solution {
public:
    Node* construct(vector<vector<int>>& grid) {
        return build(grid, 0, 0, grid.size());
    }
    
private:
    Node* build(vector<vector<int>>& grid, int r, int c, int size) {
        // 递归到底：1×1 一定是叶节点
        if (size == 1) {
            return new Node(grid[r][c] == 1, true);
        }
        
        int half = size / 2;
        Node* tl = build(grid, r,        c,        half);
        Node* tr = build(grid, r,        c + half, half);
        Node* bl = build(grid, r + half, c,        half);
        Node* br = build(grid, r + half, c + half, half);
        
        // 合并：如果四个子节点都是叶节点且值相同 → 合并为一个叶节点
        if (tl->isLeaf && tr->isLeaf && bl->isLeaf && br->isLeaf
            && tl->val == tr->val && tr->val == bl->val && bl->val == br->val) {
            // 可以合并（实际面试中应该释放四个子节点的内存）
            return new Node(tl->val, true);
        }
        
        return new Node(true, false, tl, tr, bl, br);
    }
};
```

**关键点**：
- 这种写法不需要 `allSame` 函数，逻辑更统一
- 但最坏情况下会递归到所有 1×1 格子再合并，与解法1复杂度相同
- 优点是代码更简洁，且天然正确（合并条件清晰）

## 解法对比

| | 解法1: 朴素分治 | 解法2: 前缀和+分治 | 解法3: 先递归后合并 |
|---|---|---|---|
| 时间 | O(n² log n) | O(n²) | O(n² log n) 最坏 |
| 空间 | O(log n) 递归栈 | O(n²) 前缀和 | O(log n) 递归栈 |
| 判断全同的方式 | 遍历子区域 | 前缀和 O(1) 查询 | 不判断，递归后合并 |
| 代码复杂度 | 简单 | 中等（需要写前缀和） | 最简洁 |
| 适合场景 | 面试首选 | 追问优化时 | 偏好自底向上风格时 |

**关键区别**：解法1和3是纯分治，区别在于"先检查再分"还是"先分再合并"。解法2用前缀和把每次检查从 O(size²) 降到 O(1)，总体最优。

## 易错点

1. **子网格坐标计算错误**
   - ✗ `build(grid, r + half, c + half, half)` 只写了一个子网格，忘了其他三个的坐标
   - ✓ 四个子网格坐标要清晰列出：`(r,c)`, `(r,c+half)`, `(r+half,c)`, `(r+half,c+half)`

2. **前缀和索引偏移**
   - ✗ 前缀和用 0-indexed，查询时 `prefix[r+size][c+size] - prefix[r][c+size] - ...` 容易搞混
   - ✓ 前缀和用 1-indexed，grid 坐标到 prefix 坐标的映射要清晰：`prefix[i][j]` 对应 `grid[0..i-1][0..j-1]` 的和

3. **合并条件不完整（解法3）**
   - ✗ 只检查 `tl->val == tr->val == bl->val == br->val`，忘了检查四个都是 `isLeaf`
   - ✓ 必须同时满足 `isLeaf` 都为 true 且 `val` 都相同才能合并

4. **叶节点的 val 设置**
   - ✗ `new Node(grid[r][c], true)` — grid 的值是 int，Node 的 val 是 bool
   - ✓ `new Node(grid[r][c] == 1, true)` 或者在 C++ 中隐式转换也行，但显式更清晰

5. **非叶节点的 val 值**
   - 题目说非叶节点的 val 可以是任意值，不影响判题。但如果你在合并时用了 val 来判断，就不能随意设置。

## 面试追问

**Q1: 基础理解** — 你的 `allSame` 函数最坏情况下扫描整个子区域，整体时间复杂度是多少？
> O(n² log n)。递归有 log n 层，每层所有子区域的总面积是 n²，所以每层总扫描量 O(n²)。

**Q2: 优化** — 能否把判断全同的操作优化到 O(1)？
> 用二维前缀和预处理。区域和为 0 说明全 0，等于 size² 说明全 1，否则不全同。总时间降到 O(n²)。

**Q3: 变体** — 如果不预先判断是否全同，有没有别的写法？
> 先递归后合并（解法3）。递归到 1×1 为叶节点，回溯时如果四个子节点都是相同值的叶节点就合并。代码更简洁但最坏时间不变。

**Q4: 内存管理** — 解法3中合并时被丢弃的四个子节点怎么处理？
> 在 C++ 中应该 `delete` 释放内存。面试时可以提及这一点展示工程意识。实际 LeetCode 不会检查内存泄漏，但好的习惯是释放。

## 相关题型

- **558. 四叉树交集 (Logical OR of Two Binary Grids Represented as Quad-Trees)** — 复用本题的四叉树结构，在两棵已有的四叉树上做合并操作，关键是理解叶节点和非叶节点的合并规则
- **304. 二维区域和检索 (Range Sum Query 2D)** — 复用本题解法2中的二维前缀和技巧，是前缀和的模板题
- **240. 搜索二维矩阵 II** — 同样是在矩阵上利用结构性质进行分治/剪枝搜索