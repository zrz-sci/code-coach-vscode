# 461. Hamming Distance - 汉明距离

## 核心思路
两整数的汉明距离 = XOR 结果中 1 的个数。XOR 恰好标记出不同的位。

## 思维链
1. 两数对应位不同 -> XOR 该位为 1
2. 统计 XOR 结果中 1 的个数 -> 即汉明距离
3. 统计方法：逐位右移 & 1 计数，或 Brian Kernighan 算法 `n &= (n-1)`

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ XOR + 逐位统计 | O(32) | O(1) | 简单直观 |
| ⭐ XOR + Kernighan | O(k) | O(1) | k=1的个数，更快 |
| 内置函数 __builtin_popcount | O(1) | O(1) | 一行搞定 |

## 关键提示
1. `x ^ y` 得到不同位的掩码
2. `n & (n-1)` 消除最低位的 1，循环次数 = 1 的个数
3. 注意本题与 LC 2220 完全相同

## 解法详解

### 解法一：XOR + 逐位统计
```cpp
class Solution {
public:
    int hammingDistance(int x, int y) {
        int xorVal = x ^ y;
        int count = 0;
        while (xorVal) {
            count += xorVal & 1;
            xorVal >>= 1;
        }
        return count;
    }
};
```

### 解法二：XOR + Brian Kernighan
```cpp
class Solution {
public:
    int hammingDistance(int x, int y) {
        int xorVal = x ^ y;
        int count = 0;
        while (xorVal) {
            xorVal &= (xorVal - 1); // 消除最低位的1
            count++;
        }
        return count;
    }
};
```

## 易错点
- ✗ 用 `x & y` 找不同位 -> ✓ 必须用 `x ^ y`（AND 找的是相同的1位）
- ✗ 右移时用 `xorVal /= 2` 处理负数有问题 -> ✓ 用无符号右移或 `>>= 1`（本题输入非负，无此风险）

## 面试追问

**Q1: Brian Kernighan 算法的原理是什么？**
> `n & (n-1)` 会将 n 的二进制中最低位的 1 变成 0。原理：n-1 把最低位 1 变 0，其下方所有 0 变 1，AND 后只消去那一个 1。

**Q2: 如果要计算一组数两两之间汉明距离总和怎么办？（LC 477）**
> 按位统计：对第 k 位，若有 c 个数该位为 1，则该位贡献 c * (n-c) 对不同。所有位累加即可，O(32n)。

**Q3: 汉明距离在实际中有什么应用？**
> 纠错码（ECC）、相似性检测（simhash）、DNA 序列比对。汉明距离表示传输中发生的比特翻转数。

## 相关题型
- [191. Number of 1 Bits](https://leetcode.com/problems/number-of-1-bits/) - popcount 基础
- [477. Total Hamming Distance](https://leetcode.com/problems/total-hamming-distance/) - 按位统计
- [2220. Minimum Bit Flips to Convert Number](https://leetcode.com/problems/minimum-bit-flips-to-convert-number/) - 完全相同
