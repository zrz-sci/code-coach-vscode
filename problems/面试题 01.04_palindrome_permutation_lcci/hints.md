# 面试题 01.04. 回文排列 - 提示与详解

## 难度：Easy | 标签：Bit Manipulation, Hash Table, String

---

## 一、核心思路

判断一个字符串的某个排列是否能构成回文串。关键数学性质：**回文串中，最多只有一个字符出现奇数次**（如果字符串长度为偶数，则所有字符都必须出现偶数次；如果为奇数，恰好一个字符出现奇数次，放在正中间）。

**核心洞察**：不需要真的构造回文，只需要统计字符频次，检查"奇数频次的字符个数 <= 1"即可。

---

## 二、思维链：从题目到解法

### Step 1：回文串的字符频次规律

```
回文串示例分析:

"tacocat" (长度7, 奇数)
  t:2  a:2  c:2  o:1
  奇数频次: 1个 (o)
  ✓ o 放在正中间: tac-o-cat

"abba" (长度4, 偶数)
  a:2  b:2
  奇数频次: 0个
  ✓ 所有字符配对: ab-ba

"abc" (长度3, 奇数)
  a:1  b:1  c:1
  奇数频次: 3个
  ✗ 无法排成回文

结论:
  奇数频次字符数 <= 1 → 可以排成回文
  奇数频次字符数 >= 2 → 不可能排成回文
```

### Step 2：为什么 <=1 就够了？

```
构造回文的方式:
1. 所有偶数频次的字符, 一半放左边一半放右边
2. 如果有一个奇数频次的字符, 多出来的那个放正中间

例: s = "tactcoa"
频次: t:2, a:2, c:2, o:1

构造过程:
  左半: t a c
  中间: o
  右半: c a t (左半的镜像)
  → "tacocat" ✓

如果有两个奇数频次字符:
例: "ab" → a:1, b:1
  无论如何排, "ab" 或 "ba" 都不是回文
  因为两个"多余的"字符无法都放在中间
```

### Step 3：三种实现方式

```
方法1: HashMap 统计频次
  遍历计数 → 遍历频次统计奇数个数

方法2: 数组统计频次
  用 int[128] 替代 HashMap (ASCII范围)

方法3: 位运算 (最巧妙)
  用一个 bitset/整数, 每遇到字符就翻转对应位
  最终检查是否至多一个位为 1
  
  原理:
  出现偶数次 → 翻转偶数次 → 位为 0
  出现奇数次 → 翻转奇数次 → 位为 1
  "至多一个位为1" ↔ bits == 0 || (bits & (bits-1)) == 0
```

### Step 4：位运算详解

```
s = "tactcoa"

处理每个字符 (用 bitset 简化表示, 只关注用到的位):
初始: bits = 0000000

't': bits ^= (1 << 't')  → 位t翻转为1
'a': bits ^= (1 << 'a')  → 位a翻转为1
'c': bits ^= (1 << 'c')  → 位c翻转为1
't': bits ^= (1 << 't')  → 位t翻转回0
'c': bits ^= (1 << 'c')  → 位c翻转回0
'o': bits ^= (1 << 'o')  → 位o翻转为1
'a': bits ^= (1 << 'a')  → 位a翻转回0

最终: bits = ...010...  (只有位o为1)
bits & (bits-1) = ...010... & ...001... = 0
→ 至多一个位为1 → 可以构成回文 ✓

技巧: n & (n-1) 消除 n 的最低位的1
  如果结果为0, 说明 n 原来最多只有一个1
```

### Step 5：关于 bits & (bits-1) == 0 的证明

```
bits & (bits - 1) 的效果: 消除 bits 最低位的 1

示例:
  bits   = 01010000
  bits-1 = 01001111
  &      = 01000000  (消除了最低位的1)

判断至多一个1:
  如果 bits == 0: 零个1 → 直接 true
  如果 bits 恰好一个1: bits & (bits-1) == 0 → true
  如果 bits 有多个1: bits & (bits-1) != 0 → false

所以判断条件: bits == 0 || (bits & (bits-1)) == 0
简化为:        (bits & (bits-1)) == 0
(因为 0 & (0-1) = 0 & 0xFFFFFFFF = 0, 等于0, 也满足)

但注意: 当 bits == 0 时, bits-1 = -1 (无符号为全1)
0 & 全1 = 0, 所以条件成立 ✓
```

---

