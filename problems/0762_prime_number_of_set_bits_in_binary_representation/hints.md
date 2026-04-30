# 762. 二进制表示中质数个计算置位 (Prime Number of Set Bits in Binary Representation)

## 核心思路

本题将**位运算**与**质数判定**两个基础知识点结合。对于区间 `[left, right]` 中的每个数，先用 `__builtin_popcount` 或手动位运算计算二进制中 1 的个数（popcount），然后判断该计数是否为质数。由于 `right <= 10^6 < 2^20`，popcount 最大值为 20，因此只需判断 `{2, 3, 5, 7, 11, 13, 17, 19}` 这几个质数即可。可以用 **位掩码（bitmask）** 一次性编码所有 20 以内的质数，O(1) 完成质数判定。

## 思维链

1. **范围分析**：`right <= 10^6`，所以二进制最多 20 位，popcount 取值范围为 `[0, 20]`
2. **质数预计算**：20 以内的质数只有 `{2, 3, 5, 7, 11, 13, 17, 19}`，可以提前列出
3. **位掩码技巧**：将这些质数编码为一个整数的位：`(1<<2)|(1<<3)|(1<<5)|(1<<7)|(1<<11)|(1<<13)|(1<<17)|(1<<19) = 665772`
4. **逐个检查**：遍历 `[left, right]`，对每个数计算 popcount，用位掩码 O(1) 判断是否为质数
5. **累加计数**：满足条件的数量累加返回

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 位掩码判质数 | O(right - left) | O(1) | 最优解，利用 popcount 范围有限的特性 |
| HashSet 判质数 | O(right - left) | O(1) | 用集合存质数，语义清晰 |
| 手动 popcount + 试除 | O((right-left) * log(right)) | O(1) | 不依赖内置函数 |
| 查表法 popcount | O(right - left) | O(256) | 字节级查表，跨平台 |

## 关键提示

1. **popcount 上界分析**：`10^6 < 2^20`，所以 popcount 最大为 20，质数判定可以预计算
2. **位掩码加速**：把质数集合编码成一个 int 的位，用 `mask >> cnt & 1` 判断
3. **内置函数**：C++ 的 `__builtin_popcount`、Java 的 `Integer.bitCount` 都是 O(1) 操作
4. **Brian Kernighan 算法**：`n = n & (n-1)` 每次消去最低位的 1，也可手动计算 popcount
5. **区间约束**：`right - left <= 10^4`，总计算量很小，不需要特别优化

## 解法详解

### 解法一：位掩码 + __builtin_popcount（最优解）

**思路**：预先将 20 以内的所有质数编码为一个位掩码。遍历区间中每个数，计算其 popcount，然后通过位运算 O(1) 判断是否为质数。

**步骤**：
1. 构造位掩码：`mask = (1<<2)|(1<<3)|(1<<5)|(1<<7)|(1<<11)|(1<<13)|(1<<17)|(1<<19)`
2. 遍历 `[left, right]`，对每个数 `i` 计算 `cnt = __builtin_popcount(i)`
3. 检查 `(mask >> cnt) & 1` 是否为 1
4. 累加满足条件的计数

**时间复杂度**：O(right - left)，每个数 O(1) 处理  
**空间复杂度**：O(1)

### 解法二：HashSet 判质数

**思路**：用一个集合 `{2,3,5,7,11,13,17,19}` 存储所有可能的质数。遍历区间中每个数的 popcount，用 `count` 在集合中查找。

**步骤**：
1. 定义 `primes = {2, 3, 5, 7, 11, 13, 17, 19}`
2. 遍历计算 popcount 并在集合中查找
3. 使用 `unordered_set` 实现 O(1) 平均查找

### 解法三：手动 popcount (Brian Kernighan) + 试除法

**思路**：不依赖内置函数，手动实现 popcount（Brian Kernighan 算法），并用试除法判断质数。适合面试中被要求不使用内置函数时。

### 解法四：查表法 popcount

**思路**：预计算 0~255 每个字节的 popcount 查表数组。将 32 位整数拆成 4 个字节分别查表求和。这是工业级实现中常用的方法。

## 易错点

1. **1 不是质数**：popcount 为 1 时不应计入结果，这是常见错误
2. **0 不是质数**：popcount 为 0（即数值为 0）时也不算，但题目范围 `left >= 1` 所以不会遇到
3. **位掩码构造错误**：手动计算 `665772` 时容易出错，建议用 `|` 逐个构造验证
4. **popcount 上界估算错误**：如果将上界估为 32 则需要加入更多质数（23, 29, 31），但本题 `10^6 < 2^20` 所以 20 足够
5. **区间边界**：注意是**闭区间** `[left, right]`，`right` 本身要包含在内
6. **混淆 popcount 与数值本身**：要判断的是"popcount 是否为质数"而非"数值是否为质数"

## 面试追问

1. **Q: 如果 right 可以到 2^31-1，popcount 最大值是多少？需要加入哪些质数？**
   A: 最大 31 位。20 以内已覆盖 `{2,3,5,7,11,13,17,19}`，还需要加入 `{23, 29, 31}`。位掩码需要相应扩展为 64 位整数

2. **Q: 不用内置 popcount 函数怎么办？**
   A: 使用 Brian Kernighan 算法：`while(n) { n &= n-1; count++; }`，每次消除最低位 1。时间复杂度 O(popcount)

3. **Q: 位掩码技巧还能用在哪些场景？**
   A: 小范围集合判定（如元音字母判定 `mask >> (c-'a') & 1`）、状态压缩 DP、子集枚举（`for(s=mask; s; s=(s-1)&mask)`）

4. **Q: 如果区间很大（right - left 达到 10^9），如何优化？**
   A: 使用数位 DP。按 popcount 值分组，计算区间 `[0, right]` 和 `[0, left-1]` 中 popcount 恰好等于某个质数 p 的数的个数，然后对所有质数 p 求和

5. **Q: 能否用查表法替代 popcount？**
   A: 可以。对 0~255 预建 256 大小的查表数组，将 32 位数拆成 4 个字节查表后求和。这是 GCC `__builtin_popcount` 在无硬件指令支持时的实际实现方式之一

## 相关题型

- [191. 位1的个数 (Number of 1 Bits)](https://leetcode.com/problems/number-of-1-bits/) - popcount 基础练习
- [338. 比特位计数 (Counting Bits)](https://leetcode.com/problems/counting-bits/) - 批量 popcount + DP 递推
- [461. 汉明距离 (Hamming Distance)](https://leetcode.com/problems/hamming-distance/) - XOR + popcount
- [204. 计数质数 (Count Primes)](https://leetcode.com/problems/count-primes/) - 埃拉托斯特尼筛法
- [693. 交替位二进制数 (Binary Number with Alternating Bits)](https://leetcode.com/problems/binary-number-with-alternating-bits/) - 位运算技巧
- [477. 汉明距离总和 (Total Hamming Distance)](https://leetcode.com/problems/total-hamming-distance/) - 逐位统计技巧
