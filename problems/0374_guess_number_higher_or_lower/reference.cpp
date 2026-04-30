/*
 * 374. Guess Number Higher or Lower
 *
 * 【题目本质】
 * 在 [1, n] 的有序空间中，通过 guess() API 进行二分搜索找到目标数。
 * 最基础的交互式二分查找题。
 *
 * 【解法总览】
 * Solution1: 标准二分查找 — lo/hi 逼近               O(log n) / O(1)
 * Solution2: 三分查找 — 每次排除 1/3 区间             O(log₃ n) / O(1)
 *
 * 【API 说明】
 * int guess(int num):
 *   返回 -1: num > pick (你猜大了)
 *   返回  1: num < pick (你猜小了)
 *   返回  0: num == pick (猜对了)
 */

// ===================== Solution1: 标准二分查找（推荐）=====================
// 经典 lo/hi 二分，每次取中点调用 guess()
// 时间 O(log n)，空间 O(1)
class Solution1 {
public:
    int guessNumber(int n) {
        int lo = 1, hi = n;
        while (lo <= hi) {
            // 防溢出写法
            int mid = lo + (hi - lo) / 2;
            int res = guess(mid);
            if (res == 0) {
                return mid;        // 猜对了
            } else if (res == -1) {
                hi = mid - 1;      // mid 太大，往左找
            } else {
                lo = mid + 1;      // mid 太小，往右找
            }
        }
        return -1; // 题目保证有解，不会执行到这里
    }
};

// ===================== Solution2: 三分查找 =====================
// 将区间分为三段，每次最多排除 2/3
// 理论 O(log₃ n) 但每轮可能调用两次 guess()，实际不一定更快
// 时间 O(log₃ n)，空间 O(1)
class Solution2 {
public:
    int guessNumber(int n) {
        long lo = 1, hi = n;
        while (lo <= hi) {
            long mid1 = lo + (hi - lo) / 3;
            long mid2 = hi - (hi - lo) / 3;
            int r1 = guess((int)mid1);
            if (r1 == 0) return (int)mid1;
            int r2 = guess((int)mid2);
            if (r2 == 0) return (int)mid2;

            if (r1 == -1) {
                hi = mid1 - 1;       // 在左 1/3
            } else if (r2 == 1) {
                lo = mid2 + 1;       // 在右 1/3
            } else {
                lo = mid1 + 1;       // 在中间 1/3
                hi = mid2 - 1;
            }
        }
        return (int)lo;
    }
};

/*
 * 【解法对比】
 * | 解法     | 时间     | 空间 | API调用次数    | 优点              |
 * |----------|----------|------|---------------|-------------------|
 * | Solution1| O(log n) | O(1) | 最多 log₂(n)  | 简洁标准，最少调用  |
 * | Solution2| O(log₃n) | O(1) | 最多 2*log₃(n)| 理论轮次少但调用多  |
 *
 * 推荐 Solution1：代码简洁、API 调用次数最少、不易出错。
 *
 * 【易错点】
 * 1. mid 溢出：(lo + hi) / 2 对大 n 溢出 → 用 lo + (hi - lo) / 2
 * 2. API 语义搞反：-1 是猜大了不是猜小了，非常容易记反
 * 3. 循环条件：必须是 lo <= hi（包含等号），否则漏掉单元素区间
 * 4. 边界更新遗漏排除 mid：猜错时 mid 已排除，必须 ±1
 *
 * 【面试追问】
 * Q1: n 为 INT_MAX 时有什么注意事项？
 * A1: mid 计算必须用防溢出写法。lo + (hi - lo) / 2 保证不溢出。
 *
 * Q2: 如果答案不存在（pick 不在 [1,n] 范围）如何处理？
 * A2: 题目保证 1 <= pick <= n，实际中应在循环结束后返回 -1 表示未找到。
 *
 * Q3: 二分查找有哪些变体模板？
 * A3: 三种常见模板：(1) lo<=hi 找精确值; (2) lo<hi 找边界;
 *     (3) lo+1<hi 避免死循环。本题用模板(1)最合适。
 */
