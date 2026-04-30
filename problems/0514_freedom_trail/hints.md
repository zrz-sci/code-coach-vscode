## 核心思路

动态规划：定义 dp[i][j] 为拼完 key[i] 时 ring 的位置 j 对齐 12 点方向的最小步数。对每个 key 字符，枚举 ring 上所有匹配位置，计算顺/逆时针旋转的最小距离。

## 思维链

1. **状态定义**：dp[i][j] = 拼完 key 前 i 个字符，且 ring[j] 对齐 12 点的最小步数
2. **转移方程**：对于 key[i]，枚举 ring 上所有等于 key[i] 的位置 j，从上一步的位置 k 转移
   - dp[i][j] = min(dp[i-1][k] + min(顺时针距离, 逆时针距离) + 1)
3. **旋转距离**：顺时针 abs(j-k)，逆时针 n - abs(j-k)，取较小值
4. **初始化**：dp[0][j] = 从位置 0 到位置 j 的最小距离 + 1（仅 ring[j]==key[0] 时）
5. **答案**：min(dp[m-1][j])，其中 ring[j] == key[m-1]

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| DP（二维） | 逐字符转移 | O(m*n^2) | O(m*n) | ⭐ |
| DP + 预处理位置 | 哈希表存每个字符的位置 | O(m*n*k) | O(m*n) | ⭐ |
| 记忆化搜索 | DFS + memo | O(m*n^2) | O(m*n) | |

## 关键提示

- ring 是环形的，距离要取顺/逆时针的较小值
- 每按一次按钮也算 1 步（别忘了 +1）
- 预处理每个字符在 ring 中的所有位置，避免重复扫描
- key 中字符保证能在 ring 中找到

## 解法详解

### 解法一：DP + 位置预处理（推荐）

```cpp
class Solution {
public:
    int findRotateSteps(string ring, string key) {
        int n = ring.size(), m = key.size();
        
        // 预处理：每个字符在 ring 中的位置列表
        vector<vector<int>> pos(26);
        for (int i = 0; i < n; i++) {
            pos[ring[i] - 'a'].push_back(i);
        }
        
        // dp[i][j] = 拼完 key[0..i]，ring[j] 对齐12点的最小步数
        vector<vector<int>> dp(m, vector<int>(n, INT_MAX));
        
        // 初始化：从位置 0 出发拼 key[0]
        for (int j : pos[key[0] - 'a']) {
            int dist = min(j, n - j);  // 顺时针 vs 逆时针
            dp[0][j] = dist + 1;       // +1 是按按钮
        }
        
        // 状态转移
        for (int i = 1; i < m; i++) {
            for (int j : pos[key[i] - 'a']) {
                for (int k : pos[key[i-1] - 'a']) {
                    int dist = abs(j - k);
                    dist = min(dist, n - dist);
                    dp[i][j] = min(dp[i][j], dp[i-1][k] + dist + 1);
                }
            }
        }
        
        // 结果：拼完最后一个字符的所有可能位置中的最小值
        int ans = INT_MAX;
        for (int j : pos[key[m-1] - 'a']) {
            ans = min(ans, dp[m-1][j]);
        }
        return ans;
    }
};
```

**复杂度分析：**
- 时间：O(m * n^2) 最坏情况（所有字符相同），通常远小于此
- 空间：O(m * n)

### 解法二：记忆化搜索

```cpp
class Solution {
public:
    int findRotateSteps(string ring, string key) {
        int n = ring.size(), m = key.size();
        vector<vector<int>> pos(26);
        for (int i = 0; i < n; i++) pos[ring[i] - 'a'].push_back(i);
        
        vector<vector<int>> memo(m, vector<int>(n, -1));
        
        // dfs(i, j) = 从 ring 位置 j 开始，拼 key[i..m-1] 的最小步数
        function<int(int, int)> dfs = [&](int i, int j) -> int {
            if (i == m) return 0;
            if (memo[i][j] != -1) return memo[i][j];
            
            int res = INT_MAX;
            for (int k : pos[key[i] - 'a']) {
                int dist = abs(k - j);
                dist = min(dist, n - dist);
                res = min(res, dist + 1 + dfs(i + 1, k));
            }
            return memo[i][j] = res;
        };
        
        return dfs(0, 0);
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 只算顺时针距离 | `min(dist, n - dist)` | ring 是环形，要取两个方向的较小值 |
| 忘记 +1 按按钮 | `dist + 1` | 每拼一个字符需要按一次按钮 |
| `dp[0][j] = j + 1` | `dp[0][j] = min(j, n-j) + 1` | 初始位置到 j 也要考虑逆时针 |

## 面试追问

**Q1: 能否优化空间到 O(n)？**
> 可以。dp[i] 只依赖 dp[i-1]，用滚动数组优化为两行。

**Q2: 如果 ring 不是环形而是线性的？**
> 距离计算改为 abs(j - k)，去掉逆时针部分。

**Q3: BFS 能解这题吗？**
> 可以用 BFS（状态 = (key下标, ring当前位置)），但效率不如 DP，因为 BFS 按步数扩展，状态空间大。

## 相关题型

- [514. Freedom Trail](https://leetcode.com/problems/freedom-trail/) - 本题
- [72. Edit Distance](https://leetcode.com/problems/edit-distance/) - 经典字符串 DP
- [322. Coin Change](https://leetcode.com/problems/coin-change/) - 最小步数 DP
