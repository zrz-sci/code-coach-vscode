# 266. Palindrome Permutation (回文排列)

## 核心思路

判断一个字符串是否能重新排列成回文串。回文串的特征：
- **偶数长度**: 每个字符出现次数都是偶数
- **奇数长度**: 最多有一个字符出现次数为奇数（放在中间）

因此只需统计字符频率，**检查出现奇数次的字符个数是否 <= 1**。

## 思维链

1. **回文的性质**: 回文串从两端向中间对称，每个字符需要成对出现
2. **奇数次字符**: 最多允许 1 个奇数次字符（放中间位置）
3. **统计频率**: 用哈希表/数组统计每个字符出现次数
4. **计数奇数**: 遍历频率表，统计奇数次字符个数
5. **返回结果**: oddCount <= 1 即可构成回文

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 哈希表计数 | O(n) | O(1) | 直观简洁 |
| 位运算 (bitset/set) | O(n) | O(1) | ⭐ 巧妙优雅 |
| 数组计数 | O(n) | O(1) | 最快（26 个字母） |

## 关键提示

- 字符集只有小写字母(26个)，空间是常数级 O(1)
- 位运算技巧：用一个 int 的每个 bit 代表一个字母的奇偶性，遇到字符就 XOR
- 最终 `__builtin_popcount(mask) <= 1` 即为回文
- 也可以用 unordered_set：遇到重复的删掉，最终 set 大小 <= 1

## 解法详解

### 解法1: 哈希表计数

```cpp
class Solution {
public:
    bool canPermutePalindrome(string s) {
        unordered_map<char, int> freq;
        for (char c : s) freq[c]++;
        
        int oddCount = 0;
        for (auto& [ch, cnt] : freq) {
            if (cnt % 2 != 0) oddCount++;
        }
        return oddCount <= 1;
    }
};
```

### 解法2: 位运算 ⭐

```cpp
class Solution {
public:
    bool canPermutePalindrome(string s) {
        int mask = 0;
        for (char c : s) {
            mask ^= (1 << (c - 'a')); // 翻转对应 bit
        }
        // 最多只有 1 个 bit 为 1
        return __builtin_popcount(mask) <= 1;
        // 等价写法：return (mask & (mask - 1)) == 0;
    }
};
```

### 解法3: Set 技巧

```cpp
class Solution {
public:
    bool canPermutePalindrome(string s) {
        unordered_set<char> st;
        for (char c : s) {
            if (st.count(c)) st.erase(c); // 已有则删除（配对）
            else st.insert(c);             // 否则加入
        }
        return st.size() <= 1; // 最多剩 1 个未配对字符
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| oddCount == 0 | oddCount <= 1 | 奇数长度字符串允许 1 个奇数次字符 |
| 判断 oddCount == 1 | oddCount <= 1 | 偶数长度字符串 oddCount = 0 也合法 |
| 忘记空字符串 | 空串是回文 | 空串 oddCount = 0 <= 1，自然正确 |

## 面试追问

- **Q1**: 如果字符集扩展到 Unicode 怎么办？
  → 用 unordered_map 代替数组，位运算不再适用
- **Q2**: 如何构造出一个回文排列？（即 267 题）
  → 统计频率后，取一半字符做全排列，然后镜像翻转
- **Q3**: 能否 O(1) 空间、一次遍历解决？
  → 位运算解法已经是一次遍历 + O(1) 空间

## 相关题型

- [267. Palindrome Permutation II](https://leetcode.com/problems/palindrome-permutation-ii/) - 构造所有回文排列
- [125. Valid Palindrome](https://leetcode.com/problems/valid-palindrome/) - 判断回文
- [409. Longest Palindrome](https://leetcode.com/problems/longest-palindrome/) - 最长回文长度
- [5. Longest Palindromic Substring](https://leetcode.com/problems/longest-palindromic-substring/) - 最长回文子串
