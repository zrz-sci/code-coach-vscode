# 131. 分割回文串

## 核心思路

本质上是一个**分割枚举问题**：在字符串的每个位置决定"切不切"，然后检查每一段是否是回文。所有满足条件的切法就是答案。这是一道经典的**回溯 + 回文判断**题。

## 思维链

1. **读完题第一反应**：要找出所有可能的分割方案，每种方案的每一段都必须是回文。"所有可能" → 穷举 → 回溯。

2. **怎么穷举？** 从位置 `start` 开始，尝试截取 `s[start..end]` 作为第一段（`end` 从 `start` 到 `n-1`）。如果这一段是回文，就递归处理剩余部分 `s[end+1..]`。当 `start == n` 时说明所有字符都被分完了，得到一个合法方案。

3. **瓶颈在哪？** 每次截取一段后要判断是否回文，朴素判断是 O(n)。而回溯过程中同一个子串 `s[i..j]` 可能被反复判断回文，造成重复计算。

4. **怎么优化回文判断？** 用 **DP 预处理**一张二维表 `isPalin[i][j]`，O(n²) 时间预处理后，任何子串的回文判断都是 O(1)。

5. **整体流程**：先用 DP 建回文表，再用回溯枚举所有合法分割。

6. **复杂度本质**：字符串长度最多 16，最坏情况（如 "aaaa..."）分割方案数是指数级的，但 n≤16 完全可以接受。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯 + 朴素回文判断 | 回溯枚举分割点，每次 O(n) 判回文 | O(n · 2ⁿ) | O(n) 递归栈 | 能说出即可 |
| 回溯 + DP 预处理回文表 | 先 O(n²) 建表，回文判断 O(1) | O(n · 2ⁿ) 但常数更小 | O(n²) 回文表 | ⭐ 必须写出 |
| 回溯 + 记忆化 (Manacher 等) | 更高级的回文预处理 | 同上 | 同上 | 加分项 |

> 注意：由于要输出所有方案，最坏情况方案数本身就是指数级的，所以时间复杂度的主导项来自输出，两种解法的渐进复杂度相同。DP 预处理优化的是"常数"——把每次 O(n) 的回文检查变成 O(1)。

## 关键提示

1. **回溯的决策是什么？** 在位置 `start` 处，你要决定这一"刀"切在哪里——即选择第一段的长度。每种长度对应一个分支。

2. **决策树长什么样？** 以 `s = "aab"` 为例：
   ```
   // 决策树 (从 start=0 开始，尝试不同的第一段):
   //                    start=0
   //              /         |          \
   //          "a"          "aa"        "aab"(×不回文)
   //         start=1       start=2
   //        /     \          |
   //      "a"    "ab"(×)    "b"
   //    start=2            start=3 ✓ → ["aa","b"]
   //      |
   //     "b"
   //   start=3 ✓ → ["a","a","b"]
   ```

3. **回文表怎么填？** `isPalin[i][j]` 表示 `s[i..j]` 是否回文。转移：`isPalin[i][j] = (s[i]==s[j]) && isPalin[i+1][j-1]`。注意遍历方向：`i` 从大到小，`j` 从 `i` 到 `n-1`（保证 `i+1` 在 `i` 之前已计算）。

4. **剪枝点**：只有当 `s[start..end]` 是回文时才递归。不是回文的分支直接跳过，这就是天然的剪枝。

5. **结束条件**：`start == s.size()` 时，当前路径 `path` 就是一个合法分割方案。

## 解法详解

### 解法1: 回溯 + 朴素回文判断 — O(n · 2ⁿ) / O(n)

**思考过程**：最直觉的做法——回溯枚举每种分割，每截一段就当场检查是否回文。

```cpp
class Solution {
public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> result;
        vector<string> path;
        backtrack(s, 0, path, result);
        return result;
    }
    
    void backtrack(const string& s, int start, 
                   vector<string>& path, vector<vector<string>>& result) {
        // 所有字符都分割完毕 → 收集结果
        if (start == s.size()) {
            result.push_back(path);
            return;
        }
        // 尝试从 start 截取到 end（end 从 start 到 n-1）
        for (int end = start; end < s.size(); end++) {
            // 只有当 s[start..end] 是回文时，才做选择并递归
            if (isPalindrome(s, start, end)) {
                path.push_back(s.substr(start, end - start + 1)); // 做选择
                backtrack(s, end + 1, path, result);              // 递归处理剩余
                path.pop_back();                                   // 撤销选择
            }
        }
    }
    
    // 朴素回文判断：双指针，O(n)
    bool isPalindrome(const string& s, int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) return false;
            left++;
            right--;
        }
        return true;
    }
};
```

