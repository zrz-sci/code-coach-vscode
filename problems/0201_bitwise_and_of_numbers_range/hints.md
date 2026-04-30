# 201. 数字范围按位与 (Bitwise AND of Numbers Range)

**难度**: Medium | **标签**: Bit Manipulation

## 核心思路

[left, right] 范围内所有数的按位与，本质是找 left 和 right 的**公共前缀**。因为只要某一位在范围内出现过 0，AND 后该位就必然为 0。

## 思维链

1. 暴力逐个 AND → 超时（right - left 可达 2^31）
2. 观察：从 left 到 right，低位不断变化，AND 后低位全变 0
3. 只有 left 和 right 的**高位公共前缀**会保留
4. 问题转化：找 left 和 right 的二进制公共前缀，低位补 0

## 解法概览

| 解法 | 思路 | 时间 | 推荐 |
|------|------|------|------|
| 右移找公共前缀 | 同时右移直到相等 | O(log n) | ⭐ |
| Brian Kernighan | 不断清除 right 最右 1 | O(log n) | ⭐ |
| 位掩码 | 从高位逐位检查 | O(32) | |

## 关键提示

1. 两个数不同的位，在范围内一定会被 AND 清零
2. `right & (right - 1)` 可以清除 right 的最低位 1（Brian Kernighan 技巧）
3. 右移法：left 和 right 同时右移，移动次数就是需要补零的位数
4. left == right 时停止，此时就是公共前缀

## 解法详解

### 解法一：右移找公共前缀（推荐）

```cpp
class Solution {
public:
    int rangeBitwiseAnd(int left, int right) {
        int shift = 0;
        // 不断右移直到 left == right，找到公共前缀
        while (left < right) {
            left >>= 1;
            right >>= 1;
            shift++;
        }
        // 公共前缀左移回原来的位置
        return left << shift;
    }
};
```

**示例**：left=5(101), right=7(111)
- 第1次右移：left=10, right=11, shift=1
- 第2次右移：left=1, right=1, shift=2
- 相等停止，结果 = 1 << 2 = 4(100)

### 解法二：Brian Kernighan

```cpp
class Solution {
public:
    int rangeBitwiseAnd(int left, int right) {
        // 不断清除 right 的最低位 1，直到 right <= left
        while (right > left) {
            right &= (right - 1);
        }
        return right;
    }
};
```

**原理**：`right & (right-1)` 每次清除最低的 1 位。当 right 的低位被清完到 <= left 时，剩余的就是公共前缀。

### 解法三：位掩码

```cpp
class Solution {
public:
    int rangeBitwiseAnd(int left, int right) {
        unsigned int mask = ~0;  // 全1掩码
        while ((left & mask) != (right & mask)) {
            mask <<= 1;  // 掩码左移，忽略更多低位
        }
        return left & mask;
    }
};
```

## 易错点

1. ✗ 暴力循环 `for(i=left; i<=right; i++) result &= i;` → 超时
   ✓ 用公共前缀法 O(log n)

2. ✗ 使用 `int` 移位时溢出（left=1, right=2147483647）
   ✓ 注意右移操作安全，但 left<<shift 需确保不溢出

3. ✗ `while(left != right)` 对于 left=0, right=0 仍正确
   ✓ 初始就相等，直接返回 0

## 面试追问

**Q1**: 为什么公共前缀以下的位一定都是 0？
→ 如果某位在 left 和 right 间不同，那范围内一定存在该位为 0 的数

**Q2**: Brian Kernighan 技巧 `n & (n-1)` 的本质是什么？
→ 消除 n 的最低位 1，因为 n-1 会把最低 1 及其右边全部翻转

**Q3**: 如果范围扩展到 64 位整数，需要改什么？
→ 循环最多 64 次，用 long long 类型，逻辑不变

## 相关题型

- 136: Single Number（位运算 XOR）
- 191: Number of 1 Bits（Brian Kernighan）
- 231: Power of Two（n & (n-1) == 0）
- 338: Counting Bits（位运算 DP）
