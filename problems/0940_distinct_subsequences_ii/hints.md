# 940. 不同的子序列 II (Distinct Subsequences II)

## 难度: Hard | 标签: String, Dynamic Programming

---

## 核心思路

计算字符串 s 中**不同非空子序列**的个数。子序列可以删除任意字符但保持相对顺序，难点在于**去重**——相同的子序列只计一次。

关键洞察：用 `endWith[c]` 表示"以字符 c 结尾的不同子序列个数"。遍历到新字符 c 时，c 可以追加到所有已有子序列后面，也可以单独成为一个子序列。直接覆盖 `endWith[c]` 就自动去重了。

## 思维链

1. **读完题第一反应**：子序列个数是 2^n（每个字符选或不选），但需要去重。直接枚举 + HashSet 会超时（n 可达 2000）。

2. **DP 计数的经典框架**：
   - `dp[i]` = 考虑前 i 个字符时，所有不同非空子序列的个数
   - 新字符 s[i] 可以：(1) 单独作为子序列 (2) 追加到所有已有子序列后面
   - 所以 `dp[i] = 2 * dp[i-1] + 1`

3. **如何去重？** 如果字符 s[i] 之前在位置 j 出现过，那么"以 s[j] 结尾的子序列"和"以 s[i] 结尾但前缀相同的子序列"会重复。需要减去这部分。

4. **更优雅的方式 → endWith 数组**：
   - 维护 `endWith[c]` = 以字符 c 结尾的不同子序列个数
   - 遍历到字符 c 时：`endWith[c] = (endWith 所有值的总和) + 1`
   - 含义：把 c 追加到所有已有子序列后面（+总和），再加上 c 自身（+1）
   - **覆盖** `endWith[c]` 就自动消除了旧的以 c 结尾的子序列计数
   - 最终答案 = 所有 `endWith[c]` 的总和

5. **为什么覆盖能去重？** 因为新的 `endWith[c]` 已经包含了所有以 c 结尾的子序列（无论 c 在什么位置出现）。旧值被覆盖，就不会重复计数。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DP + 去重（经典公式） | dp[i] = 2*dp[i-1]+1 - 重复部分 | O(n) | O(n) | ⭐⭐ 需要推导 |
| endWith 数组 | 维护以每个字符结尾的子序列数 | O(26n) = O(n) | O(26) = O(1) | ⭐⭐⭐ 优雅直觉 |

## 关键提示

1. **endWith[c] 的更新公式**：`endWith[c] = total + 1`，其中 total = sum(endWith[0..25])。含义是把 c 追加到所有已有子序列后面，再加上 c 自身作为长度为 1 的子序列。

2. **覆盖即去重**：更新 `endWith[c]` 时直接赋值（覆盖），不是累加。旧的 `endWith[c]` 被新值替代，保证不重复。

3. **答案**：sum(endWith[0..25])，即所有以某个字符结尾的不同子序列的总数。

4. **取模**：每步都要对 10^9 + 7 取模，注意减法时可能需要 + MOD 防止负数。

5. **endWith 本质上是一种"分类计数"**：将所有子序列按结尾字符分为 26 类，每类内部保证不重复（因为 endWith[c] 在 c 出现时会被完全重置）。

## 解法详解

### 解法1: endWith 数组 — O(n) / O(1) ⭐ 面试首选

```
DP 表模拟 (s = "aba"):

初始: endWith = [0, 0, ..., 0] (26个0)

Step 1: c = 'a' (idx=0)
  total = sum(endWith) = 0
  endWith[0] = 0 + 1 = 1     ← 子序列: {"a"}
  endWith = [1, 0, 0, ...]
  当前所有子序列: {"a"} → 共 1 个

Step 2: c = 'b' (idx=1)
  total = sum(endWith) = 1
  endWith[1] = 1 + 1 = 2     ← 子序列: {"b", "ab"}
  endWith = [1, 2, 0, ...]
  当前所有子序列: {"a", "b", "ab"} → 共 3 个

Step 3: c = 'a' (idx=0)
  total = sum(endWith) = 1 + 2 = 3
  endWith[0] = 3 + 1 = 4     ← 子序列: {"a", "ba", "aa", "aba"}
  endWith = [4, 2, 0, ...]           注意: 旧的 endWith[0]=1 被覆盖!
  当前所有子序列: {"a","ba","aa","aba","b","ab"} → 共 6 个

答案 = sum(endWith) = 4 + 2 = 6 ✓

去重过程详解:
  Step 1 后 endWith['a'] = 1, 代表以'a'结尾的子序列: {"a"}
  Step 3 后 endWith['a'] = 4, 代表以'a'结尾的子序列: {"a","ba","aa","aba"}
  旧的 {"a"} 被包含在新的集合中了吗?
  是的! 新的 endWith['a'] = total + 1 = 3 + 1 = 4
    → "a"本身 (+1)
    → "a"追加到{"a"} → "aa"
    → "a"追加到{"b"} → "ba"  
    → "a"追加到{"ab"} → "aba"
  所以 "a" 被重新计入了，不需要旧值，直接覆盖!
```

