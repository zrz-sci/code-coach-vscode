# 87. 扰乱字符串 (Scramble String)

## 难度: Hard | 标签: String, Dynamic Programming

---

## 核心思路

给定两个等长字符串 s1、s2，判断 s2 是否是 s1 的"扰乱字符串"。扰乱过程是一个递归的二叉树结构：在每一层可以选择一个分割点，将字符串分成两段，然后可以选择交换或不交换这两段，再递归地对每一段继续扰乱。

核心难点在于：分割点有 n-1 种选择，每次分割后又有交换/不交换两种选择，直接穷举是指数级的。需要用**区间 DP**或**记忆化递归**来复用子问题的答案。

## 思维链

1. **读完题第一反应**：这本质上是一棵二叉树。s1 可以在任意位置分割成两段，选择是否交换，然后递归。我需要判断是否存在某种分割+交换的方式使得 s1 变成 s2。

2. **暴力递归的结构**：对于 s1 的某个子串和 s2 的某个子串（长度相同），枚举分割点 k (1 <= k < len)：
   - **不交换**：s1 的前 k 个字符 scramble 匹配 s2 的前 k 个，且 s1 的后 len-k 个 scramble 匹配 s2 的后 len-k 个
   - **交换**：s1 的前 k 个字符 scramble 匹配 s2 的后 k 个，且 s1 的后 len-k 个 scramble 匹配 s2 的前 len-k 个

3. **为什么可以 DP？** 子问题可以用三个参数唯一确定：s1 的起点 i、s2 的起点 j、子串长度 len。状态空间是 O(n^3)，每个状态需要 O(n) 枚举分割点，总时间 O(n^4)。

4. **关键剪枝**：如果 s1[i..i+len-1] 和 s2[j..j+len-1] 的字符频次不同，它们不可能是扰乱关系，直接返回 false。这个剪枝能大幅减少实际计算量。

5. **两种实现方式**：
   - 记忆化递归：用 map 存 (s1_sub, s2_sub) -> bool，直觉清晰
   - 三维 DP：`dp[i][j][len]`，自底向上填表，效率更高

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 枚举所有分割+交换 | 指数级 | O(n) 栈 | 超时 |
| 记忆化递归 | 缓存子问题结果 | O(n^4) | O(n^3) | ⭐ 直觉清晰 |
| 三维 DP | dp[i][j][len] 自底向上 | O(n^4) | O(n^3) | ⭐ 面试首选 |

## 关键提示

1. **状态定义**：`dp[i][j][len]` = s1 从位置 i 开始、s2 从位置 j 开始、长度为 len 的子串是否为扰乱关系。三个维度完整刻画了一个子问题。

2. **交换时的下标推导（最易出错）**：
   - 不交换：s1[i..i+k-1] 对应 s2[j..j+k-1]，s1[i+k..i+len-1] 对应 s2[j+k..j+len-1]
   - **交换**：s1[i..i+k-1] 对应 s2[**j+len-k**..j+len-1]，s1[i+k..i+len-1] 对应 s2[j..j+len-k-1]
   - 注意是 `j+len-k` 而不是 `j+k`！

3. **DP 外层循环必须按 len 从小到大**：因为长度为 len 的子问题依赖长度 < len 的子问题。

4. **字符频次剪枝不可省**：没有剪枝会 TLE。在记忆化递归中，先检查字符频次再递归；在 DP 中，在内层循环开头先做频次检查。

5. **Base case**：`dp[i][j][1] = (s1[i] == s2[j])`，单个字符直接比较。

## 解法详解

### 解法1: 记忆化递归 — O(n^4) / O(n^3)

**思考过程**：直接按照题意递归，用 map 缓存已计算的子问题避免重复。

```
递归树示意 (s1="gr", s2="rg", len=2):

isScramble("gr", "rg")
├── k=1, 不交换: isScramble("g","r") && isScramble("r","g")
│                   → false (g != r)    不用算第二个
└── k=1, 交换:   isScramble("g","g") && isScramble("r","r")
                    → true               → true
                 → return true! ("gr" 交换后得到 "rg")
```

