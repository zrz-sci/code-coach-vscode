# 751. IP to CIDR - IP 到 CIDR

## 核心思路

将起始 IP 转为 32 位整数，然后贪心地从当前 IP 出发，每次找到**能覆盖的最大 CIDR 块**（不超出剩余范围且对齐于当前 IP 的二进制结构），添加到结果中，推进 IP 并减少剩余数量。关键在于利用**最低有效位 (lowest set bit)** 来确定当前 IP 能支持的最大 CIDR 块大小。

## 思维链

1. 将 IP 地址字符串解析为 32 位无符号整数 `start`
2. 贪心循环：while `n > 0`
3. 每次确定当前 IP 能对齐的最大块大小：`lowbit = start & (-start)`（最低有效位）
4. 同时块大小不能超过剩余需要覆盖的 IP 数量 `n`
5. 取 `lowbit` 和 `n` 之间满足 2 的幂的最大值作为块大小
6. 由块大小算出前缀长度 `prefix = 32 - log2(blockSize)`
7. 将当前 IP 和前缀长度格式化为 CIDR 字符串
8. 推进 `start += blockSize`，减少 `n -= blockSize`

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 说明 |
|------|-----------|-----------|------|
| 贪心 + 位运算 | O(n) 最坏，O(log n) 通常 | O(1) 额外 | 唯一正解，利用 lowbit 对齐 |

## 关键提示

1. **lowbit 技巧**：`x & (-x)` 取出 x 的最低有效位，决定了该 IP 地址作为 CIDR 块起始时能对齐的最大块
2. **块大小上限**：除了 lowbit 限制外，块大小不能超过剩余的 n（否则覆盖了范围外的 IP）
3. **前缀长度计算**：块大小为 `2^p`，则前缀长度为 `32 - p`
4. **整数回转 IP**：从 32 位整数恢复为点分十进制格式
5. **特殊情况**：当 `start == 0` 时 `lowbit` 为 0，需要特殊处理——此时可覆盖任意大块，受 n 限制即可
6. **使用 long 防溢出**：IP 地址最大为 `255.255.255.255 = 2^32 - 1`，加法可能溢出 32 位 int

## 解法详解

### 贪心 + 位运算

**步骤详解**：

1. **解析 IP**：将 `"a.b.c.d"` 解析为 `(a << 24) | (b << 16) | (c << 8) | d`

2. **确定块大小**：
   - `lowbit = start & (-start)` 是对齐约束
   - 同时找到不超过 n 的最大 2 的幂
   - 块大小 = `min(lowbit, largestPowerOf2 <= n)`
   - 当 `start == 0` 时，lowbit 为 0，意味着没有对齐约束，取 n 决定的最大 2 的幂

3. **生成 CIDR**：
   - 前缀长度 = `32 - numberOfTrailingZeros(blockSize)`
   - 将当前 start 转回 IP 字符串，拼接 `/prefix`

4. **推进**：`start += blockSize; n -= blockSize;`

**为什么是贪心最优**：每次取最大合法块，等价于最少切割数，因为大块总是可以被拆分为小块，反之则不行。

## 易错点

1. **整数溢出**：IP 地址是无符号 32 位，C++ 中用 `long` 或 `unsigned int` 避免符号问题
2. **start == 0 的特殊情况**：`0 & (-0) == 0`，lowbit 为 0 不代表块大小为 0，而是无对齐限制
3. **块大小必须是 2 的幂**：不能直接用 `min(lowbit, n)`，因为 n 不一定是 2 的幂
4. **IP 字符串拼接格式**：不要忘记 `/` 分隔符
5. **点分十进制还原**：位移和掩码操作顺序不能搞错

## 面试追问

1. **Q: 为什么 lowbit 决定了最大对齐块？**
   A: CIDR 块要求起始 IP 的后 p 位全为 0 才能形成 /（32-p）的块。lowbit 表示从最低位起第一个 1 的位置，它上面的位都是 0，所以最多可以覆盖 lowbit 个地址。

2. **Q: 时间复杂度是多少？**
   A: 每个 CIDR 块至少覆盖 1 个 IP，最多需要 n 个块。但实际上每次覆盖 2 的幂个 IP，所以总块数为 O(log n) 级别。

3. **Q: 如何验证生成的 CIDR 列表是否正确？**
   A: 验证：(1) 所有块不重叠；(2) 并集恰好等于 [ip, ip+n-1]；(3) 每个 CIDR 块的 base IP 的后 (32-prefix) 位全为 0。

4. **Q: 如果输入是 IPv6 怎么办？**
   A: 原理相同，但地址空间为 128 位。需要使用大整数或 128 位整数类型，lowbit 和块大小计算逻辑不变。

5. **Q: 为什么不能直接用一个大 CIDR 块覆盖？**
   A: 因为大块可能包含范围外的 IP。题目要求恰好覆盖 [ip, ip+n-1]，不能多也不能少。

## 相关题型

- [468. Validate IP Address](https://leetcode.com/problems/validate-ip-address/) - 验证 IP 地址
- [93. Restore IP Addresses](https://leetcode.com/problems/restore-ip-addresses/) - 复原 IP 地址
- [1018. Binary Prefix Divisible By 5](https://leetcode.com/problems/binary-prefix-divisible-by-5/) - 位运算前缀
- [338. Counting Bits](https://leetcode.com/problems/counting-bits/) - 比特位计数
- [191. Number of 1 Bits](https://leetcode.com/problems/number-of-1-bits/) - 位1的个数
