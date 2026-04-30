/*
 * LeetCode 400: Nth Digit (第 N 位数字)
 *
 * 难度: Medium
 * 标签: Math, Binary Search
 *
 * 题目概述:
 *   在无穷整数序列 1,2,3,4,5,6,7,8,9,10,11,... 中找到第 n 位数字。
 *   例如: n=11 → 序列 "12345678910..." 的第 11 位是 '0'（属于数字 10）。
 *
 * 核心思路:
 *   数学分段定位。自然数按位数分段:
 *     1位数(1~9): 9个, 占 9 位
 *     2位数(10~99): 90个, 占 180 位
 *     d位数: 9*10^(d-1) 个, 占 d*9*10^(d-1) 位
 *   三步定位: (1)确定位数段 → (2)确定具体数字 → (3)确定该数的第几位
 *
 * 关键注意:
 *   - 必须使用 long 类型防止 digits * count 溢出
 *   - (n-1)/d 和 (n-1)%d 实现 1-based → 0-based 转换
 */

// =====================================================
// 解法1: 暴力拼接（演示用，会超时）
// 时间: O(n)  空间: O(n)
//
// 逐个把自然数转为字符串拼接，然后取第 n 位。
// 当 n = 2^31-1 时内存和时间都不可行，仅作为理解问题的起点。
// =====================================================
class Solution_Brute {
public:
    int findNthDigit(int n) {
        string s;
        for (int num = 1; (int)s.size() < n; num++) {
            s += to_string(num);
        }
        return s[n - 1] - '0';
    }
};

// =====================================================
// 解法2: 数学分段定位（推荐）
// 时间: O(log n)  空间: O(log n) [to_string 的空间]
//
// 分三步:
//   Step 1: 确定 n 落在几位数段
//           逐段减去 digits * count，直到剩余的 n 不够减
//   Step 2: 确定是该段的第几个数
//           num = start + (n-1) / digits
//   Step 3: 确定是该数的第几位
//           idx = (n-1) % digits，取 to_string(num)[idx]
//
// 详细推导:
//   d位数段:
//     数字范围: [10^(d-1), 10^d - 1]
//     数字个数: count = 9 * 10^(d-1)
//     占用位数: digits * count = d * 9 * 10^(d-1)
//     起始数字: start = 10^(d-1)
//
//   手算 n = 187:
//     d=1: 9 位, 187-9 = 178 → 继续
//     d=2: 180 位, 178 < 180 → n 落在 2 位数段
//     num = 10 + (178-1)/2 = 10 + 88 = 98
//     idx = (178-1)%2 = 1
//     "98"[1] = '8' → 答案 8
// =====================================================
class Solution {
public:
    int findNthDigit(int n) {
        // digits: 当前段的位数 (1, 2, 3, ...)
        // count:  当前段的数字个数 (9, 90, 900, ...)
        // start:  当前段的起始数字 (1, 10, 100, ...)
        long digits = 1;
        long count = 9;
        long start = 1;

        // ---- Step 1: 确定 n 落在几位数段 ----
        // 循环条件: n 大于当前段的总位数，则减掉并进入下一段
        // 注意: digits * count 可能溢出 int，必须用 long
        while (n > digits * count) {
            n -= digits * count;
            digits++;
            count *= 10;   // 9 → 90 → 900 → ...
            start *= 10;   // 1 → 10 → 100 → ...
        }
        // 此时 n 是该段内的第 n 位（1-based）

        // ---- Step 2: 确定是该段的第几个数 ----
        // (n-1)/digits: 0-based 的偏移量
        // 例如 n=1 → (1-1)/d = 0 → 就是 start 本身
        long num = start + (n - 1) / digits;

        // ---- Step 3: 确定是该数的第几位 ----
        // (n-1)%digits: 0-based 的位内索引
        int idx = (n - 1) % digits;
        string s = to_string(num);
        return s[idx] - '0';
    }
};

