# 555. Split Concatenated Strings

[LeetCode 链接](https://leetcode.com/problems/split-concatenated-strings/)

## 核心思路

**贪心预处理 + 枚举切割点**: 将字符串数组连成环, 每个字符串可正序或反转, 然后在某位置切开, 求字典序最大的展开结果。

关键观察: 对于**不包含切割点的字符串**, 贪心选 `max(s, reverse(s))` 一定最优 (它们在结果的中间段, 更大不会变差)。对于**包含切割点的字符串**, 需要同时尝试正序和反转的每个切割位置。

总复杂度 O(L^2), 其中 L 是所有字符串的总长度 (L <= 1000), 完全可接受。

---

## 思维链

```
输入: 字符串数组 strs
输出: 所有可能的环形切割展开中, 字典序最大的字符串

Step 1  预处理: 对每个 strs[i], 取 max(strs[i], reverse(strs[i]))
        → 这是对"不含切割点"的字符串的贪心最优方向

Step 2  枚举"在哪个字符串内切割": 对每个 strs[i]:
        (a) 构造"其他部分": other = strs[i+1] + ... + strs[n-1] + strs[0] + ... + strs[i-1]
            → 按环形顺序拼接所有非切割字符串

        (b) 尝试正序和反转: 对 strs[i] 和 reverse(strs[i]):
            → 枚举每个切割位置 j ∈ [0, len):
               candidate = cur[j..end] + other + cur[0..j-1]
               → 右半部分在前, 其他串在中间, 左半部分在后

        (c) 更新全局最大 result

Step 3  返回 result

时间: O(L^2) — 枚举 L 个切割位置, 每次构造 O(L) 字符串
空间: O(L) — 存储中间字符串
```

**手工模拟**: strs = ["abc", "xyz"]

```
预处理: "abc" vs "cba" → "cba"; "xyz" vs "zyx" → "zyx"
strs 变为 ["cba", "zyx"]

枚举 i=0 ("cba"), other = "zyx":
  正序 "cba": j=0 → "cbazyx", j=1 → "bazyxc", j=2 → "azyxcb"
  反转 "abc": j=0 → "abczyx", j=1 → "bczyxa", j=2 → "czyxab"

枚举 i=1 ("zyx"), other = "cba":
  正序 "zyx": j=0 → "zyxcba" ← 最大!, j=1 → "yxcbaz", j=2 → "xcbazy"
  反转 "xyz": j=0 → "xyzcba", j=1 → "yzcbax", j=2 → "zcbaxy"

答案: "zyxcba" ✓
```

---

## 解法概览

| 解法 | 时间 | 空间 | 推荐 | 说明 |
|------|------|------|------|------|
| 贪心 + 枚举切割 | O(L^2) | O(L) | ⭐ | L=总字符数(<=1000), 枚举每个字符位置 |
| 带剪枝优化版 | O(L^2) 最坏 | O(L) | | 只考虑以最大字符开头的位置, 实际快很多 |

---

## 关键提示

1. **不含切割点的串**: 贪心选 `max(s, reverse(s))`, 因为它们在结果中间段, 字典序更大一定不会变差。
2. **含切割点的串**: 正序和反转都要尝试, 因为切割位置不同时最优方向可能不同。
3. **环形拼接**: 切割 strs[i] 后, 其他部分按 `i+1, ..., n-1, 0, ..., i-1` 顺序拼接。
4. **切割位置**: 在字符串内部每个字符间都可能是切割点, 不只是字符串边界。
5. **字符串比较**: C++ 的 `>` 运算符直接进行字典序比较, 无需手写。

---

## 解法详解

### 解法一: 贪心 + 枚举切割点 (推荐)

```cpp
class Solution {
public:
    string splitLoopedString(vector<string>& strs) {
        int n = strs.size();

        // Step 1: 预处理 — 每个字符串取正序/反转中字典序更大的
        for (auto& s : strs) {
            string rev(s.rbegin(), s.rend());
            if (rev > s) s = rev;
        }

        string result = "";

        // Step 2: 枚举在哪个字符串内切割
        for (int i = 0; i < n; i++) {
            // 拼接 "其他部分": 环形顺序 i+1,...,n-1,0,...,i-1
            string other;
            for (int k = i + 1; k < n; k++) other += strs[k];
            for (int k = 0; k < i; k++) other += strs[k];

            // 尝试正序和反转
            string rev(strs[i].rbegin(), strs[i].rend());
            for (const string& cur : {strs[i], rev}) {
                int len = cur.size();
                // 枚举切割位置
                for (int j = 0; j < len; j++) {
                    // 切割后: cur[j..end] + other + cur[0..j-1]
                    string candidate = cur.substr(j) + other + cur.substr(0, j);
                    if (candidate > result) {
                        result = candidate;
                    }
                }
            }
        }

        return result;
    }
};
```

### 解法二: 带剪枝优化版

```cpp
class Solution {
public:
    string splitLoopedString(vector<string>& strs) {
        int n = strs.size();

        // 预处理: 贪心选最大方向
        for (auto& s : strs) {
            string rev(s.rbegin(), s.rend());
            if (rev > s) s = rev;
        }

        string result = "";

        for (int i = 0; i < n; i++) {
            string other;
            for (int k = i + 1; k < n; k++) other += strs[k];
            for (int k = 0; k < i; k++) other += strs[k];

            string rev(strs[i].rbegin(), strs[i].rend());

            for (const string& cur : {strs[i], rev}) {
                // 剪枝: 找到当前串中最大字符, 只考虑以该字符开头的位置
                char maxChar = *max_element(cur.begin(), cur.end());

                for (int j = 0; j < (int)cur.size(); j++) {
                    if (cur[j] < maxChar) continue;  // 跳过不可能最优的位置

                    string candidate = cur.substr(j) + other + cur.substr(0, j);
                    if (candidate > result) {
                        result = candidate;
                    }
                }
            }
        }

        return result;
    }
};
```

---

## 易错点

| 错误写法 (✗) | 正确写法 (✓) | 说明 |
|-------------|-------------|------|
| ✗ 切割串也用预处理的固定方向 | ✓ 切割串同时尝试正序和反转 | 切割位置不同时, 正序和反转可能产生不同最优结果 |
| ✗ 只枚举字符串边界切割 | ✓ 枚举字符串内部每个字符位置 | 最优切割点可能在字符串中间 |
| ✗ 环形拼接顺序错误 | ✓ other = strs[i+1..n-1] + strs[0..i-1] | 切割 i 后, 后半环在前, 前半环在后 |
| ✗ result 初始化为某个候选值 | ✓ result = "" (空串字典序最小) | 空串比任何非空串都小, 逐步更新即可 |
| ✗ 预处理时修改了切割串的方向就不再尝试另一方向 | ✓ 切割时重新计算 rev 并都尝试 | 预处理只影响非切割串 |

---

## 面试追问

**Q1: 时间复杂度为什么是可接受的?**

总字符数 L <= 1000。枚举每个字符作为切割点 O(L), 每次构造候选字符串 O(L), 总共 O(L^2) = O(10^6), 在时间限制内。字符串比较也是 O(L), 但实际上大多数比较在前几个字符就能得出结果。

**Q2: 为什么不含切割点的字符串可以贪心选最大方向?**

这些字符串出现在结果的"中间段"。假设中间段有两种选择 X 和 Y, X > Y (字典序)。无论前缀和后缀是什么, 选 X 的结果 `prefix + X + suffix` 一定 >= `prefix + Y + suffix`。因为当 prefix 相同时, 比较取决于 X vs Y; 当 prefix 不同时, 中间段不影响结果。

**Q3: 如果字符串可以重新排序 (不按给定顺序), 怎么做?**

变成全排列 * 方向选择 * 切割点的组合优化, 搜索空间为 O(n! * 2^n * L), 对于 n=1000 完全不可行。需要更高级的贪心策略或剪枝, 这已经是一个开放性研究问题。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 796 | Rotate String | 环形字符串旋转判断 |
| 899 | Orderly Queue | 字典序最小 + 旋转操作 |
| 1163 | Last Substring in Lexicographic Order | 字典序最大子串 |
| 151 | Reverse Words in a String | 字符串翻转操作 |
| 344 | Reverse String | 基础字符串反转 |
