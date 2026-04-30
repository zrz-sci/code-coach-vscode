/*
 * 【题目本质】
 * 用 read4 实现 read(n)，且 read 可被多次调用。
 * 核心是"维护内部缓冲区保存上次多读但未消费的字符"。
 *
 * 【解法总览】
 * 1. 内部缓冲区 + 成员变量状态管理: O(n) 时间, O(1) 空间
 *
 * 【缓冲区状态图示】
 *
 *   file = "abcdefg"
 *
 *   === 第1次 read(buf, 2) ===
 *   buf4: [a][b][c][d]  bufCount=4, bufIdx=0
 *          ^取 ^取
 *   buf4: [a][b][c][d]  bufIdx=2 (c,d 还没用)
 *   buf = "ab", 返回 2
 *
 *   === 第2次 read(buf, 4) ===
 *   buf4: [a][b][c][d]  bufIdx=2, 先取 c,d
 *                ^取 ^取
 *   buf4 空了, 调 read4:
 *   buf4: [e][f][g][ ]  bufCount=3, bufIdx=0
 *          ^取 ^取
 *   buf = "cdef", bufIdx=2, 返回 4
 *
 *   === 第3次 read(buf, 5) ===
 *   buf4: [e][f][g][ ]  bufIdx=2, 先取 g
 *                   ^取
 *   buf4 空了, 调 read4: 返回0, 文件结束
 *   buf = "g", 返回 1
 */

// ===================== 解法1: 内部缓冲区 + 状态管理 =====================
// 思路: 用成员变量保存 read4 的缓冲状态，每次 read 先消费剩余再补充
// 为什么需要成员变量? 因为 read 被多次调用，局部变量下次调用就丢了
class Solution1 {
private:
    char buf4[4];      // read4 的持久化缓冲区
    int bufIdx = 0;    // 下次从 buf4 的哪个位置开始读
    int bufCount = 0;  // buf4 中当前有多少有效字符

public:
    int read(char *buf, int n) {
        int total = 0;

        while (total < n) {
            // 缓冲区空了才调 read4 补充
            // 为什么不每次都调? 上次可能有剩余字符还没消费
            if (bufIdx >= bufCount) {
                bufCount = read4(buf4);
                bufIdx = 0;
                // read4 返回 0 说明文件已经读完
                if (bufCount == 0) break;
            }

            // 从缓冲区逐字符拷贝到目标 buf
            // 为什么用 while 而非 for? 需要同时检查两个条件
            while (total < n && bufIdx < bufCount) {
                buf[total++] = buf4[bufIdx++];
            }
        }

        return total;
    }
};

// ===================== 解法1 变体: 逐字符版本 =====================
// 思路: 更紧凑的写法，每次循环只处理一个字符
// 为什么这样写? 逻辑更简单，不容易出错
class Solution2 {
private:
    char buf4[4];
    int bufIdx = 0;
    int bufCount = 0;

public:
    int read(char *buf, int n) {
        int total = 0;

        while (total < n) {
            // 缓冲区为空时补充
            if (bufIdx == bufCount) {
                bufCount = read4(buf4);
                bufIdx = 0;
            }
            // 文件结束
            if (bufCount == 0) break;

            // 每次只取一个字符
            buf[total++] = buf4[bufIdx++];
        }

        return total;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 说明 |
 * |------|------|------|------|
 * | 解法1 | O(n) | O(1) | 批量拷贝，效率略高 |
 * | 解法2 | O(n) | O(1) | 逐字符，代码更简洁 |
 *
 * 【与157题对比】
 * | 特性 | 157 (单次调用) | 158 (多次调用) |
 * |------|---------------|----------------|
 * | buf4 位置 | 局部变量 | 成员变量 |
 * | 状态管理 | 不需要 | bufIdx + bufCount |
 * | 复杂度 | 简单 | 需要理解状态机 |
 *
 * 【易错点】
 * 1. 成员变量未初始化 → bufIdx/bufCount 值不确定
 * 2. 每次 read 错误重置成员变量 → 丢弃上次剩余数据
 * 3. 调 read4 后忘记 bufIdx = 0 → 从错误位置读
 * 4. buf4 声明为局部变量 → 下次调用数据丢失(157做法)
 *
 * 【面试追问】
 * Q1: 与157的区别? → 多次调用需要持久化缓冲区状态
 * Q2: 线程安全? → buf4/bufIdx/bufCount 需加锁
 * Q3: 性能优化? → 增大内部缓冲区，减少 read4 调用次数(类似 BufferedReader)
 */