// =====================================================
// 解法3: 二分查找变体（面试加分项）
// 时间: O(log^2 n)  空间: O(1)
//
// 思路: 二分搜索目标数字 num，辅助函数计算 1~num 总共占多少位。
// 找最小的 num 使得 totalDigits(num) >= n，再定位到具体位。
//
// totalDigits(num) 的计算:
//   假设 num 是 d 位数 (d = len(num)):
//   1位数贡献: min(num, 9) * 1
//   2位数贡献: min(num, 99) - 9 如果 > 0，乘以 2
//   ...
//   d位数贡献: (num - 10^(d-1) + 1) * d
// =====================================================
class Solution_BinarySearch {
public:
    int findNthDigit(int n) {
        // 计算从 1 到 num 拼接后的总位数
        auto totalDigits = [](long num) -> long {
            long total = 0;
            long d = 1, start = 1, end = 9;
            while (start <= num) {
                long cur_end = min(num, end);
                total += (cur_end - start + 1) * d;
                d++;
                start = end + 1;
                end = end * 10 + 9;
            }
            return total;
        };

        // 二分查找: 找最小的 num 使得 totalDigits(num) >= n
        long lo = 1, hi = (long)n; // num 不会超过 n
        while (lo < hi) {
            long mid = lo + (hi - lo) / 2;
            if (totalDigits(mid) >= n) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        // lo 就是目标数字
        // n 在 lo 中的位内偏移
        long prev = totalDigits(lo - 1); // lo-1 之前总共占多少位
        int offset = n - prev - 1;       // 0-based 的位内索引
        string s = to_string(lo);
        return s[offset] - '0';
    }
};

/*
 * 复杂度分析:
 *   解法2（数学分段）:
 *     时间: O(log_10(n)) — while 循环最多 ~10 次
 *     空间: O(log_10(n)) — to_string 占用
 *
 *   解法3（二分查找）:
 *     时间: O(log(n) * log_10(n)) — 二分 O(log n) 次，每次计算 totalDigits O(log_10 n)
 *     空间: O(log_10(n)) — to_string 占用
 *
 * 易错点:
 *   1. 溢出: digits * count 在 d=9 时约 81 亿，必须用 long
 *   2. 1-based: n 是 1-based，(n-1)/d 和 (n-1)%d 完成转换
 *   3. start 初始值: 1位数段从 1 开始（不是 0）
 *   4. 字符转数字: s[idx] - '0'，不要忘记减 '0'
 *
 * 面试技巧:
 *   - 先画位数表: 1位(9个,9位), 2位(90个,180位), 3位(900个,2700位)
 *   - 手算一个例子（如 n=187）走完三步，面试官就能跟上你的思路
 *   - 提到 long 溢出风险是加分项
 */

// ============================================================
// 总结
// ============================================================
/*
 * -------- 解法对比一览 --------
 *
 * | 解法         | 时间           | 空间        | 适用场景           | 面试推荐 |
 * |-------------|----------------|-------------|-------------------|---------|
 * | 暴力拼接     | O(n)           | O(n)        | 小 n 演示用，会TLE | 起手     |
 * | 数学分段定位 | O(log10 n)     | O(log10 n)  | 标准做法           | ⭐首选  |
 * | 二分查找     | O(log n*log10n)| O(log10 n)  | 展示二分思维       | 加分项   |
 *
 * -------- 易错点 --------
 *
 * 1. 整型溢出 -- digits*count 在 d=9 时约 81 亿，必须用 long
 *    这是本题最高频 WA 原因
 * 2. 1-based 转 0-based -- (n-1)/d 和 (n-1)%d 是关键公式
 *    写成 n/d 和 n%d 全错
 * 3. start 从 1 开始 -- 1 位数段起始是 1 不是 0
 *    (自然数序列不含 0)
 * 4. 字符转数字 -- s[idx]-'0' 别忘减 '0'
 * 5. 分段表要记牢 -- d 位数: 个数 9*10^(d-1), 占位 d*9*10^(d-1)
 *    面试时先画这个表再写代码
 *
 * -------- 面试追问链 --------
 *
 * Q1: 如果序列从 0 开始 (0,1,2,...) 怎么改?
 *     -> 1 位数段变成 10 个(0~9)占 10 位，start 从 0 开始
 *        或者直接 n++ 转化为从 1 开始的问题
 *
 * Q2: 如果要返回第 n 位所在的完整数字而非某一位?
 *     -> Step 1+2 不变，直接返回 num 即可，省掉 Step 3
 *
 * Q3: 给定数字 x，它的各位数字分别在序列中的什么位置?
 *     -> 反向计算: 先算 x 之前所有数字占的总位数 offset
 *        x 的第 i 位(0-based)在序列的第 offset+i+1 位
 *        totalDigits(x-1) + i + 1
 */