**关键点**：
- 回溯三要素都很清晰：路径=`path`，选择列表=从 `start` 到末尾的所有回文前缀，结束条件=`start == n`
- 缺点：同一个子串可能被多次判断回文。比如 `s = "aaaa"`，`isPalindrome(0,1)` 在不同的回溯路径中会被反复调用。

### 解法2: 回溯 + DP 预处理回文表 — O(n · 2ⁿ) / O(n²) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是每次 O(n) 判回文，且有重复计算。我们用一个 O(n²) 的 DP 预处理，把所有子串的回文性一次算好，之后每次查表 O(1)。

```cpp
class Solution {
public:
    vector<vector<string>> partition(string s) {
        int n = s.size();
        // DP 预处理回文表
        // isPalin[i][j] = true 表示 s[i..j] 是回文
        vector<vector<bool>> isPalin(n, vector<bool>(n, false));
        
        // 填表：i 从大到小（因为 isPalin[i][j] 依赖 isPalin[i+1][j-1]）
        //
        // 填表方向和依赖关系:
        //        j →
        //    i  [i][j] 依赖 [i+1][j-1]（左下方）
        //    ↓       ↙
        //       [i+1][j-1]
        //
        // 所以 i 必须从下往上遍历
        for (int i = n - 1; i >= 0; i--) {
            for (int j = i; j < n; j++) {
                if (s[i] == s[j]) {
                    // 长度<=2 直接回文("a" 或 "aa")，否则看内层
                    isPalin[i][j] = (j - i <= 1) || isPalin[i + 1][j - 1];
                }
            }
        }
        
        vector<vector<string>> result;
        vector<string> path;
        backtrack(s, 0, isPalin, path, result);
        return result;
    }
    
    void backtrack(const string& s, int start,
                   const vector<vector<bool>>& isPalin,
                   vector<string>& path, vector<vector<string>>& result) {
        if (start == s.size()) {
            result.push_back(path);
            return;
        }
        for (int end = start; end < s.size(); end++) {
            if (isPalin[start][end]) {                             // O(1) 查表
                path.push_back(s.substr(start, end - start + 1));
                backtrack(s, end + 1, isPalin, path, result);
                path.pop_back();
            }
        }
    }
};
```

**关键点**：
- DP 预处理只是一个"加速器"，回溯的逻辑和解法1完全相同
- 对于 n=16 的数据规模，这个优化实际效果明显——尤其是全相同字符时回溯分支多，每条分支上的回文判断从 O(n) 降到 O(1)

### 解法3: 回溯中动态判断（中心扩展缓存）

**另一种思路**：不预处理整张表，而是在回溯过程中用记忆化的方式缓存已判断过的子串。这种方式的好处是只计算实际用到的子串，在某些输入下比全量预处理更高效。

```cpp
class Solution {
public:
    vector<vector<string>> partition(string s) {
        int n = s.size();
        // memo[i][j]: 0=未计算, 1=回文, -1=非回文
        vector<vector<int>> memo(n, vector<int>(n, 0));
        vector<vector<string>> result;
        vector<string> path;
        backtrack(s, 0, memo, path, result);
        return result;
    }
    
    void backtrack(const string& s, int start,
                   vector<vector<int>>& memo,
                   vector<string>& path, vector<vector<string>>& result) {
        if (start == s.size()) {
            result.push_back(path);
            return;
        }
        for (int end = start; end < s.size(); end++) {
            if (isPalin(s, start, end, memo)) {
                path.push_back(s.substr(start, end - start + 1));
                backtrack(s, end + 1, memo, path, result);
                path.pop_back();
            }
        }
    }
    
    bool isPalin(const string& s, int i, int j, vector<vector<int>>& memo) {
        if (i >= j) return true;
        if (memo[i][j] != 0) return memo[i][j] == 1;
        // 递归判断 + 记忆化
        bool res = (s[i] == s[j]) && isPalin(s, i + 1, j - 1, memo);
        memo[i][j] = res ? 1 : -1;
        return res;
    }
};
```