## 三、解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|------|-----------|-----------|----------|
| ⭐ 位运算 | XOR翻转+检查至多一个1 | O(n) | O(1) | 最巧妙 |
| HashMap频次统计 | 统计频次+数奇数个数 | O(n) | O(k) | 最直观 |
| 数组频次统计 | 用int[128]计数 | O(n) | O(1) | 效率略高 |
| Set Toggle | 遇到就插入/删除 | O(n) | O(k) | 思路清晰 |

其中 k 为不同字符种类数。

---

## 四、关键提示（逐步递进）

<details>
<summary>提示 1：回文串有什么字符频次特征？</summary>

回文串中，最多只有一个字符出现奇数次。偶数长度回文所有字符都是偶数次；奇数长度回文恰好一个字符为奇数次（放在正中间）。

</details>

<details>
<summary>提示 2：怎么统计每个字符的出现次数？</summary>

可以用 HashMap 或固定大小的数组 `int freq[128]`。遍历字符串，对每个字符计数。然后再遍历频次表，统计奇数频次的字符个数。

</details>

<details>
<summary>提示 3：能否一次遍历就得到结果？</summary>

可以。用一个集合 (Set)：如果字符不在集合中就加入，如果已在集合中就移除。遍历结束后，集合大小就是奇数频次的字符个数。`set.size() <= 1` 即为回文排列。

</details>

<details>
<summary>提示 4：位运算怎么做？</summary>

用一个整数（或 bitset）作为"状态"。对每个字符 c，执行 `bits ^= (1 << c)`。XOR 的性质：出现偶数次的字符对应位回到 0，出现奇数次的字符对应位为 1。最终检查 `(bits & (bits-1)) == 0`，即至多一个位为 1。

</details>

<details>
<summary>提示 5：字符范围是什么？能直接用 int 做 bitset 吗？</summary>

题目没有限制只是小写字母。字符串由各种字符组成（可能包括空格等）。如果只考虑 ASCII（128 种字符），需要 128 位，可以用两个 long long 或 `bitset<128>`。如果只考虑小写字母（26种），一个 int 就够。

</details>

---

## 五、解法详解

### 解法一：位运算（最巧妙）

```cpp
class Solution {
public:
    bool canPermutePalindrome(string s) {
        // 用 bitset<128> 覆盖所有 ASCII 字符
        bitset<128> bits;
        
        for (char c : s) {
            bits.flip(c); // 翻转第 c 位
        }
        
        // 至多一个位为 1 → 可构成回文
        return bits.count() <= 1;
    }
};
```

**执行过程可视化**:

```
s = "tactcoa"

初始: bits = 全0

't': flip(116) → 位116变为1    奇数频次: {t}
'a': flip(97)  → 位97变为1     奇数频次: {t, a}
'c': flip(99)  → 位99变为1     奇数频次: {t, a, c}
't': flip(116) → 位116变回0    奇数频次: {a, c}
'c': flip(99)  → 位99变回0     奇数频次: {a}
'o': flip(111) → 位111变为1    奇数频次: {a, o}
'a': flip(97)  → 位97变回0     奇数频次: {o}

bits.count() = 1 <= 1 → true ✓
```

### 解法二：HashMap 频次统计（最直观）

```cpp
class Solution {
public:
    bool canPermutePalindrome(string s) {
        unordered_map<char, int> freq;
        
        // 统计每个字符频次
        for (char c : s) {
            freq[c]++;
        }
        
        // 统计奇数频次的字符个数
        int oddCount = 0;
        for (auto& [ch, cnt] : freq) {
            if (cnt % 2 != 0) {
                oddCount++;
            }
        }
        
        return oddCount <= 1;
    }
};
```

### 解法三：数组频次统计（常数更小）

```cpp
class Solution {
public:
    bool canPermutePalindrome(string s) {
        int freq[128] = {0}; // ASCII 范围
        
        for (char c : s) {
            freq[(unsigned char)c]++;
        }
        
        int oddCount = 0;
        for (int i = 0; i < 128; i++) {
            if (freq[i] % 2 != 0) {
                oddCount++;
                if (oddCount > 1) return false; // 提前退出
            }
        }
        
        return true;
    }
};
```

### 解法四：Set Toggle（一次遍历）

```cpp
class Solution {
public:
    bool canPermutePalindrome(string s) {
        unordered_set<char> oddChars;
        
        for (char c : s) {
            if (oddChars.count(c)) {
                oddChars.erase(c);  // 出现第偶数次 → 移除
            } else {
                oddChars.insert(c); // 出现第奇数次 → 加入
            }
        }
        
        // 集合中剩余的就是出现奇数次的字符
        return oddChars.size() <= 1;
    }
};
```

---

## 六、位运算技巧深入图解

