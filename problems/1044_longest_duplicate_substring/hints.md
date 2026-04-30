# 1044. 最长重复子串 (Longest Duplicate Substring)

## 难度: Hard | 标签: String, Binary Search, Suffix Array, Sliding Window, Hash Function, Rolling Hash

---

## 题目理解

给定字符串 `s`，找到**最长的重复子串**（即至少出现 2 次的连续子串），返回其中任意一个。若不存在，返回空串。

关键点：
- **子串** = 连续字符（非子序列）
- **重复** = 出现至少 2 次（可以重叠）
- 返回**任意一个**最长重复子串即可
- 字符串长度可达 3 * 10^4，暴力 O(n^3) 不可行

---

## 思维链

```
1. 读完题 --> 找最长重复子串
   暴力思路: 枚举所有子串对，比较是否相同 --> O(n^3) 太慢

2. 观察: "答案长度"具有单调性!
   - 如果存在长度为 L 的重复子串，那么一定存在长度为 L-1 的重复子串
     (取前 L-1 个字符即可)
   - 如果不存在长度为 L 的重复子串，那么长度 > L 的也不存在
   --> 可以二分搜索答案长度!

3. 二分什么?
   - 二分长度 L，范围 [0, n-1]
   - check(L): 是否存在长度为 L 的重复子串?
   - 如果 check(L) = true，尝试更大的 L
   - 如果 check(L) = false，缩小 L

4. 如何高效实现 check(L)?
   - 方法A: 暴力比较所有长度为 L 的子串 --> O(n * L) 太慢
   - 方法B: 用 HashSet 存所有长度为 L 的子串 --> 存字符串 O(n * L) 还是慢
   - 方法C: Rabin-Karp 滚动哈希! --> O(n) 时间 check

5. Rabin-Karp 滚动哈希的原理:
   - 将子串看作一个 base 进制的数
   - s[i..i+L-1] 的哈希值 = s[i]*base^(L-1) + s[i+1]*base^(L-2) + ... + s[i+L-1]
   - 滑动窗口:
     hash(s[i+1..i+L]) = (hash(s[i..i+L-1]) - s[i]*base^(L-1)) * base + s[i+L]
   - 每次只需 O(1) 时间计算下一个子串的哈希值

6. 哈希冲突处理:
   - 用大质数取模减少冲突
   - 冲突时需要逐字符验证 (或者用双哈希)
   - 实战中单哈希 + 大模数通常够用

7. 总体复杂度: O(n log n)
   - 二分 O(log n) 轮
   - 每轮 Rabin-Karp O(n) 检查
```

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 特点 |
|------|------|------|------|------|
| **二分 + Rabin-Karp** | 二分长度 + 滚动哈希判重 | O(n log n) | O(n) | 推荐，面试高频 |
| 后缀数组 + LCP | 建后缀数组，求相邻后缀最长公共前缀 | O(n log n) 或 O(n) | O(n) | 竞赛向，实现复杂 |

---

## 解法一: 二分 + Rabin-Karp 滚动哈希 -- O(n log n) / O(n) 【推荐】

### 核心思路

```
二分搜索答案长度 L:
  lo = 0, hi = n - 1
  每轮 mid = (lo + hi + 1) / 2
  
  check(mid):
    用 Rabin-Karp 计算所有长度为 mid 的子串的哈希值
    将哈希值存入 HashSet
    若某个哈希值已存在 -> 找到重复子串，返回起始位置
    否则不存在

二分的方向:
  check(mid) = true  --> lo = mid (尝试更长)
  check(mid) = false --> hi = mid - 1 (缩短)
```

### Rabin-Karp 滚动哈希详解

```
给定字符串 s = "banana"，假设 base = 31

字符映射: a=1, b=2, c=3, ..., z=26 (从 1 开始，避免前导零)

计算子串哈希值 (不取模简化演示):
  s = "banana"
  检查 L = 3 的子串

  子串 "ban": hash = 2*31^2 + 1*31 + 14 = 2*961 + 31 + 14 = 1967

  子串 "ana": 滚动更新
    hash = (1967 - 2*961) * 31 + 14
         = (1967 - 1922) * 31 + 14
         = 45 * 31 + 14 = 1409

  子串 "nan": 滚动更新
    hash = (1409 - 1*961) * 31 + 1
         = 448 * 31 + 1 = 13889

  子串 "ana": 滚动更新
    hash = (13889 - 14*961) * 31 + 1
         = (13889 - 13454) * 31 + 1
         = 435 * 31 + 1 = 13486

  等等... 这里简化演示有误差，实际运算中用取模。
  关键点: 第二个 "ana" 的哈希和第一个一样 -> 找到重复!

滚动公式 (一般形式):
  hash_new = (hash_old - s[i] * base^(L-1)) * base + s[i+L]
  所有运算 mod 大质数

关键预计算:
  power = base^(L-1) mod p   (用循环计算)
```

### 图解: 二分搜索过程

