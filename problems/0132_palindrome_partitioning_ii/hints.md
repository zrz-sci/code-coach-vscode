# 132. 分割回文串 II

## 核心思路
本质是求将字符串切割成若干回文子串所需的最少切割次数，即在预处理回文判断的基础上做一维DP求最小分割。

## 思维链
1. **暴力回溯**: 枚举所有分割方案(类似131题)，取最少切割数 → O(2^n) 指数级
2. **观察子问题**: 前i个字符的最少切割数，只依赖前j个字符(j<i)的最少切割数 → 一维DP
3. **定义状态**: dp[i] = s[0..i-1] 的最少切割次数
4. **转移方程**: 若 s[j..i-1] 是回文，则 dp[i] = min(dp[i], dp[j] + 1)，其中 j ∈ [0, i-1]
5. **优化回文判断**: 提前用中心扩展法或二维DP预处理 isPalin[i][j]
6. **进一步优化**: Manacher思想 + 中心扩展在DP过程中同步更新，省去预处理

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DP + 预处理回文 | 二维isPalin + 一维dp | O(n^2) | O(n^2) | ⭐ 必知 |
| DP + 中心扩展 | 边扩展边更新dp | O(n^2) | O(n) | ⭐ 最优 |

## 关键提示
1. **初始化**: dp[i] 初始化为 i-1 (最坏情况每个字符都切一刀)，dp[0] = -1 作为哨兵
2. **回文预处理**: isPalin[i][j] 可用 `s[i]==s[j] && (j-i<2 || isPalin[i+1][j-1])` 递推
3. **中心扩展技巧**: 对每个中心点同时处理奇数/偶数长度回文，发现回文直接更新dp
4. **哨兵妙用**: dp[0] = -1，这样当 s[0..i-1] 整体是回文时，dp[i] = dp[0]+1 = 0

```
示例: s = "aab"

dp数组变化:
初始: dp = [-1, 0, 1, 2]  (哨兵, 最坏情况)
i=1: "a" 是回文 → dp[1] = dp[0]+1 = 0
i=2: "aa" 是回文 → dp[2] = dp[0]+1 = 0
i=3: "b" 是回文 → dp[3] = dp[2]+1 = 1
     "ab" 不是回文, "aab" 不是回文
结果: dp[3] = 1
```

## 解法详解

### 解法1: DP + 二维回文预处理 (标准解法)

**思路**: 先用区间DP预处理所有子串是否回文，再用一维DP求最少切割数。两步分离，逻辑清晰。

```cpp
class Solution {
public:
    int minCut(string s) {
        int n = s.size();
        // 预处理: isPalin[i][j] 表示 s[i..j] 是否为回文
        vector<vector<bool>> isPalin(n, vector<bool>(n, false));
        for (int i = n - 1; i >= 0; i--) {
            for (int j = i; j < n; j++) {
                // 两端相等，且内部是回文(或长度<=2)
                if (s[i] == s[j] && (j - i < 2 || isPalin[i + 1][j - 1])) {
                    isPalin[i][j] = true;
                }
            }
        }
        
        // dp[i] = s[0..i-1] 的最少切割次数
        vector<int> dp(n + 1);
        dp[0] = -1; // 哨兵: 整串回文时 dp[i] = dp[0]+1 = 0
        for (int i = 1; i <= n; i++) {
            dp[i] = i - 1; // 最坏: 每个字符一刀
            for (int j = 0; j < i; j++) {
                if (isPalin[j][i - 1]) {
                    dp[i] = min(dp[i], dp[j] + 1);
                }
            }
        }
        return dp[n];
    }
};
```

**关键点**:
- 回文预处理必须从大到小枚举i，保证 isPalin[i+1][j-1] 已计算
- dp[0] = -1 是精髓，避免单独判断整串回文的特殊情况

### 解法2: DP + 中心扩展 (空间优化)

**思路**: 不单独预处理回文表，而是对每个位置作为中心向两边扩展，每发现一个回文就更新dp。这样只需O(n)空间。

```cpp
class Solution {
public:
    int minCut(string s) {
        int n = s.size();
        vector<int> dp(n + 1);
        // 初始化: dp[i] 表示 s[0..i-1] 的最少切割
        for (int i = 0; i <= n; i++) dp[i] = i - 1;
        
        for (int center = 0; center < n; center++) {
            // 奇数长度回文: 以 center 为中心
            for (int r = 0; center - r >= 0 && center + r < n 
                          && s[center - r] == s[center + r]; r++) {
                dp[center + r + 1] = min(dp[center + r + 1], dp[center - r] + 1);
            }
            // 偶数长度回文: 以 center, center+1 为中心
            for (int r = 0; center - r >= 0 && center + 1 + r < n 
                          && s[center - r] == s[center + 1 + r]; r++) {
                dp[center + 1 + r + 1] = min(dp[center + 1 + r + 1], dp[center - r] + 1);
            }
        }
        return dp[n];
    }
};
```

**关键点**:
- 中心扩展天然保证了从短回文到长回文的发现顺序
- 每发现 s[l..r] 是回文，立即用 dp[l] + 1 更新 dp[r+1]
- 空间从 O(n^2) 降到 O(n)

## 易错点

1. **dp初始值错误**
```
✗ dp[0] = 0  // 导致整串回文时 dp[n] = 1 而非 0
✓ dp[0] = -1 // 哨兵值，整串回文时 dp[n] = dp[0]+1 = 0
```

2. **回文预处理遍历方向错误**
```
✗ for (int i = 0; i < n; i++)       // isPalin[i+1][j-1] 还没算出来
      for (int j = i; j < n; j++)
✓ for (int i = n-1; i >= 0; i--)    // 从下往上，保证子问题先算
      for (int j = i; j < n; j++)
```

3. **中心扩展边界写错**
```
✗ center + r < n && center - r > 0   // 漏掉 r=center 的情况
✓ center + r < n && center - r >= 0  // 允许扩展到首字符
```

## 面试追问

**Q1**: 这道题和131题(回文分割I)有什么区别？
→ 131题求所有合法分割方案(回溯)，本题只求最少切割数(DP)。131的回溯复杂度O(2^n)，本题DP O(n^2)。

**Q2**: 如何将空间从O(n^2)优化到O(n)？
→ 不预处理回文表，改用中心扩展法。对每个位置向两边扩展，发现回文立即更新dp数组。

**Q3**: 如果要求输出最少切割的具体方案（不只是次数），怎么做？
→ 在DP过程中额外记录 `parent[i]` 表示 dp[i] 由哪个 j 转移而来，最后从 parent[n] 回溯还原切割位置。

## 相关题型
- [131. 回文分割 I](../0131_palindrome_partitioning/) — 回溯枚举所有方案，可复用回文预处理
- [5. 最长回文子串](../0005_longest_palindromic_substring/) — 中心扩展法的基础练习
- [647. 回文子串计数](../0647_palindromic_substrings/) — 中心扩展计数，同一技巧
- [1745. 分割回文串 IV](../1745_palindrome_partitioning_iv/) — 判断能否分成3个回文串