```
n & (n-1) 消除最低位 1:

n     = 01010100
n-1   = 01010011   (最低位1变0, 更低位全变1)
n&n-1 = 01010000   (消除了最低位的1)

应用: 判断 n 是否为 2 的幂 (恰好一个1)
  n > 0 && (n & (n-1)) == 0

本题: 判断至多一个1
  (bits & (bits-1)) == 0
  (包括 bits==0 的情况, 因为 0 & (-1) = 0)

popcount (统计1的个数) 另一种写法:
  __builtin_popcount(bits) <= 1  // GCC 内建函数
  __builtin_popcountll(bits) <= 1  // 64位版本
```

---

## 七、边界情况分析

```
1. "" (空串) → true (0个奇数频次字符)
2. "a" (单字符) → true (1个奇数频次)
3. "aa" → true (0个奇数频次)
4. "ab" → false (2个奇数频次)
5. "aab" → true (b出现1次奇数, 放中间)
6. "  " (两个空格) → true (空格出现偶数次)
7. "A a" → 可能取决于题意, 本题含空格也算字符
```

---

## 八、易错点

### Bug 1：忘记考虑空格也是字符

```cpp
// 错误: 只统计字母
if (isalpha(c)) freq[c]++;

// 正确: 统计所有字符（包括空格）
// 本题的回文排列包含所有字符
freq[c]++;
```

### Bug 2：位运算范围不够

```cpp
// 错误: 用 int (32位) 做位运算
int bits = 0;
bits ^= (1 << c); // 当 c > 31 时溢出!

// 正确: 用 bitset<128> 或其他能覆盖 ASCII 的结构
bitset<128> bits;
bits.flip(c);
```

### Bug 3：判断条件写成 == 1 而非 <= 1

```cpp
// 错误: 只允许恰好一个奇数
return oddCount == 1;
// "aa" 的 oddCount = 0, 会返回 false!

// 正确: 允许零个或一个奇数
return oddCount <= 1;
```

### Bug 4：混淆"判断回文"和"判断回文排列"

```
"code" → 不是回文, 但也不是回文排列
  c:1, o:1, d:1, e:1 → 4个奇数 → false

"tactcoa" → 不是回文, 但是回文排列
  t:2, a:2, c:2, o:1 → 1个奇数 → true
  (可排列为 "tacocat")
```

---

## 九、面试追问（递进链）

### Q1：如果要实际构造出一个回文排列呢？

这是 LeetCode 267 "Palindrome Permutation II"。构造方法：找出所有偶数频次字符的一半放左边，如果有奇数频次字符放中间，然后左边镜像到右边。需要用回溯法生成所有排列。

### Q2：如果字符集不是 ASCII 而是 Unicode 呢？

HashMap 方法天然支持。位运算方法需要更大的 bitset（不实际），改用 HashMap 更合理。

### Q3：能否做到 O(1) 额外空间（不算频次数组）？

如果字符集大小固定（如 ASCII 128），freq[128] 是 O(1) 空间。位运算用 bitset<128> 也是 O(1)。这些都是常数空间。

### Q4：这道题和 LCR 018 / LC 125 "验证回文串"有什么区别？

LC 125 是判断字符串本身是否为回文。本题是判断字符串的某个排列是否能构成回文。前者需要双指针比较顺序，后者只需要频次统计不关心顺序。

### Q5：如果要求判断字符串的子串是否存在回文排列呢？

这就是 LeetCode 1457 "Pseudo-Palindromic Paths in a Binary Tree" 的思路，或者更复杂的子串版本需要前缀异或。

---

## 十、相关题型

| 题号 | 题名 | 关联点 |
|------|------|--------|
| 266 | Palindrome Permutation | 主站同题 |
| 267 | Palindrome Permutation II | 进阶: 构造所有回文排列 |
| 125 | Valid Palindrome | 验证回文串（双指针） |
| 680 | Valid Palindrome II | 删一个字符的回文验证 |
| 5 | Longest Palindromic Substring | 最长回文子串 |
| 1457 | Pseudo-Palindromic Paths | 树路径的回文排列判断 |

---

## 十一、总结

```
       面试题 01.04. 回文排列
                |
     核心性质: 奇数频次字符 <= 1
         /          |          \
    HashMap      数组计数      位运算
    统计频次     freq[128]    XOR翻转
        \          |          /
        统计奇数频次字符个数
                |
        <= 1 → true
        > 1  → false
```

**记忆口诀**：回文排列看频次，奇数字符至多一；位运算翻转巧，`n & (n-1)` 来判定。
