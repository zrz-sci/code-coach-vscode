/*
 * 【题目本质】
 * 用每次只读4字符的 read4 API 实现读取 n 字符的 read 函数。
 * 核心是"循环调用 read4 + 截断多余字符"。本题 read 只调用一次。
 *
 * 【解法总览】
 * 1. 循环调用 read4: O(n) 时间, O(1) 空间
 *
 * 【数据流图示】
 *
 *   file: "a b c d e f g"
 *          ↓ ↓ ↓ ↓
 *   read4 → buf4 = "a b c d"  (返回4)
 *                    ↓ 拷贝到 buf
 *          ↓ ↓ ↓ ↓
 *   read4 → buf4 = "e f g _"  (返回3, 文件结束)
 *                    ↓ 拷贝到 buf (只取需要的部分)
 *
 *   buf: "a b c d e"  (如果 n=5，只拷贝1个)
 */

// ===================== 解法1: 循环调用 read4 =====================
// 思路: 每次用 read4 读一批，按需截断拷贝到 buf
// 为什么需要 min(count, n-total)? 防止最后一批读多了
class Solution1 {
public:
    int read(char *buf, int n) {
        int total = 0;     // 已写入 buf 的字符数
        char buf4[4];      // read4 的临时缓冲区

        while (total < n) {
            // 调用底层API读取最多4个字符
            int count = read4(buf4);

            // 实际需要拷贝的字符数: 不超过剩余需求
            // 为什么取 min? 比如 n=5, 第二次 read4 返回4，但只需要1个
            int toRead = min(count, n - total);

            // 从 buf4 拷贝到目标 buf
            for (int i = 0; i < toRead; i++) {
                buf[total++] = buf4[i];
            }

            // 为什么 count < 4 就退出?
            // read4 返回 < 4 说明文件已经没有更多字符了
            if (count < 4) break;
        }

        return total;
    }
};

// ===================== 解法1 变体: 使用 memcpy =====================
// 思路: 用 memcpy 替代逐字符拷贝，在系统编程中更常见
class Solution2 {
public:
    int read(char *buf, int n) {
        int total = 0;
        char buf4[4];

        while (total < n) {
            int count = read4(buf4);
            int toRead = min(count, n - total);

            // 为什么用 memcpy? 底层是按字节拷贝，比循环更高效
            // 注意: buf + total 是拷贝的目标位置
            memcpy(buf + total, buf4, toRead);
            total += toRead;

            if (count < 4) break;
        }

        return total;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 说明 |
 * |------|------|------|------|
 * | 解法1 | O(n) | O(1) | 逐字符拷贝，逻辑清晰 |
 * | 解法2 | O(n) | O(1) | memcpy 更贴近系统编程 |
 *
 * 【易错点】
 * 1. 忘记 min(count, n-total) → 可能写入超过 n 个字符
 * 2. 没有 break 在 count < 4 时 → 文件结束后继续循环
 * 3. buf4 声明为指针但没分配空间 → 段错误
 *
 * 【面试追问】
 * Q1: 多次调用怎么办? → 需要保存 buf4 剩余数据(LC 158)
 * Q2: read4 改为 readK? → 把4替换为k，逻辑不变
 * Q3: 如何测试? → n=0, 文件空, n>文件长度, n是/不是4的倍数
 */