```
s = "banana"  n = 6

Step 1: 二分搜索答案长度
  lo=1, hi=5

  Round 1: mid = 3
    check(3): 所有长度为 3 的子串:
      "ban" "ana" "nan" "ana"
                        ^^^
      "ana" 出现了 2 次! --> check = true
    lo = 4

  Round 2: mid = 4
    check(4): 所有长度为 4 的子串:
      "bana" "anan" "nana"
      三个都不同 --> check = false
    hi = 3

  lo=4 > hi=3, 结束
  答案长度 = 3, 子串 = "ana"


Step 2: check(L=3) 内部 Rabin-Karp 详细流程

  s = b  a  n  a  n  a
  idx 0  1  2  3  4  5

  base = 31, mod = 大质数 M

  power = 31^2 = 961 (mod M)

  +-----------+------------------+-------------------+
  | 窗口      | 哈希计算          | seen 集合状态      |
  +-----------+------------------+-------------------+
  | s[0..2]   | 初始计算 "ban"    | {h_ban: [0]}      |
  |   "ban"   | h = 2*961+1*31+14|                   |
  +-----------+------------------+-------------------+
  | s[1..3]   | 滑动: 去 b 加 a   | {h_ban:[0],       |
  |   "ana"   | h_ana             |  h_ana:[1]}       |
  +-----------+------------------+-------------------+
  | s[2..4]   | 滑动: 去 a 加 n   | {...,             |
  |   "nan"   | h_nan             |  h_nan:[2]}       |
  +-----------+------------------+-------------------+
  | s[3..5]   | 滑动: 去 n 加 a   | h_ana 已存在!      |
  |   "ana"   | h_ana (重复!)     | 验证 s[1..3]==s[3..5]|
  |           |                  | "ana"=="ana" YES! |
  +-----------+------------------+-------------------+

  返回 start = 3 (或 1，取决于实现)
```

### 关于模数和冲突

```
为什么需要大模数?
  - 哈希值要在 [0, mod) 范围内
  - 模数越大，冲突概率越小
  - 常用: mod = 10^9 + 7, 或 (1LL << 61) - 1 (梅森素数)

冲突怎么办?
  方法1: 发现哈希相同时，逐字符比较确认 (实战推荐)
  方法2: 双哈希 (两个不同的 base 和 mod，同时匹配才算相同)
  方法3: 用 unordered_set<string_view> (但 O(L) 插入)

base 的选择:
  - 通常取 26 或 31 (大于字符集大小的质数)
  - 避免用太小的 base (冲突多)
```

### 代码

```cpp
class Solution {
public:
    string longestDupSubstring(string s) {
        int n = s.size();
        long long mod = (1LL << 61) - 1; // 梅森素数
        long long base = 31;

        int lo = 1, hi = n - 1;
        int ansStart = -1, ansLen = 0;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            int start = check(s, mid, base, mod);
            if (start != -1) {
                ansStart = start;
                ansLen = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        return ansLen == 0 ? "" : s.substr(ansStart, ansLen);
    }

private:
    long long mul(long long a, long long b, long long mod) {
        return (__int128)a * b % mod;
    }

    int check(const string& s, int L, long long base, long long mod) {
        if (L == 0) return -1;
        int n = s.size();
        long long power = 1;
        for (int i = 0; i < L - 1; i++)
            power = mul(power, base, mod);

        long long hash = 0;
        for (int i = 0; i < L; i++)
            hash = (mul(hash, base, mod) + (s[i] - 'a' + 1)) % mod;

        unordered_map<long long, vector<int>> seen;
        seen[hash].push_back(0);

        for (int i = 1; i + L - 1 < n; i++) {
            hash = (hash - mul(s[i-1] - 'a' + 1, power, mod) % mod + mod) % mod;
            hash = (mul(hash, base, mod) + (s[i + L - 1] - 'a' + 1)) % mod;
            if (seen.count(hash)) {
                for (int prev : seen[hash])
                    if (s.substr(prev, L) == s.substr(i, L))
                        return i;
            }
            seen[hash].push_back(i);
        }
        return -1;
    }
};
```

---

## 解法二: 后缀数组 + LCP -- O(n log^2 n) / O(n) (竞赛向)

### 核心思路

```
后缀数组方法:
1. 构建后缀数组 SA: 将所有后缀排序
2. 构建 LCP (Height) 数组: 相邻排序后缀的最长公共前缀
3. LCP 数组的最大值就是最长重复子串的长度

例子: s = "banana"
后缀排序:
  a       (idx 5)
  ana     (idx 3)
  anana   (idx 1)
  banana  (idx 0)
  na      (idx 4)
  nana    (idx 2)

LCP (相邻后缀的最长公共前缀):
  a      vs ana     --> 1
  ana    vs anana   --> 3  <-- 最大!
  anana  vs banana  --> 0
  banana vs na      --> 0
  na     vs nana    --> 2

最长重复子串长度 = max(LCP) = 3, 子串 = "ana"
```

