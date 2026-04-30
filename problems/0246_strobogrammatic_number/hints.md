# 246. Strobogrammatic Number (中心对称数)

## 核心思路

旋转180度后不变的数字，本质上是**双指针对称匹配**问题。只有 0, 1, 6, 8, 9 这五个数字旋转后有效，且 6<->9 互为对称，0, 1, 8 自身对称。用双指针从两端向中间检查每对字符是否满足对称映射关系。

## 思维链

1. **识别有效字符**: 旋转180度后仍为有效数字的只有 0, 1, 6, 8, 9
2. **建立映射关系**: 0->0, 1->1, 6->9, 8->8, 9->6
3. **双指针策略**: 左指针 `l` 从头开始，右指针 `r` 从尾开始
4. **逐对校验**: `num[l]` 旋转后必须等于 `num[r]`，否则返回 false
5. **中心处理**: 如果长度为奇数，中间字符必须是自对称的 (0, 1, 8)
6. **全部通过则返回 true**

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 双指针 + HashMap | O(n) | O(1) | ⭐⭐⭐⭐⭐ |
| 构建翻转字符串比较 | O(n) | O(n) | ⭐⭐⭐ |

## 关键提示

- 映射表只有5个entry: `{0:0, 1:1, 6:9, 8:8, 9:6}`
- 遇到 2, 3, 4, 5, 7 直接返回 false
- 奇数长度时中间字符只能是 0, 1, 8
- 双指针会自然处理奇偶长度，无需特判

## 解法详解

### 解法1: 双指针 + HashMap (推荐)

用哈希表存储旋转映射，双指针从两端向中间逼近。

```cpp
class Solution {
public:
    bool isStrobogrammatic(string num) {
        unordered_map<char, char> mp = {
            {'0', '0'}, {'1', '1'}, {'6', '9'}, {'8', '8'}, {'9', '6'}
        };
        int l = 0, r = num.size() - 1;
        while (l <= r) {
            if (mp.find(num[l]) == mp.end() || mp[num[l]] != num[r]) {
                return false;
            }
            l++;
            r--;
        }
        return true;
    }
};
```

**复杂度**: 时间 O(n), 空间 O(1)

### 解法2: 构建翻转字符串比较

将整个字符串逐字符翻转映射后反转，与原字符串比较。

```cpp
class Solution {
public:
    bool isStrobogrammatic(string num) {
        unordered_map<char, char> mp = {
            {'0', '0'}, {'1', '1'}, {'6', '9'}, {'8', '8'}, {'9', '6'}
        };
        string rotated;
        for (char c : num) {
            if (mp.find(c) == mp.end()) return false;
            rotated += mp[c];
        }
        reverse(rotated.begin(), rotated.end());
        return rotated == num;
    }
};
```

**复杂度**: 时间 O(n), 空间 O(n)

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 只检查 `num[l] == num[r]` | ✓ 检查 `mp[num[l]] == num[r]` | 6和9不相等但互为对称 |
| ✗ `while (l < r)` | ✓ `while (l <= r)` | 奇数长度时中间字符也需检查 |
| ✗ 忘记检查字符是否在映射表中 | ✓ 先检查 `mp.find() != mp.end()` | 2,3,4,5,7 不是有效中心对称字符 |

## 面试追问

**Q1: 如果输入可能包含负号或小数点怎么办?**
> 需要在预处理阶段剥离符号，小数点本身旋转180度不变，可加入映射。

**Q2: 如何生成所有 n 位的中心对称数? (引出 LC 247)**
> 递归从内到外构建，base case 为 n=0 返回 `[""]`, n=1 返回 `["0","1","8"]`。

**Q3: 能否用位运算或其他 O(1) 空间方案替代 HashMap?**
> 可以用数组 `char mp[10]` 替代，将 0-9 映射到对应字符，无效数字映射到特殊值。

## 相关题型

- [247. Strobogrammatic Number II](../0247_strobogrammatic_number_ii/) - 生成所有n位中心对称数
- [248. Strobogrammatic Number III](../0248_strobogrammatic_number_iii/) - 统计范围内中心对称数个数
- [9. Palindrome Number](https://leetcode.com/problems/palindrome-number/) - 回文数判断
- [125. Valid Palindrome](https://leetcode.com/problems/valid-palindrome/) - 类似双指针对称检查
