# 319. 灯泡开关 (Bulb Switcher)

**难度**: Medium | **标签**: Brainteaser, Math

## 核心思路

第 i 个灯泡被切换的次数等于 i 的因子个数。灯泡最终亮着当且仅当它被切换了奇数次。只有**完全平方数**的因子个数是奇数（因为因子成对出现，唯独平方根只算一次）。所以答案就是 1~n 中完全平方数的个数，即 `floor(sqrt(n))`。

## 思维链

1. **模拟观察**: 灯泡 k 在第 d 轮被切换，当且仅当 d 是 k 的因子
2. **因子配对**: 对于任意正整数 k，因子总是成对出现 (d, k/d)，除非 d = k/d，即 k 是完全平方数
3. **奇偶性推导**: 因子个数为奇数 <=> 切换奇数次 <=> 最终亮着 <=> k 是完全平方数
4. **计数**: 1~n 中完全平方数的个数 = floor(sqrt(n))

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 说明 |
|------|-----------|-----------|--------|------|
| 数学推导 (sqrt) | O(1) | O(1) | ⭐⭐⭐⭐⭐ | 直接返回 sqrt(n) |
| 暴力模拟 | O(n^2) | O(n) | ⭐ | n 可达 10^9，不可行 |

## 关键提示

1. **因子与切换的关系**: 灯泡 k 被切换的次数 = k 的因子个数
2. **完全平方数的特殊性**: 只有完全平方数有奇数个因子
3. **数学公式**: 答案 = floor(sqrt(n))
4. **边界条件**: n = 0 时返回 0

## 解法详解

### 解法一：数学推导 (最优解)

```cpp
class Solution {
public:
    int bulbSwitch(int n) {
        // 灯泡k最终亮着 <=> k的因子个数为奇数 <=> k是完全平方数
        // 1~n中完全平方数的个数 = floor(sqrt(n))
        return (int)sqrt(n);
    }
};
```

### 解法二：逐个检查完全平方数 (O(sqrt(n)))

```cpp
class Solution {
public:
    int bulbSwitch(int n) {
        int count = 0;
        for (long long i = 1; i * i <= n; i++) {
            count++;
        }
        return count;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `return sqrt(n);` 返回 double | ✓ `return (int)sqrt(n);` | 需要强制转换为 int |
| ✗ 模拟每一轮切换 O(n^2) | ✓ 数学推导 O(1) | n 可达 10^9，模拟超时 |
| ✗ 没有处理 n=0 的情况 | ✓ sqrt(0) = 0 自然处理 | 边界条件 |
| ✗ 用 int 做 i*i 可能溢出 | ✓ 用 long long 或直接 sqrt | 避免整数溢出 |

## 面试追问

**Q1: 为什么只有完全平方数的因子个数是奇数？**
> 因为因子成对出现 (d, n/d)，只有当 d = n/d 时（即 n 是完全平方数），这对因子合并为一个，导致总数为奇数。

**Q2: 如果改为第 i 轮把所有 i 的倍数位置的灯泡设为开（而非切换），答案如何变化？**
> 这时灯泡 k 最终是否亮取决于 k 的最大因子轮次操作。需要重新分析语义，不再是简单的因子计数问题。

**Q3: 如果 n 极大（如 10^18），sqrt 的精度是否会出问题？**
> 浮点 sqrt 可能有精度问题。可以用二分搜索找最大的 x 使得 x*x <= n，或者对 sqrt 结果做 +1/-1 微调验证。

## 相关题型

- [172. 阶乘后的零](https://leetcode.com/problems/factorial-trailing-zeroes/) - 数学推导
- [233. 数字 1 的个数](https://leetcode.com/problems/number-of-digit-one/) - 数学规律
- [204. 计数质数](https://leetcode.com/problems/count-primes/) - 数论基础
- [1375. 灯泡开关 III](https://leetcode.com/problems/bulb-switcher-iii/) - 灯泡系列变体
