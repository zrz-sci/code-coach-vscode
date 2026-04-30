# 302. Smallest Rectangle Enclosing Black Pixels (包含全部黑色像素的最小矩形)

**难度**: Hard | **标签**: Depth-First Search, Breadth-First Search, Array, Binary Search, Matrix

## 核心思路

利用黑色像素**连通性**的特点，对行和列分别做**二分搜索**，找到黑色区域的上/下/左/右四个边界。每次二分判断某行/列是否包含黑色像素。时间复杂度 O(m*log(n) + n*log(m))，优于暴力 O(m*n)。

## 思维链

1. 需要找包含所有黑像素的最小矩形 -> 找四个边界：top, bottom, left, right
2. 暴力 O(mn) 扫描可以，但题目要求 < O(mn)
3. 黑像素连通 -> 如果某行有黑像素，则该行在 [top, bottom] 范围内
4. 行的"有无黑像素"具有单调性（从已知黑像素位置向外扩展）
5. 用二分查找分别确定四个边界

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|------|-----------|-----------|------|
| 二分搜索四边界 | 行列分别二分 | O(m*logn + n*logm) | O(1) | ⭐ |
| DFS/BFS 遍历 | 遍历所有黑像素 | O(k), k=黑像素数 | O(k) | |
| 暴力扫描 | 遍历整个矩阵 | O(m*n) | O(1) | |

## 关键提示

```
矩阵示例:
     col: 0  1  2  3
row 0:    0  0  1  0
row 1:    0  1  1  0
row 2:    0  1  0  0

已知黑像素: (0, 2)

四个边界的二分搜索:

Top边界 (在 [0, x] 即 [0, 0] 中找第一个有黑像素的行):
  搜索 [0, 0] -> row 0 有黑像素('1') -> top = 0

Bottom边界 (在 [x, m-1] 即 [0, 2] 中找最后一个有黑像素的行):
  搜索 [1, 3) -> row 2 有黑像素 -> bottom = 2+1 = 3

Left边界 (在 [0, y] 即 [0, 2] 中找第一个有黑像素的列):
  搜索 [0, 2] -> col 1 有黑像素 -> left = 1

Right边界 (在 [y, n-1] 即 [2, 3] 中找最后一个有黑像素的列):
  搜索 [3, 4) -> col 2 有黑像素 -> right = 2+1 = 3

面积 = (bottom - top) * (right - left) = (3-0) * (3-1) = 3 * 2 = 6

二分原理图:
  行方向:
  row:  0   1   2
  有1?: Y   Y   Y
        ^           ^
       top        bottom(exclusive)

  列方向:
  col:  0   1   2   3
  有1?: N   Y   Y   N
            ^       ^
          left   right(exclusive)
```

## 解法详解

### 解法一：二分搜索四边界（推荐）

```cpp
class Solution {
public:
    int minArea(vector<vector<char>>& image, int x, int y) {
        int m = image.size(), n = image[0].size();
        
        // 找 top: [0, x] 中第一个有黑像素的行
        int top = searchFirst(image, 0, x, true);
        // 找 bottom: [x, m-1] 中最后一个有黑像素的行 +1
        int bottom = searchLast(image, x, m - 1, true) + 1;
        // 找 left: [0, y] 中第一个有黑像素的列
        int left = searchFirst(image, 0, y, false);
        // 找 right: [y, n-1] 中最后一个有黑像素的列 +1
        int right = searchLast(image, y, n - 1, false) + 1;
        
        return (bottom - top) * (right - left);
    }
    
private:
    // 找第一个包含黑像素的 行/列
    int searchFirst(vector<vector<char>>& image, int lo, int hi, bool isRow) {
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (hasBlack(image, mid, isRow)) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        return lo;
    }
    
    // 找最后一个包含黑像素的 行/列
    int searchLast(vector<vector<char>>& image, int lo, int hi, bool isRow) {
        while (lo < hi) {
            int mid = lo + (hi - lo + 1) / 2;  // 上取整防死循环
            if (hasBlack(image, mid, isRow)) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }
        return lo;
    }
    
    // 检查某行/列是否包含黑像素
    bool hasBlack(vector<vector<char>>& image, int idx, bool isRow) {
        if (isRow) {
            for (char c : image[idx])
                if (c == '1') return true;
        } else {
            for (auto& row : image)
                if (row[idx] == '1') return true;
        }
        return false;
    }
};
```

### 解法二：DFS 遍历

```cpp
class Solution {
public:
    int minArea(vector<vector<char>>& image, int x, int y) {
        int m = image.size(), n = image[0].size();
        int top = m, bottom = 0, left = n, right = 0;
        
        dfs(image, x, y, m, n, top, bottom, left, right);
        
        return (bottom - top + 1) * (right - left + 1);
    }
    
private:
    void dfs(vector<vector<char>>& image, int r, int c,
             int m, int n, int& top, int& bottom, int& left, int& right) {
        if (r < 0 || r >= m || c < 0 || c >= n || image[r][c] != '1') return;
        
        image[r][c] = '2';  // 标记已访问
        top = min(top, r);
        bottom = max(bottom, r);
        left = min(left, c);
        right = max(right, c);
        
        dfs(image, r + 1, c, m, n, top, bottom, left, right);
        dfs(image, r - 1, c, m, n, top, bottom, left, right);
        dfs(image, r, c + 1, m, n, top, bottom, left, right);
        dfs(image, r, c - 1, m, n, top, bottom, left, right);
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ searchLast 用 `mid = lo + (hi-lo)/2` | ✓ 用 `mid = lo + (hi-lo+1)/2` | 找最后一个时必须上取整，否则死循环 |
| ✗ 面积 = (bottom-top) * (right-left) 不+1 | ✓ 看边界是 inclusive 还是 exclusive | 统一用 exclusive 右边界更不易出错 |
| ✗ DFS 后没还原 image | ✓ 标记为 '2' 或用 visited 数组 | DFS 会修改原数组 |
| ✗ 二分搜索范围不含已知黑像素 | ✓ 搜索范围必须包含 x 或 y | 否则可能找不到有效边界 |

## 面试追问

**Q1: 为什么可以用二分搜索？单调性在哪里？**
> 黑像素连通，所以行方向上，从第一行有黑像素的行到最后一行有黑像素的行之间，所有行都至少有一个黑像素（投影连续性）。列同理。这个"从有到无"的分界点就是二分的目标。

**Q2: 时间复杂度详细分析？**
> 四次二分搜索，每次 O(log(m)) 或 O(log(n))。每次二分内部检查一行/列是 O(n) 或 O(m)。总计 O(m*log(n) + n*log(m))。

**Q3: 如果黑像素不连通怎么办？**
> 二分不再适用（投影可能不连续）。必须用 DFS/BFS 遍历所有黑像素，或暴力扫描，时间 O(mn)。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 200 | Number of Islands | DFS/BFS 连通区域 |
| 74 | Search a 2D Matrix | 矩阵二分搜索 |
| 240 | Search a 2D Matrix II | 矩阵搜索 |
| 1139 | Largest 1-Bordered Square | 矩阵边界问题 |
| 463 | Island Perimeter | 连通区域边界 |
