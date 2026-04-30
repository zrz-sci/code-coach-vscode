# 902. 最大为 N 的数字组合

## 核心思路

给定一组数字 digits（1-9 中的子集），可以重复使用，问能组成多少个 <= n 的正整数。本质是一道**数位 DP / 数学计数**问题，需要分两部分计算：(1) 位数比 n 少的数全部合法；(2) 位数和 n 相同的数需要逐位分析上界约束。

关键洞察：当我们构造的数字位数比 n 少时，每一位都可以自由选择 digits 中的任何数字，没有上界限制。只有当位数与 n 相同时，才需要逐位考虑"不超过 n"的约束。

## 思维链

1. **读完题第一反应**：这是一个计数问题 — 用给定数字集合能组成多少个 <= n 的正整数。暴力枚举所有可能的数显然不行（n 可达 10^9）。

2. **分解问题**：按照组成的数字的**位数**来分类计数。
   - 假设 n 有 len 位，digits 集合大小为 d
   - 位数为 1 的合法数：d 个（digits 中每个数字都行）
   - 位数为 2 的合法数：d^2 个（每一位 d 种选择）
   - 位数为 k (k < len) 的合法数：d^k 个
   - 总计：d + d^2 + ... + d^(len-1)

3. **位数等于 len 的情况是难点**：这时必须保证组成的数 <= n。需要逐位从高到低分析。

4. **逐位分析的核心逻辑**：
   - 对于第 i 位（从高到低），n 的第 i 位是 S[i]
   - 如果我选的数字 < S[i]：后面的位可以任意填，贡献 d^(len-i-1)
   - 如果我选的数字 == S[i]：继续约束下一位
   - 如果我选的数字 > S[i]：不合法，跳过
   - 如果某一位找不到 == S[i] 的数字：后面不可能恰好匹配了，停止

5. **边界处理**：如果所有 len 位都恰好匹配，说明 n 本身也可以用 digits 组成，结果 +1。

6. **面试最佳策略**：先讲清"短位数全算 + 等长位数逐位分析"的思路框架，再手写代码。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 数位分析（数学） | 分短位数 + 等长逐位 | O(log n * d) | O(log n) | ⭐ 必须写出 |
| 数位 DP（模板） | dp[pos][tight] 逐位决策 | O(log n * d) | O(log n) | 加分项 |

## 关键提示

1. **短位数的计数是等比数列**：d + d^2 + ... + d^(len-1) = d * (d^(len-1) - 1) / (d - 1)，但直接用循环累加更不容易出错。

2. **逐位分析时的"严格小于"与"等于"**：对于每一位，先统计 digits 中有多少个数字严格小于 S[i]（设为 cnt），贡献 cnt * d^(remaining)。然后检查是否有数字恰好等于 S[i]，如果有才继续下一位。

3. **digits 不包含 0**：题目保证 digits 中的数字是 1-9，所以不用担心前导零问题。

4. **"所有位都匹配"的 +1**：如果循环正常结束（没有提前 break），说明 n 本身可以被组成，别忘了最后 +1。

5. **digits 已排序**：题目保证 digits 非递减排序，可以用二分查找优化 "小于 S[i] 的数字个数" 的计算。

## 解法详解

### 解法1: 数位分析（数学法） ⭐

**思考过程**：

以 digits = ["1","3","5","7"], n = 100 为例：
- n = "100"，len = 3，d = 4
- Part 1: 位数 < 3 的数
  - 1 位数：4 个 (1,3,5,7)
  - 2 位数：4^2 = 16 个 (11,13,...,77)
  - 小计：4 + 16 = 20
- Part 2: 位数 = 3 的数
  - 第 0 位：n 的第 0 位是 '1'
    - digits 中 < '1' 的：0 个 → 贡献 0 * 4^2 = 0
    - digits 中 == '1' 的：有 '1'，继续
  - 第 1 位：n 的第 1 位是 '0'
    - digits 中 < '0' 的：0 个（digits 最小是 '1'）→ 贡献 0
    - digits 中 == '0' 的：没有！→ break，停止
  - Part 2 贡献 = 0
- 总计：20 + 0 = 20

**DP 表/计算过程图解**：

```
n = 100, digits = [1,3,5,7], d = 4

Part 1: 位数 < len(n) = 3 的数
  1位: d^1 = 4     → {1, 3, 5, 7}
  2位: d^2 = 16    → {11, 13, 15, 17, 31, 33, ..., 77}
  小计: 20

Part 2: 位数 == 3 的数, 逐位比较 n = "100"
  位 0: S[0]='1'
    digits < '1': 无 → 贡献 0 * 4^2 = 0
    digits == '1': 有 → 继续下一位
  位 1: S[1]='0'
    digits < '0': 无 → 贡献 0 * 4^1 = 0
    digits == '0': 无 → BREAK! 不可能匹配
  Part 2 贡献: 0

总计: 20 + 0 = 20
```