```cpp
class Solution1 {
public:
    unordered_map<string, bool> memo;
    
    bool isScramble(string s1, string s2) {
        if (s1 == s2) return true;
        if (s1.size() != s2.size()) return false;
        
        string key = s1 + "#" + s2;
        if (memo.count(key)) return memo[key];
        
        int n = s1.size();
        
        // 字符频次剪枝：频次不同不可能是扰乱关系
        vector<int> cnt(26, 0);
        for (int i = 0; i < n; i++) {
            cnt[s1[i] - 'a']++;
            cnt[s2[i] - 'a']--;
        }
        for (int c : cnt) {
            if (c != 0) return memo[key] = false;
        }
        
        // 枚举分割点 k: 左段长度为 k, 右段长度为 n-k
        for (int k = 1; k < n; k++) {
            // 情况1: 不交换
            if (isScramble(s1.substr(0, k), s2.substr(0, k)) &&
                isScramble(s1.substr(k), s2.substr(k)))
                return memo[key] = true;
            
            // 情况2: 交换 — s1前k个 对应 s2后k个
            if (isScramble(s1.substr(0, k), s2.substr(n - k)) &&
                isScramble(s1.substr(k), s2.substr(0, n - k)))
                return memo[key] = true;
        }
        
        return memo[key] = false;
    }
};
```

**优缺点**：思路直觉，代码清晰，但 substr 产生大量临时字符串，常数较大。面试中推荐先讲这个思路，再改写为 DP。

---

### 解法2: 三维 DP — O(n^4) / O(n^3) ⭐ 面试首选

**从递归到 DP**：把递归的三个参数 (i, j, len) 直接变成三维数组。

**状态定义**：`dp[i][j][len]` = s1[i..i+len-1] 和 s2[j..j+len-1] 是否为扰乱关系

**状态转移**：
```
dp[i][j][len] = OR over k from 1 to len-1:
  (dp[i][j][k] && dp[i+k][j+k][len-k])          // 不交换
  OR
  (dp[i][j+len-k][k] && dp[i+k][j][len-k])      // 交换
```

```
DP 填表方向与含义示意 (s1="ab", s2="ba"):

dp[i][j][len] 三维表，按 len 从小到大填:

len=1 (base case): 单字符比较
  dp[0][0][1] = (s1[0]=='b'?) = ('a'=='b') = false
  dp[0][1][1] = (s1[0]=='a'?) = ('a'=='a') = true   ← s1的'a'对应s2的'a'
  dp[1][0][1] = (s1[1]=='b'?) = ('b'=='b') = true   ← s1的'b'对应s2的'b'  
  dp[1][1][1] = (s1[1]=='a'?) = ('b'=='a') = false

len=2: 枚举分割点 k=1
  dp[0][0][2]:
    不交换: dp[0][0][1] && dp[1][1][1] = false && false = false
    交换:   dp[0][1][1] && dp[1][0][1] = true && true  = true ✓
    → dp[0][0][2] = true  (s1="ab" 和 s2="ba" 是扰乱关系!)

更复杂的例子 (s1="great", s2="rgeat", n=5):

len=1: 逐个字符比较 (5x5 矩阵中对角线处为 true)
len=2: 考虑长度2的子串对...
len=3: 考虑长度3的子串对...
len=4: 考虑长度4的子串对...
len=5: dp[0][0][5] = 最终答案 = true
```