---

## 关键提示序列（渐进式）

| # | 提示 | 思路方向 |
|---|------|----------|
| 1 | 如果存在长度为 L 的重复子串，长度 L-1 的呢？ | 单调性 -> 可以二分 |
| 2 | 二分什么？怎么 check？ | 二分长度，check 该长度是否存在重复 |
| 3 | check(L) 中如何高效判断两个子串相同？ | 哈希比较 O(1) vs 逐字符 O(L) |
| 4 | 如何 O(1) 从前一个子串的哈希推出下一个？ | 滚动哈希: 去头加尾 |
| 5 | 哈希冲突怎么处理？ | 二次验证 or 双哈希 |
| 6 | base 和 mod 怎么选？ | base > 26, mod 用大质数 (梅森素数最佳) |

---

## 易错点

1. **二分的边界**
   - `lo = 1, hi = n - 1`（长度范围 [1, n-1]）
   - 长度为 0 是平凡情况（空串）
   - 长度为 n 不可能重复（整个字符串只有一份）

2. **滚动哈希的取模溢出**
   - `hash * base` 可能溢出 long long
   - 解决方案1: 用 `__int128` 做中间乘法
   - 解决方案2: 用梅森素数 `(1LL << 61) - 1` 可以用位运算优化
   - 解决方案3: 用两个较小的模数做双哈希

3. **负数取模**
   - `(hash - x * power) % mod` 可能为负
   - 必须 `+ mod` 再 `% mod` 确保非负
   - 这是滚动哈希最常见的 bug

4. **字符映射**
   - 用 `s[i] - 'a' + 1`（从 1 开始）而非 `s[i] - 'a'`（从 0 开始）
   - 从 0 开始会导致前导零问题："a" 和 "aa" 的哈希都含有 0 因子

5. **哈希冲突导致 WA**
   - 单哈希在某些测试用例上可能冲突
   - 安全做法: 哈希相同时逐字符验证
   - 或使用双哈希彻底消除冲突

---

## 复杂度分析

### 解法一（二分 + Rabin-Karp）
- **时间: O(n log n)** -- 二分 O(log n) 轮，每轮 O(n) 哈希检查
- **空间: O(n)** -- 哈希集合存储

### 解法二（后缀数组 + LCP）
- **时间: O(n log n)** (倍增) 或 O(n) (SA-IS)
- **空间: O(n)** -- 后缀数组和 LCP 数组

---

## Rabin-Karp 算法知识卡片

```
Rabin-Karp 是字符串哈希的经典算法，核心思想:

1. 将字符串视为 base 进制数:
   "abc" = a * base^2 + b * base^1 + c * base^0

2. 滑动窗口更新公式:
   hash(s[i+1..i+L]) = (hash(s[i..i+L-1]) - s[i] * base^(L-1)) * base + s[i+L]

3. 应用场景:
   - 字符串匹配 (模式搜索)
   - 最长重复子串 (本题)
   - 最长公共子串
   - 判断回文子串
   - 字符串去重

4. 时间复杂度:
   - 预计算: O(L)
   - 每次滑动: O(1)
   - 总计: O(n)

5. 注意事项:
   - 必须处理取模溢出
   - 必须处理负数取模
   - 哈希冲突需要二次验证或双哈希
```

---

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 28 | Find the Index of the First Occurrence | Rabin-Karp 基础应用 |
| 187 | Repeated DNA Sequences | 固定长度重复子串 (滚动哈希简化版) |
| 718 | Maximum Length of Repeated Subarray | 最长公共子数组 (二分+哈希) |
| 1062 | Longest Repeating Substring | 本题简化版 (Premium) |
| 1698 | Number of Distinct Substrings | 后缀数组/哈希 统计不同子串数 |

---

## 面试技巧

1. **先说思路框架**: "答案长度有单调性，可以二分搜索，每轮用 Rabin-Karp 检查"
2. **解释清楚滚动哈希**: 面试官最关注的部分，用具体例子 "banana" 演示
3. **主动讨论哈希冲突**: 展示你知道这不是 100% 正确的，需要验证步骤
4. **对比后缀数组**: 提到有 O(n) 解法但实现复杂，展示知识广度
5. **提到 __int128**: 如果面试官问溢出，说明你了解大数乘法的工程细节

## 面试追问

| 追问 | 关键点 |
|------|--------|
| 如何处理哈希冲突？能否完全避免？ | 双哈希将冲突概率降到 ~1/mod^2；后缀数组方案完全无冲突 |
| 如果要找出现至少 k 次的最长子串？ | 二分不变，check 改为 HashMap 中某哈希出现 >= k 次 |
| 字符集不是小写字母而是任意 byte？ | base 取 257 (>256)，字符映射改为 s[i]+1 |
| 能否用后缀数组 O(n) 解决？ | SA-IS 算法 O(n) 建后缀数组 + LCP，实现极复杂 |