另一个例子：digits = ["1","4","9"], n = 1000000000
```
n = "1000000000", len = 10, d = 3

Part 1: 位数 1~9
  1位: 3^1 = 3
  2位: 3^2 = 9
  3位: 3^3 = 27
  ...
  9位: 3^9 = 19683
  小计: 3 + 9 + 27 + 81 + 243 + 729 + 2187 + 6561 + 19683 = 29523

Part 2: 位数 == 10, 比较 "1000000000"
  位 0: S[0]='1', digits中<'1': 无, =='1': 有 → 继续
  位 1: S[1]='0', digits中<'0': 无, =='0': 无 → BREAK
  Part 2 贡献: 0

总计: 29523
```

```cpp
class Solution {
public:
    int atMostNGivenDigitSet(vector<string>& digits, int n) {
        string S = to_string(n);
        int len = S.size();
        int d = digits.size();
        int ans = 0;
        
        // Part 1: 位数比 n 短的数，每位都可以自由选择
        // 1位: d 个, 2位: d^2 个, ..., (len-1)位: d^(len-1) 个
        for (int i = 1; i < len; i++) {
            int power = 1;
            for (int j = 0; j < i; j++) power *= d;
            ans += power;
        }
        
        // Part 2: 位数和 n 相同的数，逐位分析
        for (int i = 0; i < len; i++) {
            bool hasSame = false;
            for (const string& dig : digits) {
                if (dig[0] < S[i]) {
                    // 这一位放比 S[i] 小的数字，后续位任意选
                    int power = 1;
                    for (int j = 0; j < len - i - 1; j++) power *= d;
                    ans += power;
                } else if (dig[0] == S[i]) {
                    hasSame = true;
                }
            }
            // 如果当前位没有匹配的数字，无法继续
            if (!hasSame) return ans;
        }
        
        // 所有位都恰好匹配 → n 本身也合法
        return ans + 1;
    }
};
```

**复杂度分析**：
- 时间：O(log n * d)，log n 是 n 的位数，d 是 digits 大小
- 空间：O(log n)，将 n 转为字符串

---

### 解法2: 数位 DP（模板法）

**思考过程**：这是数位 DP 的经典题型。定义 `dp(pos, tight)` 表示"从第 pos 位开始，之前是否贴着上界"时能组成的数的个数。

```cpp
class Solution {
public:
    int atMostNGivenDigitSet(vector<string>& digits, int n) {
        string S = to_string(n);
        int len = S.size();
        int d = digits.size();
        
        // dp[i] = 从第 i 位开始，且前面已经"贴着上界"，能组成多少合法数
        // 从后往前填 dp
        vector<int> dp(len + 1, 0);
        dp[len] = 1; // 所有位都填完了，算一个合法数
        
        // 从最低位向最高位计算
        for (int i = len - 1; i >= 0; i--) {
            for (const string& dig : digits) {
                if (dig[0] < S[i]) {
                    // 当前位放比上界小的数字，后面全自由
                    int power = 1;
                    for (int j = 0; j < len - i - 1; j++) power *= d;
                    dp[i] += power;
                } else if (dig[0] == S[i]) {
                    // 当前位贴着上界，后面继续受约束
                    dp[i] += dp[i + 1];
                }
                // dig[0] > S[i]: 不合法，跳过
            }
        }
        
        // dp[0] 是位数 == len 且 <= n 的数的个数
        // 还要加上位数 < len 的所有数
        int ans = dp[0];
        for (int i = 1; i < len; i++) {
            int power = 1;
            for (int j = 0; j < i; j++) power *= d;
            ans += power;
        }
        
        return ans;
    }
};
```

**DP 表填写过程**（以 digits = [1,3,5,7], n = 100 为例）：

```
S = "100", len = 3, d = 4

从后往前:
dp[3] = 1 (base case)

dp[2]: S[2] = '0'
  dig='1': '1' > '0' → 跳过
  dig='3': '3' > '0' → 跳过
  dig='5': '5' > '0' → 跳过
  dig='7': '7' > '0' → 跳过
  dp[2] = 0

dp[1]: S[1] = '0'
  所有 digit > '0' → 全跳过
  dp[1] = 0

dp[0]: S[0] = '1'
  dig='1': '1'=='1' → dp[0] += dp[1] = 0
  dig='3': '3'>'1' → 跳过
  dp[0] = 0

等长部分: 0
短位数: d^1 + d^2 = 4 + 16 = 20
总计: 20
```

