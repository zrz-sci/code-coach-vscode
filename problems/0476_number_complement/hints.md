# 476. 数字的补数

## 核心思路
构造一个与num二进制位数相同的全1掩码mask，然后`num XOR mask`即为补数，关键在于如何高效构造这个mask。

## 思维链

1. **理解补数**: 5(101)的补数是2(010)，即每一位0变1、1变0，但不考虑前导零
2. **暴力法**: 逐位检查num的每一位，翻转后构造结果，O(log n)
3. **XOR思路**: num XOR 全1掩码 = 补数。例如101 XOR 111 = 010
4. **构造mask**: 需要一个与num二进制位数相同的全1数。mask = (1 << bitLength) - 1
5. **循环构造**: 也可以从0开始，不断 `mask = (mask << 1) | 1` 直到 mask >= num
6. **highestBit法**: 找到最高位的位置，直接算出 mask = (highestBit << 1) - 1

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 循环构造mask | mask不断左移加1直到>=num | O(log n) | O(1) | ⭐ |
| 位长计算mask | mask = (1<<bitLen)-1 | O(log n) | O(1) | |
| highestOneBit | 找最高位后计算mask | O(log n) | O(1) | |
| 逐位翻转 | 每一位独立XOR | O(log n) | O(1) | |

## 关键提示

1. **XOR翻转原理**:
```
  num  = 101  (5)
  mask = 111  (7)
  XOR  = 010  (2)  <-- 补数

规律: 0 XOR 1 = 1 (翻转)
      1 XOR 1 = 0 (翻转)
```

2. **mask构造过程**:
```
num = 5 (101), 需要 mask = 111 (7)

方法1: 循环左移加1
  mask = 0
  mask = (0 << 1) | 1 = 1    (1)       < 5, 继续
  mask = (1 << 1) | 1 = 3    (11)      < 5, 继续
  mask = (3 << 1) | 1 = 7    (111)     >= 5, 停止

方法2: 计算位长
  bitLen = 3  (101 有 3 位)
  mask = (1 << 3) - 1 = 8 - 1 = 7  (111)

方法3: 最高位
  highest = 4  (100, 最高的1所在位)
  mask = (4 << 1) - 1 = 7  (111)
```

3. **特殊情况**: num = 1(1) -> mask = 1(1) -> 1 XOR 1 = 0

4. **整数溢出注意**: num < 2^31，mask最大可达2^31 - 1。用unsigned或注意1<<31的溢出

## 解法详解

### 解法1: 循环构造mask (最推荐)

```cpp
class Solution {
public:
    int findComplement(int num) {
        unsigned mask = 0;
        unsigned n = num;
        // 构造全1掩码: 不断左移加1直到 >= num
        while (mask < n) {
            mask = (mask << 1) | 1;
        }
        return num ^ mask;
    }
};
```

### 解法2: 位长计算mask

```cpp
class Solution {
public:
    int findComplement(int num) {
        // 计算num的二进制位长度
        int bitLen = 0;
        int temp = num;
        while (temp > 0) {
            bitLen++;
            temp >>= 1;
        }
        // mask = 2^bitLen - 1 (全1)
        // 注意: 1<<31会溢出int, 用unsigned
        unsigned mask = (1u << bitLen) - 1;
        return num ^ mask;
    }
};
```

### 解法3: 逐位翻转

```cpp
class Solution {
public:
    int findComplement(int num) {
        int result = 0;
        int bit = 1;  // 当前处理的位
        int temp = num;
        while (temp > 0) {
            // 如果当前位是0，结果对应位设为1
            if ((temp & 1) == 0) {
                result |= bit;
            }
            bit <<= 1;
            temp >>= 1;
        }
        return result;
    }
};
```

### 解法4: 利用内置函数 (一行解法)

```cpp
class Solution {
public:
    int findComplement(int num) {
        // __builtin_clz: 前导零个数
        // 32 - clz(num) = 有效位数
        // mask = (1u << 有效位数) - 1
        return num ^ ((1u << (32 - __builtin_clz(num))) - 1);
    }
};
```

## 易错点

1. **mask构造时的整数溢出**:
```cpp
// ✗ 错误: 当num接近INT_MAX时, 1<<bitLen可能溢出
int mask = (1 << bitLen) - 1;
// ✓ 正确: 使用unsigned避免溢出
unsigned mask = (1u << bitLen) - 1;
```

2. **mask循环条件错误**:
```cpp
// ✗ 错误: mask > num 时才停止，但可能mask永远追不上
while (mask != num) { ... }
// ✓ 正确: mask >= num 时停止
while (mask < (unsigned)num) {
    mask = (mask << 1) | 1;
}
```

3. **混淆补码和补数**:
```cpp
// ✗ 错误: ~num 是按位取反(32位全翻转，包括前导零)
return ~num;  // 5 -> -6, 不是2!
// ✓ 正确: 只翻转有效位
return num ^ mask;  // mask只覆盖有效位
```

4. **num=0的边界**:
```cpp
// 题目约束 num >= 1, 但若要处理0:
// 0的补数是1 (0->1), mask=1, 0^1=1
// 循环构造mask时: while(mask<0) 不执行, mask=0, 0^0=0 -> 错误!
// 需要特殊处理 if (num == 0) return 1;
```

## 面试追问

**Q1: ~num和findComplement(num)有什么区别？**
> `~num`是对所有32位取反，包括前导零。例如5(00...0101) -> ~5 = 11...1010 = -6。而补数只翻转有效位：5(101) -> 010 = 2。区别在于mask只覆盖有效二进制位。

**Q2: 如何不用XOR实现？**
> 用逐位翻转：检查每一位，如果是0就在结果中设为1。或者用减法：mask - num = 补数（因为mask是全1，num + complement = mask）。

**Q3: 这题和LC 1009有什么关系？**
> LC 1009 "Complement of Base 10 Integer" 完全相同，只是1009允许num=0（答案为1），而476约束num >= 1。

## 相关题型

| 题号 | 题目 | 关系 |
|------|------|------|
| 1009 | Complement of Base 10 Integer | 完全相同的题 |
| 338 | Counting Bits | 位运算DP |
| 191 | Number of 1 Bits | popcount |
| 461 | Hamming Distance | XOR后popcount |
| 136 | Single Number | XOR性质 |