```cpp
class Solution2 {
public:
    bool isScramble(string s1, string s2) {
        int n = s1.size();
        if (n != (int)s2.size()) return false;
        if (s1 == s2) return true;
        
        // dp[i][j][len]: s1[i..i+len-1] 和 s2[j..j+len-1] 是否扰乱
        vector<vector<vector<bool>>> dp(n, vector<vector<bool>>(n, vector<bool>(n + 1, false)));
        
        // Base case: 长度为1，单字符直接比较
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                dp[i][j][1] = (s1[i] == s2[j]);
        
        // 按长度从小到大填表
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                for (int j = 0; j <= n - len; j++) {
                    // 剪枝：字符频次不同直接跳过
                    vector<int> cnt(26, 0);
                    for (int p = 0; p < len; p++) {
                        cnt[s1[i + p] - 'a']++;
                        cnt[s2[j + p] - 'a']--;
                    }
                    bool same = true;
                    for (int c : cnt) if (c != 0) { same = false; break; }
                    if (!same) continue;
                    
                    // 枚举分割点
                    for (int k = 1; k < len; k++) {
                        // 不交换: s1左k对应s2左k, s1右对应s2右
                        if (dp[i][j][k] && dp[i + k][j + k][len - k]) {
                            dp[i][j][len] = true;
                            break;
                        }
                        // 交换: s1左k对应s2右k, s1右对应s2左
                        if (dp[i][j + len - k][k] && dp[i + k][j][len - k]) {
                            dp[i][j][len] = true;
                            break;
                        }
                    }
                }
            }
        }
        
        return dp[0][0][n];
    }
};
```

## 解法对比

| | 记忆化递归 | 三维 DP |
|---|---|---|
| 时间 | O(n^4) | O(n^4) |
| 空间 | O(n^3) + 字符串开销 | O(n^3) |
| 代码复杂度 | 中等 | 中等 |
| 常数因子 | 较大(substr) | 较小 |
| 面试推荐度 | ⭐⭐ 讲思路 | ⭐⭐⭐ 写代码 |
| 核心优势 | 自顶向下，只算需要的 | 自底向上，无递归开销 |

## 易错点

1. **交换情况下 s2 的起点写错**
   - 错: `dp[i][j+k][k] && dp[i+k][j][len-k]`
   - 对: `dp[i][j+len-k][k] && dp[i+k][j][len-k]`
   - 交换后 s1 的前 k 个对应 s2 的**后** k 个，起点是 `j+len-k`，不是 `j+k`

2. **DP 外层循环方向**
   - 错: 先枚举 i, j 再枚举 len
   - 对: 最外层枚举 len 从 2 到 n
   - 因为 dp[...][...][k] 和 dp[...][...][len-k] 都比当前 len 小，必须先算

3. **字符频次剪枝遗漏导致 TLE**
   - 虽然时间复杂度都是 O(n^4)，但实际中没有剪枝会超时
   - 在 DP 的 (i, j, len) 确定后、枚举 k 之前，先检查字符频次

4. **记忆化递归中 key 设计不当**
   - 用下标 (i, j, len) 做 key 比 substr 做 key 效率更高
   - 如果用 substr 做 key，注意加分隔符避免歧义

## 面试追问

**Q1: 时间复杂度怎么分析？**
→ 状态有 O(n^3) 个（i, j 各 n 种，len 有 n 种），每个状态需要 O(n) 枚举分割点 k，总时间 O(n^4)。空间 O(n^3) 存 DP 表。

**Q2: 能不能进一步优化？**
→ 字符频次剪枝在实际中效果非常好。理论上 O(n^4) 是已知的最优多项式时间上界。n <= 30 完全够用。

**Q3: 记忆化递归 vs DP 哪个更好？**
→ 记忆化递归只计算实际需要的子问题，可能比 DP 快（DP 会填满整个表）。但 DP 没有递归栈开销，常数更小。面试中推荐先讲递归思路，再写 DP 代码。

**Q4: 如果不加字符频次剪枝，实际会怎样？**
→ n=30 时 O(n^4) = 810000，加上字符串操作的常数，可能刚好超时。字符频次检查 O(n) 可以快速排除大量不可能的子问题，实测提速 10-100 倍。

## 相关题型

- [96. Unique Binary Search Trees](../0096_unique_binary_search_trees/) — 区间 DP，枚举"根"类似于枚举分割点
- [131. Palindrome Partitioning](../0131_palindrome_partitioning/) — 区间型递归拆分
- [241. Different Ways to Add Parentheses](../0241_different_ways_to_add_parentheses/) — 枚举分割点，分治递归，结构最相似