## 解法对比

| 维度 | 数位分析 | 数位 DP |
|------|---------|---------|
| 代码长度 | 较短 | 较长 |
| 思路直观性 | 直观分段 | 统一框架 |
| 扩展性 | 特定于此题 | 通用模板 |
| 面试推荐 | 首选 | 通用数位 DP 场景 |

## 易错点

1. **忘记 +1（n 本身合法时）**
   ```cpp
   // 错误：所有位都匹配后没有加 1
   for (int i = 0; i < len; i++) {
       // ... 逐位分析 ...
       if (!hasSame) return ans;
   }
   return ans; // 应该是 return ans + 1;
   ```

2. **短位数计数的起始值**
   ```cpp
   // 错误：从 i=0 开始，但 0 位数没有意义（正整数不包括 0）
   for (int i = 0; i < len; i++) { // 应该从 i=1 开始
       ans += pow(d, i);
   }
   ```

3. **幂运算溢出**
   ```cpp
   // 错误：使用 pow 函数返回 double，大数时精度丢失
   ans += (int)pow(d, i); // 可能有浮点误差
   // 应该用整数循环乘法
   int power = 1;
   for (int j = 0; j < i; j++) power *= d;
   ```

4. **混淆"严格小于"和"小于等于"**
   ```cpp
   // 错误：统计的是 dig[0] <= S[i]，应该分 < 和 == 两种情况
   if (dig[0] <= S[i]) {
       // 这样会把 == 的情况也算成"后续位任意"，多算了
   }
   ```

5. **忘记 digits 是字符串数组**
   ```cpp
   // 错误：直接用 digits[j] 比较，但 digits 是 vector<string>
   if (digits[j] < S[i]) // 这是字符串和字符的比较，可能有问题
   // 正确：用 digits[j][0] 取出字符
   if (digits[j][0] < S[i])
   ```

## 面试追问

### 追问链1: 深入理解

**Q1: 为什么短位数部分不需要考虑上界？**

因为一个 k 位数（k < len）的最大值一定小于 n（一个 len 位数）。比如 n = 100 是 3 位数，任何 1-2 位数都 < 100。所以短位数的每一位都可以任选 digits 中的数字。

**Q2: 如果 digits 包含 '0' 会怎样？**

题目保证不包含 '0'，但如果包含：
- 短位数计数时，第一位不能选 '0'（前导零），所以 1 位数有 d-1 个，k 位数有 (d-1) * d^(k-1) 个
- 等长部分逻辑不变

**Q3: 时间复杂度为什么是 O(log n * d) 而不是 O(log n * d * log n)？**

虽然内层有幂运算循环，但可以预计算 powers 数组避免重复计算。即使不预计算，每次幂运算最多 O(log n) 步，总共 O(log n * d * log n)。预计算后是严格的 O(log n * d)。

### 追问链2: 变体

**Q4: 如果要求 digits 中的数字最多用 k 次怎么办？**

变成有限重复的排列计数，不能简单用 d^i 了。需要用组合数学或回溯 + 记忆化。

**Q5: 如果 n 可以非常大（比如 10^18）？**

当前算法仍然适用，因为时间和 n 的位数成正比，10^18 只有 19 位。但要注意整数溢出，可能需要用 long long。

### 追问链3: 数位 DP 通用模板

**Q6: 数位 DP 的通用框架是什么？**

```
dfs(pos, tight, state):
  if pos == len: return base_case
  if memo[pos][tight][state]: return cached
  limit = S[pos] if tight else 9
  for d in 0..limit:
    new_tight = tight && (d == limit)
    result += dfs(pos+1, new_tight, new_state)
  memo[pos][tight][state] = result
  return result
```

tight 参数表示前面的位是否都贴着上界，决定当前位的选择范围。

## 相关题型

| 题目 | 关系 |
|------|------|
| [233. 数字1的个数](../0233_number_of_digit_one/hints.md) | 同类：数位分析 |
| [357. 统计各位数字都不同的数字个数](../0357_count_numbers_with_unique_digits/hints.md) | 同类：数位计数 |
| [600. 不含连续1的非负整数](../0600_non_negative_integers_without_consecutive_ones/hints.md) | 同类：数位 DP |
| [1012. 至少有1位重复的数字](../1012_numbers_with_repeated_digits/hints.md) | 同类：数位 DP 计数 |
| [2376. 统计特殊整数](../2376_count_special_integers/hints.md) | 同类：数位 DP |
