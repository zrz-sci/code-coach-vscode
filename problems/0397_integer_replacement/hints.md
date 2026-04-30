# 397. Integer Replacement - 整数替换

## 核心思路
偶数只能除2；奇数选+1或-1，关键在于**看末两位**来贪心决策：末两位为`11`时+1（能连续除2更多次），末两位为`01`时-1，特例n=3时-1。

## 思维链
1. 偶数：n/2，无选择
2. 奇数：n+1或n-1，目标是让后续能尽快变小（连续右移）
3. 贪心策略：让二进制末尾产生更多0
4. 若末两位=11，n+1会把连续的1进位变成更高位的10...0，产生多个0
5. 若末两位=01，n-1直接消掉末尾的1
6. 特例：n=3(二进制11)，n-1=2->1比n+1=4->2->1少一步

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 位运算贪心 | O(log n) | O(1) | 最优，看末两位决策 |
| BFS/记忆化递归 | O(log n) | O(log n) | 通用但空间较大 |
| 递归 | O(log n) | O(log n) | 简洁但需注意溢出 |

## 关键提示
1. **偶数**：直接右移 `n >>= 1`
2. **奇数贪心**：`n & 3 == 3` 且 `n != 3` 时 +1，否则 -1
3. **溢出**：n = INT_MAX = 2^31-1 时，n+1溢出 -> 用 `long` 或特殊处理
4. 操作次数 = 二进制位数 + 1的个数 - 1（近似）

## 解法详解

### 解法一：位运算贪心（推荐）
```cpp
class Solution {
public:
    int integerReplacement(int n) {
        int ops = 0;
        long num = n; // 防止INT_MAX+1溢出
        while (num > 1) {
            if (num % 2 == 0) {
                num >>= 1;
            } else if (num == 3 || (num & 3) == 1) {
                num--; // 末两位01或n==3
            } else {
                num++; // 末两位11
            }
            ops++;
        }
        return ops;
    }
};
```

### 解法二：记忆化递归
```cpp
class Solution {
public:
    unordered_map<long, int> memo;
    int integerReplacement(int n) {
        return dfs((long)n);
    }
    int dfs(long n) {
        if (n == 1) return 0;
        if (memo.count(n)) return memo[n];
        if (n % 2 == 0) return memo[n] = 1 + dfs(n / 2);
        return memo[n] = 1 + min(dfs(n + 1), dfs(n - 1));
    }
};
```

## 易错点
- ✗ 用int存储n，当n=INT_MAX时n+1溢出
- ✓ 转成long再操作

- ✗ 奇数时总是选-1 -> 对于末尾多个连续1的情况不是最优
- ✓ 末两位=11时+1更优（除了n=3的特例）

- ✗ 忘记n=3的特例（3->2->1比3->4->2->1少一步）
- ✓ 单独处理n==3或判断 `num==3 || (num&3)==1` 时-1

## 面试追问

**Q1: 为什么末两位是11时+1更优？**
> 末两位11意味着连续的1，+1会使这些1进位变成10...0，产生连续的0可以连续右移。而-1只消除最后一个1，下一位仍是1，又需要处理。例如：...111 +1 = ...1000（3个右移），...111 -1 = ...110（1个右移后又遇到1）。

**Q2: 为什么n=3是特例？**
> n=3(11): -1=2(10)->1，共2步；+1=4(100)->2->1，共3步。因为3+1=4只有一个0可以右移，而3-1=2直接就能到1。当连续1的长度<=2时，-1更优。

**Q3: 能否用BFS求解？有什么优劣？**
> 可以，BFS天然求最短路。但对于n=2^31-1，状态空间可达数十亿，不现实。贪心O(log n)是最优选择。BFS适合n较小或需要证明最优性的场景。

## 相关题型
- [326. Power of Three](https://leetcode.com/problems/power-of-three/) - 数学+位运算
- [191. Number of 1 Bits](https://leetcode.com/problems/number-of-1-bits/) - 位运算
- [1290. Convert Binary Number in a Linked List to Integer](https://leetcode.com/problems/convert-binary-number-in-a-linked-list-to-integer/) - 二进制操作
