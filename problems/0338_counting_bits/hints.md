# 338. 比特位计数

## 核心思路
利用位运算的递推关系，将每个数的popcount问题分解为已知子问题，实现O(n)线性时间求解0到n所有数的二进制1的个数。

## 思维链

1. **暴力法**: 对每个数i，逐位统计1的个数，每个数O(log i)，总共O(n log n)
2. **观察规律**: 列出0~15的二进制和popcount，发现数字之间存在递推关系
3. **右移递推**: i的二进制 = (i>>1的二进制) + 最低位 => `dp[i] = dp[i>>1] + (i&1)`
4. **去最低1递推**: `i & (i-1)` 去掉最低位的1 => `dp[i] = dp[i&(i-1)] + 1`
5. **最高位偏移**: 每到2的幂次开始新的"复制+1"周期 => `dp[i] = dp[i-highBit] + 1`
6. **最优解**: 三种递推均为O(n)时间O(1)额外空间，右移递推最直观好记

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 右移递推 | dp[i>>1] + (i&1)，砍掉最低位 | O(n) | O(1) | ⭐ |
| 去最低1递推 | dp[i&(i-1)] + 1，去掉最低位的1 | O(n) | O(1) | |
| 最高位偏移 | dp[i-highBit] + 1，分组复制 | O(n) | O(1) | |
| 暴力逐位 | 每个数while循环统计 | O(n log n) | O(1) | |
| Brian Kernighan | x&(x-1)循环计数 | O(nk) | O(1) | |

## 关键提示

1. **右移递推直觉**: i的二进制去掉最低位后就是i>>1，所以popcount(i) = popcount(i>>1) + 最低位

2. **三种递推的DP表**:
```
i    二进制   dp[i>>1]+(i&1)   dp[i&(i-1)]+1   dp[i-highBit]+1
0    0000     0                0               0
1    0001     dp[0]+1=1        dp[0]+1=1       dp[0]+1=1
2    0010     dp[1]+0=1        dp[0]+1=1       dp[0]+1=1
3    0011     dp[1]+1=2        dp[2]+1=2       dp[1]+1=2
4    0100     dp[2]+0=1        dp[0]+1=1       dp[0]+1=1
5    0101     dp[2]+1=2        dp[4]+1=2       dp[1]+1=2
6    0110     dp[3]+0=2        dp[4]+1=2       dp[2]+1=2
7    0111     dp[3]+1=3        dp[6]+1=3       dp[3]+1=3
8    1000     dp[4]+0=1        dp[0]+1=1       dp[0]+1=1
```

3. **popcount分组可视化** (每组是上一组的"镜像+1"):
```
[0]:      0
[1]:      1                   = [0] 每个+1
[2,3]:    1, 2                = [0,1] 每个+1
[4,7]:    1, 2, 2, 3          = [0,3] 每个+1
[8,15]:   1, 2, 2, 3, 2, 3, 3, 4  = [0,7] 每个+1
```

4. **位运算速查**: `i>>1`去掉最低位, `i&1`取最低位, `i&(i-1)`去掉最低位的1

## 解法详解

### 解法1: 右移递推 (最推荐)

```cpp
class Solution {
public:
    vector<int> countBits(int n) {
        vector<int> dp(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            // i 的二进制 = (i>>1 的二进制) + 最低位
            // 例: 13(1101) -> 6(0110) + 最低位1 => popcount = 2+1 = 3
            dp[i] = dp[i >> 1] + (i & 1);
        }
        return dp;
    }
};
```

### 解法2: 去最低1递推

```cpp
class Solution {
public:
    vector<int> countBits(int n) {
        vector<int> dp(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            // i & (i-1) 去掉最低位的1，结果一定 < i
            // 例: 12(1100) & 11(1011) = 8(1000) => popcount(12) = popcount(8)+1 = 2
            dp[i] = dp[i & (i - 1)] + 1;
        }
        return dp;
    }
};
```

### 解法3: 最高位偏移

```cpp
class Solution {
public:
    vector<int> countBits(int n) {
        vector<int> dp(n + 1, 0);
        int highBit = 0;
        for (int i = 1; i <= n; i++) {
            if ((i & (i - 1)) == 0) {
                highBit = i;  // i是2的幂，更新最高位
            }
            dp[i] = dp[i - highBit] + 1;
        }
        return dp;
    }
};
```

### 解法4: 暴力popcount

```cpp
class Solution {
public:
    vector<int> countBits(int n) {
        vector<int> ans(n + 1);
        for (int i = 0; i <= n; i++) {
            int count = 0, x = i;
            while (x) {
                count += (x & 1);
                x >>= 1;
            }
            ans[i] = count;
        }
        return ans;
    }
};
```

## 易错点

1. **数组大小错误**:
```cpp
// ✗ 错误: 只分配了n个元素，需要0到n共n+1个
vector<int> dp(n, 0);
// ✓ 正确: 分配n+1个元素
vector<int> dp(n + 1, 0);
```

2. **循环起始值**:
```cpp
// ✗ 错误: 从0开始，右移递推dp[0>>1]虽然不出错但多余
for (int i = 0; i <= n; i++) dp[i] = dp[i >> 1] + (i & 1);
// ✓ 正确: 从1开始，dp[0]=0是初始化的基础
dp[0] = 0;
for (int i = 1; i <= n; i++) dp[i] = dp[i >> 1] + (i & 1);
```

3. **最高位偏移忘记更新highBit**:
```cpp
// ✗ 错误: 没有检测2的幂时更新highBit
for (int i = 1; i <= n; i++)
    dp[i] = dp[i - highBit] + 1;
// ✓ 正确: 每当i是2的幂时更新
for (int i = 1; i <= n; i++) {
    if ((i & (i - 1)) == 0) highBit = i;
    dp[i] = dp[i - highBit] + 1;
}
```

## 面试追问

**Q1: 三种O(n)递推你更推荐哪个？**
> 右移递推 `dp[i>>1] + (i&1)` 最推荐。最直观、好记忆、面试写出来最快。

**Q2: 如何将空间优化到O(1)？**
> 输出数组本身就是DP数组，不计入额外空间。三种递推本身就是O(1)额外空间。

**Q3: 如果要求[lo, hi]范围内每个数的popcount？**
> 右移递推依赖更小的值，仍需从0开始计算。也可以对每个数独立用Brian Kernighan算法，单个数O(k)其中k是1的个数。

## 相关题型

| 题号 | 题目 | 关系 |
|------|------|------|
| 191 | Number of 1 Bits | 单个数的popcount |
| 461 | Hamming Distance | 异或后的popcount |
| 476 | Number Complement | 位运算掩码 |
| 762 | Prime Number of Set Bits | popcount是否为质数 |
| 1356 | Sort Integers by Number of 1 Bits | 按popcount排序 |