```
更复杂的例子 (s = "abc"):

Step 1: c='a' → endWith = [1,0,0,...] → total=1 → 子序列: {"a"}
Step 2: c='b' → endWith = [1,2,0,...] → total=3 → 子序列: {"a","b","ab"}  
Step 3: c='c' → endWith = [1,2,4,...] → total=7 → 子序列: {"a","b","ab","c","ac","bc","abc"}

答案 = 1+2+4 = 7 ✓ (与 2^3 - 1 = 7 一致，因为所有字符不同，无需去重)
```

```
去重的关键例子 (s = "aaa"):

Step 1: c='a' → total=0 → endWith[0]=1
  以'a'结尾: {"a"}

Step 2: c='a' → total=1 → endWith[0]=2      ← 覆盖! 不是累加!
  以'a'结尾: {"a", "aa"}                      ← "a" 被重新计入
  如果不覆盖而是累加: endWith[0]=1+2=3 → 会多数 "a"

Step 3: c='a' → total=2 → endWith[0]=3      ← 覆盖!
  以'a'结尾: {"a", "aa", "aaa"}

答案 = 3 ✓ (s="aaa" 的不同非空子序列就是 "a", "aa", "aaa")
```

```cpp
class Solution {
public:
    int distinctSubseqII(string s) {
        const int MOD = 1e9 + 7;
        vector<long> endWith(26, 0);  // endWith[c] = 以字符 c 结尾的不同子序列数

        for (char c : s) {
            // 计算所有已有子序列的总数
            long total = 0;
            for (int i = 0; i < 26; i++) {
                total = (total + endWith[i]) % MOD;
            }
            // 覆盖: c 追加到所有已有子序列后面 + c 自身
            endWith[c - 'a'] = (total + 1) % MOD;
        }

        // 答案 = 所有 endWith 的总和
        long ans = 0;
        for (int i = 0; i < 26; i++) {
            ans = (ans + endWith[i]) % MOD;
        }
        return ans;
    }
};
```

---

### 解法2: 经典 DP 公式 + 减法去重 — O(n) / O(n)

**另一种推导方式**：`dp[i]` = 前 i 个字符的不同非空子序列数。

```
转移: dp[i] = 2 * dp[i-1] + 1
  含义: 旧子序列 (dp[i-1]) + 旧子序列追加s[i] (dp[i-1]) + s[i]自身 (1)

去重: 如果 s[i] 之前在位置 j 出现过 (j < i, s[j] == s[i]):
  dp[i] = 2 * dp[i-1] + 1 - (dp[j-1] + 1)
  其中 dp[j-1] + 1 是重复的部分 (把s[j]追加到前j-1个字符的子序列 + s[j]自身)

为什么减 dp[j-1]+1?
  在处理 s[j] 时产生的新子序列 = dp[j-1] (追加到旧的) + 1 (自身)
  在处理 s[i] 时又产生了完全相同的子序列 (因为 s[i]==s[j])
  所以要减去这个重复量
```

```
模拟 (s = "aba"):

dp[-1] = 0 (没有字符时, 0个子序列)

i=0, c='a', 之前没出现过:
  dp[0] = 2*dp[-1] + 1 = 2*0 + 1 = 1
  子序列: {"a"}

i=1, c='b', 之前没出现过:
  dp[1] = 2*dp[0] + 1 = 2*1 + 1 = 3
  子序列: {"a", "b", "ab"}

i=2, c='a', 之前在 j=0 出现过:
  dp[2] = 2*dp[1] + 1 - (dp[j-1] + 1) = 2*3 + 1 - (dp[-1] + 1) = 7 - 1 = 6
  子序列: {"a", "b", "ab", "ba", "aa", "aba"}

答案 = dp[2] = 6 ✓
```

