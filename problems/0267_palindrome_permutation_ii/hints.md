# 267. Palindrome Permutation II (回文排列 II)

## 核心思路

在 266 题（判断能否构成回文）的基础上，构造出所有可能的回文排列。关键洞察：
1. **先判断可行性**: 奇数次字符 > 1 则直接返回空
2. **只排列一半**: 回文是对称的，只需对前半部分做全排列
3. **去重排列**: 用排序 + 跳过重复的方式避免重复

## 思维链

1. **统计字符频率**: 用哈希表计数每个字符出现次数
2. **可行性检查**: 奇数次字符 > 1 直接返回空列表
3. **构造半串**: 每个字符取一半数量，如果有奇数次字符记录为中间字符 mid
4. **全排列半串**: 对半串做不重复的全排列（backtracking）
5. **构造回文**: 每个半串排列 + mid + 反转半串 = 完整回文
6. **收集结果**: 所有合法排列加入结果集

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 回溯 + 频率计数 | O((n/2)!) | O(n) | ⭐ 面试首选，直接用频率做 backtrack |
| 排序 + next_permutation | O((n/2)! * n) | O(n) | 利用 STL，代码短 |
| 回溯 + 排序去重 | O((n/2)!) | O(n) | 经典去重手法 |

## 关键提示

- 只需要排列**一半**字符，大大减少搜索空间
- 回溯时直接用频率数组（每个字符剩余可用次数），天然去重
- 中间字符 mid 可能为空（偶数长度时）
- 半串长度 = n/2，全排列数最多 (n/2)!

## 解法详解

### 解法1: 回溯 + 频率计数 ⭐

```cpp
class Solution {
public:
    vector<string> generatePalindromes(string s) {
        // 1. 统计频率
        unordered_map<char, int> freq;
        for (char c : s) freq[c]++;
        
        // 2. 检查可行性 & 构造半串字符和中间字符
        string mid = "";
        string half = "";
        for (auto& [ch, cnt] : freq) {
            if (cnt % 2 == 1) {
                if (!mid.empty()) return {}; // 超过1个奇数次字符
                mid = string(1, ch);
            }
            half += string(cnt / 2, ch);
        }
        
        // 3. 对 half 排序后做全排列
        sort(half.begin(), half.end());
        vector<string> res;
        
        // 4. 用 next_permutation 或回溯生成所有排列
        do {
            string palindrome = half + mid + string(half.rbegin(), half.rend());
            res.push_back(palindrome);
        } while (next_permutation(half.begin(), half.end()));
        
        return res;
    }
};
```

### 解法2: 回溯法（直接用频率数组）

```cpp
class Solution {
public:
    vector<string> result;
    
    void backtrack(unordered_map<char,int>& freq, string& path, int halfLen, const string& mid) {
        if ((int)path.size() == halfLen) {
            string rev(path.rbegin(), path.rend());
            result.push_back(path + mid + rev);
            return;
        }
        for (auto& [ch, cnt] : freq) {
            if (cnt > 0) {
                cnt -= 1;           // 每次取一个（已经是半数了）
                path.push_back(ch);
                backtrack(freq, path, halfLen, mid);
                path.pop_back();
                cnt += 1;
            }
        }
    }
    
    vector<string> generatePalindromes(string s) {
        unordered_map<char, int> freq;
        for (char c : s) freq[c]++;
        
        string mid = "";
        int halfLen = 0;
        for (auto& [ch, cnt] : freq) {
            if (cnt % 2 == 1) {
                if (!mid.empty()) return {};
                mid = string(1, ch);
            }
            freq[ch] = cnt / 2; // 变成半数
            halfLen += cnt / 2;
        }
        
        string path;
        backtrack(freq, path, halfLen, mid);
        return result;
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 对整个字符串做全排列 | 只对半串做全排列 | 对称性：只需一半 |
| 不去重 | 排序 + skip 或用频率回溯 | 否则 "aabb" 会产生重复 |
| 忘记处理 mid 字符 | mid 插在两半之间 | 奇数长度时中间有一个字符 |
| freq 直接做半数后忘记 mid | 先记录 mid 再做 cnt/2 | 顺序不能错 |

## 面试追问

- **Q1**: 时间复杂度怎么分析？
  → 半串长度 L = n/2，最坏情况 L! 个排列，每个排列构造 O(n)，总 O(L! * n)
- **Q2**: 如果字符串很长（n=16），排列数会不会爆？
  → 最坏 8! = 40320，完全可接受。但如果 n 更大，需要考虑剪枝
- **Q3**: 如何保证输出的排列没有重复？
  → 用 next_permutation 天然去重；或回溯时用频率数组，同一位置同一字符只选一次

## 相关题型

- [266. Palindrome Permutation](https://leetcode.com/problems/palindrome-permutation/) - 判断可行性
- [46. Permutations](https://leetcode.com/problems/permutations/) - 全排列基础
- [47. Permutations II](https://leetcode.com/problems/permutations-ii/) - 去重全排列
- [131. Palindrome Partitioning](https://leetcode.com/problems/palindrome-partitioning/) - 回文分割
