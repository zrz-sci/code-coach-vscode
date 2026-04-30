# 390. Elimination Game - 消除游戏

## 核心思路
交替从左右两端消除元素，每轮数量减半、间距翻倍。关键在于追踪剩余序列的**起始位置**（head），而非模拟整个数组。

## 思维链
1. n最大10^9，不可能模拟 -> 需要O(log n)数学解法
2. 每轮消除后，剩余元素间距翻倍（step *= 2），数量减半
3. 从左消除时，head必然右移step；从右消除时，仅当剩余数量为奇数时head才右移
4. 循环直到remaining == 1，head即为答案

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 迭代模拟head | O(log n) | O(1) | 最优解，追踪首元素位置 |
| 递归对称性 | O(log n) | O(log n) | f(n) = 2*(n/2 + 1 - f(n/2)) |

## 关键提示
1. **从左消除**：head永远会更新（第一个被移除，head跳到下一个）
2. **从右消除**：只有当剩余个数为奇数时，最左边的才会被移除，head才更新
3. **递推公式**：`f(n) + f'(n) = n + 1`，其中f'是从右开始的结果

## 解法详解

### 解法一：迭代追踪head（推荐）
```cpp
class Solution {
public:
    int lastRemaining(int n) {
        int head = 1, step = 1, remaining = n;
        bool left = true;
        while (remaining > 1) {
            // 从左消除 或 从右消除且剩余为奇数 -> head右移
            if (left || remaining % 2 == 1) {
                head += step;
            }
            remaining /= 2;
            step *= 2;
            left = !left;
        }
        return head;
    }
};
```

### 解法二：递归对称性
```cpp
class Solution {
public:
    int lastRemaining(int n) {
        if (n == 1) return 1;
        // 从左消除后剩 n/2 个偶数：2,4,6,...
        // 等价于 2 * (从右消除 1..n/2 的结果)
        // 利用对称性：从右消除的结果 = n/2 + 1 - lastRemaining(n/2)
        return 2 * (n / 2 + 1 - lastRemaining(n / 2));
    }
};
```

## 易错点
- ✗ 尝试用数组模拟 -> n=10^9 必然超时/超内存
- ✓ 用head/step/remaining三个变量迭代

- ✗ 从右消除时无条件移动head
- ✓ 仅当remaining为奇数时才移动head

- ✗ step用int可能溢出
- ✓ 对于此题n<=10^9，int足够；但更大范围需用long

## 面试追问

**Q1: 为什么从右消除时只有奇数个元素才更新head？**
> 偶数个元素从右消除，最左边的元素不会被删（如[2,4,6,8]从右删：删8,删4，剩[2,6]，head不变）；奇数个元素从右消除，最左边会被删掉。

**Q2: 递归解法的对称性如何推导？**
> 将[1..n]从左消除一轮后得[2,4,...,2*floor(n/2)]，提取公因子2变为[1..n/2]的从右消除问题。从右消除f'(m) = m+1-f(m)（对称性：位置i和m+1-i关于中心对称）。

**Q3: 如果改为只从左消除（不交替），最后剩下什么？**
> 每轮删奇数位，剩偶数位。结果是最大的2的幂 <= n 的位置，即 `2^floor(log2(n))`。

## 相关题型
- [292. Nim Game](https://leetcode.com/problems/nim-game/) - 数学博弈
- [1823. Find the Winner of the Circular Game](https://leetcode.com/problems/find-the-winner-of-the-circular-game/) - 约瑟夫环