```cpp
class Solution2 {
public:
    int distinctSubseqII(string s) {
        const int MOD = 1e9 + 7;
        int n = s.size();
        vector<long> dp(n, 0);
        vector<int> lastOccurrence(26, -1);  // 每个字符上次出现的位置

        for (int i = 0; i < n; i++) {
            int c = s[i] - 'a';
            
            // 基础转移
            if (i == 0) {
                dp[i] = 1;
            } else {
                dp[i] = (2 * dp[i - 1] % MOD + 1) % MOD;
            }
            
            // 去重: 减去上次出现时的重复量
            if (lastOccurrence[c] != -1) {
                int j = lastOccurrence[c];
                long dup = (j == 0) ? 1 : (dp[j - 1] + 1) % MOD;
                dp[i] = (dp[i] - dup % MOD + MOD) % MOD;
            }
            
            lastOccurrence[c] = i;
        }

        return dp[n - 1];
    }
};
```

## 解法对比

| | endWith 数组 | 经典 DP + 减法 |
|---|---|---|
| 时间 | O(26n) | O(n) |
| 空间 | O(26) | O(n) |
| 代码复杂度 | 简单 | 中等 |
| 直觉清晰度 | ⭐⭐⭐ | ⭐⭐ |
| 面试推荐度 | ⭐⭐⭐ | ⭐⭐ |
| 去重方式 | 覆盖即去重（隐式） | 显式减去重复量 |

**推荐 endWith 方法**：代码更短，不需要维护 lastOccurrence，去重逻辑更自然（覆盖而非减法），不容易出取模负数问题。

## 易错点

1. **endWith 方法中用累加而不是覆盖**
   - 错: `endWith[c] += total + 1` — 旧的以 c 结尾的子序列被重复计数
   - 对: `endWith[c] = total + 1` — 覆盖旧值，自动去重
   - 这是最核心的错误，覆盖 vs 累加是去重的关键

2. **经典 DP 中减法取模产生负数**
   - 错: `dp[i] = (dp[i] - dup) % MOD` — 如果 dp[i] < dup，结果为负
   - 对: `dp[i] = (dp[i] - dup % MOD + MOD) % MOD` — 先加 MOD 再取模

3. **忘记 +1（c 自身作为子序列）**
   - 错: `endWith[c] = total` — 漏掉了 c 单独成为子序列的情况
   - 对: `endWith[c] = total + 1` — c 追加到所有已有子序列 (total) + c 自身 (1)

4. **经典 DP 中 lastOccurrence 更新时机错误**
   - 必须在减法之后再更新 `lastOccurrence[c] = i`
   - 如果先更新，lastOccurrence[c] 就是当前位置，无法找到"上一次出现的位置"

5. **答案取值位置**
   - endWith 方法: 答案是遍历结束后 sum(endWith)
   - 经典 DP: 答案是 dp[n-1]

## 面试追问

**Q1: 为什么 endWith 覆盖能去重？举个例子。**
→ 以 s="aba" 为例。第一个 'a' 时 endWith['a']=1（只有 "a"）。第二个 'a' 时 endWith['a']=4（"a", "ba", "aa", "aba"）。新值完全重新枚举了所有以 'a' 结尾的子序列，旧的 "a" 被包含在新的集合中。所以覆盖就等于"用完整的新集合替换旧集合"，自然没有重复。

**Q2: 时间复杂度是 O(26n) 还是 O(n)？**
→ 严格来说是 O(26n)，因为每次遍历 endWith 数组求 total。但 26 是常数，所以渐近意义上是 O(n)。如果用一个变量维护 total 的运行值，可以做到严格 O(n)（每次 total = total - old_endWith[c] + new_endWith[c]）。

**Q3: 这道题和 LC 115 Distinct Subsequences 有什么区别？**
→ LC 115 是"s 的子序列中有多少个等于 t"，是双串匹配问题，用二维 DP。本题是"s 的不同子序列有多少个"，是单串计数问题，用一维 DP。完全不同的问题。

**Q4: 如果字符集不是 26 个小写字母，而是 Unicode？**
→ endWith 数组改为 HashMap<char, long>。空间从 O(26) 变为 O(K)，K 为不同字符数。时间变为 O(Kn)。

## 相关题型

- [115. Distinct Subsequences](../0115_distinct_subsequences/) — 双串匹配型子序列 DP
- [392. Is Subsequence](../0392_is_subsequence/) — 子序列判断的基础题
- [1987. Number of Unique Good Subsequences](../1987_number_of_unique_good_subsequences/) — 类似的去重子序列计数
