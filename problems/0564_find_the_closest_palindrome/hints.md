# 564. Find the Closest Palindrome - 寻找最近的回文数

## 核心思路
最近的回文数一定来自有限的几个候选：用前半部分镜像构造回文，考虑前半部分 -1/不变/+1 三种情况，再加上边界候选（如 999...9、100...01）。

## 思维链
1. 回文数由前半部分决定 -> 镜像翻转即可
2. 最近回文一定在"当前前半部分 -1、不变、+1"三个镜像回文中
3. 还需考虑位数变化的边界：10...0 - 1 = 99...9，99...9 + 1 = 100...01
4. 从所有候选中选差值最小的（排除自身），平局取较小值

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 前半部分枚举 + 候选比较 | O(L) | O(L) | 唯一正解，L为位数 |

## 关键提示
1. 取前半部分（含中间位）：长度 L 的数，前半 = 前 (L+1)/2 位
2. 5个候选回文：前半-1镜像、前半镜像、前半+1镜像、10^(L-1)-1、10^L+1
3. 前半 -1 或 +1 可能导致位数变化，需特殊处理
4. 注意 n 本身可能就是回文，但题目要求排除自身

## 解法详解

### 解法1: 前半部分枚举 ⭐
```cpp
class Solution {
    long buildPalindrome(long half, bool oddLen) {
        long res = half;
        if (oddLen) half /= 10;
        while (half > 0) {
            res = res * 10 + half % 10;
            half /= 10;
        }
        return res;
    }
public:
    string nearestPalindromic(string n) {
        int len = n.size();
        long num = stol(n);
        long half = stol(n.substr(0, (len + 1) / 2));
        bool oddLen = (len % 2 == 1);
        
        // 5个候选
        vector<long> candidates;
        candidates.push_back(buildPalindrome(half, oddLen));      // 镜像
        candidates.push_back(buildPalindrome(half - 1, oddLen));  // 前半-1
        candidates.push_back(buildPalindrome(half + 1, oddLen));  // 前半+1
        candidates.push_back((long)pow(10, len - 1) - 1);        // 99...9
        candidates.push_back((long)pow(10, len) + 1);            // 100...01
        
        long closest = -1;
        for (long cand : candidates) {
            if (cand == num) continue;
            if (closest == -1 || 
                abs(cand - num) < abs(closest - num) ||
                (abs(cand - num) == abs(closest - num) && cand < closest)) {
                closest = cand;
            }
        }
        return to_string(closest);
    }
};
```

## 易错点
- ✗ 忘记边界候选 99...9 和 100...01 → ✓ 位数变化时镜像法失效
- ✗ 用 int 存储 → ✓ 数值最大 10^18，必须用 long long
- ✗ 忘记排除 n 本身 → ✓ n 本身可能是回文，必须跳过
- ✗ 前半 -1 导致位数减少（如 1000 -> 前半 10 -> 9 -> 构造 99）→ ✓ buildPalindrome 要能处理位数变化

## 面试追问

**Q1: 为什么只需要5个候选？**
最近回文要么与 n 同位数（前半 +-1 或不变的镜像），要么位数差1（只可能是 99...9 或 100...01）。

**Q2: buildPalindrome 如何处理奇偶长度？**
奇数长度时中间位不需要镜像，所以 half /= 10 跳过中间位再翻转。

**Q3: 时间复杂度为什么是 O(L)？**
构造每个回文候选 O(L)，候选数量固定为 5，总计 O(L)。

## 相关题型
- [9. Palindrome Number](https://leetcode.com/problems/palindrome-number/) - 回文数判断基础
- [479. Largest Palindrome Product](https://leetcode.com/problems/largest-palindrome-product/) - 回文数构造
- [906. Super Palindromes](https://leetcode.com/problems/super-palindromes/) - 回文数枚举