## 解法对比

| | 解法1: 朴素回文判断 | 解法2: DP 预处理 ⭐ | 解法3: 记忆化回文 |
|---|---|---|---|
| **回文判断时间** | 每次 O(n) | 预处理 O(n²)，之后 O(1) | 首次 O(n)，之后 O(1) |
| **额外空间** | O(n) 递归栈 | O(n²) 表 | O(n²) memo |
| **代码复杂度** | 最简单 | 稍复杂（需要理解 DP 填表方向） | 中等 |
| **面试推荐** | 先写这个展示理解 | 面试官追问后优化到这个 | 作为 DP 预处理的替代选项 |
| **适用场景** | 快速实现 | 回文判断非常频繁时 | 懒计算、只用到部分子串时 |

## 易错点

1. **DP 填表方向错误**：
   - ✗ `for(int i=0; i<n; i++) for(int j=i; j<n; j++)` — `isPalin[i][j]` 依赖 `isPalin[i+1][j-1]`，此时 `i+1` 还没计算
   - ✓ `for(int i=n-1; i>=0; i--) for(int j=i; j<n; j++)` — `i` 从大到小保证依赖已就绪

2. **回文判断的 base case 遗漏**：
   - ✗ `isPalin[i][j] = (s[i]==s[j]) && isPalin[i+1][j-1]` 不加长度检查 → 当 `j-i==1` 时访问 `isPalin[i+1][i]`（i+1 > i），越界或逻辑错误
   - ✓ 加条件 `(j - i <= 1) || isPalin[i+1][j-1]`：长度 1 或 2 的串只要首尾相同就是回文

3. **回溯中忘记撤销选择**：
   - ✗ 只有 `path.push_back(...)` 和递归，没有 `path.pop_back()`
   - ✓ 做选择、递归、撤销选择，三步缺一不可

4. **`substr` 参数搞错**：
   - ✗ `s.substr(start, end)` — 第二个参数是**长度**不是结束位置
   - ✓ `s.substr(start, end - start + 1)`

## 面试追问

**Q1: 这道题的回溯框架是什么？和子集/组合有什么区别？**
→ 回溯框架：在每个位置 `start`，枚举第一段的结束位置 `end`，只选回文前缀。和子集题的区别：子集是"选或不选每个元素"，这里是"选择切割位置"。更像是一个分割问题（partition），而非子集问题（subset）。

**Q2: 能否优化回文判断？O(1) 判断？**
→ 用 DP 预处理 `isPalin[i][j]` 表，O(n²) 预处理后每次查表 O(1)。或者用记忆化递归，按需计算。

**Q3: 如果题目改为"返回最少分割次数"（即 LeetCode 132），怎么做？**
→ 不再需要枚举所有方案，转为纯 DP 问题。定义 `dp[i]` = `s[0..i]` 的最少分割次数。转移：如果 `s[j..i]` 是回文，则 `dp[i] = min(dp[i], dp[j-1] + 1)`。同样需要预处理回文表。

**Q4: 如果字符串很长（n=1000），但只需要判断"是否存在至少一种合法分割"？**
→ 任何字符串都可以被分割成全部单字符（每个字符都是回文），所以答案永远是 `true`。如果加额外约束（如每段长度 ≥ 2），则变成一个 DP 可达性问题。

## 相关题型

- **132. 分割回文串 II** — 本题的 follow-up，只求最少分割次数。复用本题的 DP 回文预处理表，但主逻辑从回溯变成一维 DP。
- **5. 最长回文子串** — 复用同样的 `isPalin[i][j]` DP 表，区别是本题用它做回溯剪枝，5 题用它找最长。
- **78. 子集** — 回溯框架类似，但决策不同：子集是"选或不选"，本题是"切在哪"。
- **93. 复原 IP 地址** — 同样是分割字符串的回溯题，区别是约束条件不同（IP 段的合法性 vs 回文性）。