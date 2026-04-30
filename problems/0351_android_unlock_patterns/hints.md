# 351. 安卓系统手势解锁

## 核心思路
在 3x3 的九宫格上，枚举所有长度在 [m, n] 之间的合法解锁路径。使用回溯法（DFS），预处理每两个点之间是否存在"必须经过的中间点"，在 DFS 过程中检查中间点是否已被访问过。利用对称性优化：角点(1,3,7,9)等价，边点(2,4,6,8)等价，只需分别计算一个再乘以 4，加上中心点(5)的计数。

## 思维链
1. 看到"枚举所有合法排列" → 回溯法（DFS）
2. 九宫格只有 9 个点，最长路径 9 步 → 搜索空间有限，暴力可行
3. 关键约束："跳过"中间点时该点必须已被选择 → 需要预处理中间点关系
4. 对称性分析：
   - 4 个角点 {1,3,7,9} 对称 → 只算从 1 出发，结果 ×4
   - 4 个边点 {2,4,6,8} 对称 → 只算从 2 出发，结果 ×4
   - 1 个中心点 {5} → 单独算
5. 总方案数 = 4 * dfs(1) + 4 * dfs(2) + dfs(5)

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 回溯 + 跳跃表预处理 + 对称性优化 | O(9!) | O(9) | 面试标准做法 |
| 回溯 + 逐对计算中间点 | O(9!) | O(9) | 不需预处理，但逻辑更复杂 |

## 关键提示
1. **跳跃表 `skip[i][j]`**：表示从点 i 到点 j 必须经过的中间点。例如 `skip[1][3] = 2`，`skip[1][9] = 5`
2. **不是所有跳跃都有中间点**：如 2→9 不经过任何点的中心，所以 `skip[2][9] = 0`
3. **对称性大幅剪枝**：不需要从每个点都搜一次，只需搜 3 个代表点
4. **路径长度范围 [m, n]**：在 DFS 中，每到达一个长度 >= m 的路径就累加计数，直到长度 == n 停止

## 解法详解

### 解法一：回溯 + 跳跃表 + 对称性优化（推荐）

**思路**：预处理所有有中间点的点对关系，DFS 枚举所有合法路径。

```cpp
class Solution {
public:
    int numberOfPatterns(int m, int n) {
        // skip[i][j] = 从 i 到 j 必须经过的中间点（0 表示无需经过）
        vector<vector<int>> skip(10, vector<int>(10, 0));
        skip[1][3] = skip[3][1] = 2;
        skip[1][7] = skip[7][1] = 4;
        skip[3][9] = skip[9][3] = 6;
        skip[7][9] = skip[9][7] = 8;
        skip[1][9] = skip[9][1] = 5;
        skip[3][7] = skip[7][3] = 5;
        skip[2][8] = skip[8][2] = 5;
        skip[4][6] = skip[6][4] = 5;
        
        vector<bool> visited(10, false);
        int result = 0;
        
        // 利用对称性
        result += dfs(1, 1, m, n, skip, visited) * 4; // 角点 ×4
        result += dfs(2, 1, m, n, skip, visited) * 4; // 边点 ×4
        result += dfs(5, 1, m, n, skip, visited);      // 中心点
        
        return result;
    }
    
    int dfs(int cur, int len, int m, int n, 
            vector<vector<int>>& skip, vector<bool>& visited) {
        int count = 0;
        if (len >= m) count++;  // 当前路径长度满足下界
        if (len >= n) return count; // 已达上界，不再延伸
        
        visited[cur] = true;
        for (int next = 1; next <= 9; next++) {
            int mid = skip[cur][next];
            // 下一个点未访问 且 (无中间点 或 中间点已访问)
            if (!visited[next] && (mid == 0 || visited[mid])) {
                count += dfs(next, len + 1, m, n, skip, visited);
            }
        }
        visited[cur] = false;
        
        return count;
    }
};
```

**复杂度**：时间 O(9!)（最坏情况，但对称性优化后实际远小于此），空间 O(9)

### 解法二：不用预处理跳跃表

**思路**：在 DFS 中实时计算两点之间是否有中间点。利用坐标关系：如果两点行差和列差都是偶数，则中间点的坐标是两者的平均值。

```cpp
class Solution {
public:
    int numberOfPatterns(int m, int n) {
        vector<bool> visited(10, false);
        int result = 0;
        result += dfs(1, 1, m, n, visited) * 4;
        result += dfs(2, 1, m, n, visited) * 4;
        result += dfs(5, 1, m, n, visited);
        return result;
    }
    
    int dfs(int cur, int len, int m, int n, vector<bool>& visited) {
        int count = 0;
        if (len >= m) count++;
        if (len >= n) return count;
        
        visited[cur] = true;
        for (int next = 1; next <= 9; next++) {
            if (visited[next]) continue;
            // 计算中间点：将 1-9 映射到 (row, col)
            int r1 = (cur - 1) / 3, c1 = (cur - 1) % 3;
            int r2 = (next - 1) / 3, c2 = (next - 1) % 3;
            int mr = r1 + r2, mc = c1 + c2;
            // 如果行差和列差的和都是偶数，存在中间点
            if (mr % 2 == 0 && mc % 2 == 0) {
                int mid = (mr / 2) * 3 + (mc / 2) + 1;
                if (!visited[mid]) continue; // 中间点未访问，非法
            }
            count += dfs(next, len + 1, m, n, visited);
        }
        visited[cur] = false;
        return count;
    }
};
```

**复杂度**：同上

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 忘记 `skip[2][8] = 5` | 所有穿过中心的对角线和直线都要记录 | 共 8 对有中间点的关系 |
| `if (len == m) count++` | `if (len >= m) count++` | 路径长度在 [m,n] 范围内都算 |
| 忘记回溯 `visited[cur] = false` | DFS 结束后要恢复状态 | 否则其他分支无法使用该点 |
| 只算 4×corner + center | 还需要 4×edge | 三种代表点都不能漏 |

## 面试追问

**Q1: 对称性优化的原理是什么？为什么角点和边点各自等价？**
> 九宫格关于中心点有 4 重旋转对称性（0/90/180/270度）。角点 {1,3,7,9} 通过旋转互换，从任何角点出发的路径数都相同。边点 {2,4,6,8} 同理。中心点 5 旋转不变，单独计算。这将搜索次数从 9 次减为 3 次。

**Q2: 能否用动态规划代替回溯？**
> 可以使用状态压缩 DP：`dp[mask][last]` 表示已访问的点集为 `mask`，最后一个点是 `last` 时的路径数。状态数为 `2^9 * 9 = 4608`，非常小。但代码量更大，面试中回溯 + 对称性更易实现。

**Q3: 时间复杂度具体是多少？**
> 最坏 O(9!)，但因为约束条件（跳跃规则）和对称性优化，实际枚举的路径数远小于 9! = 362880。对于 m=1, n=9 的情况，总方案数为 389112，搜索量约为其 1/3（利用对称性）。

## 相关题型
- [37. 解数独](https://leetcode.com/problems/sudoku-solver/) - 回溯 + 约束检查
- [79. 单词搜索](https://leetcode.com/problems/word-search/) - 网格上的 DFS 回溯
- [46. 全排列](https://leetcode.com/problems/permutations/) - 回溯基础
- [52. N 皇后 II](https://leetcode.com/problems/n-queens-ii/) - 回溯 + 对称性优化
