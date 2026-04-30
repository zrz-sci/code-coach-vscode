/*
 * LeetCode 551: Student Attendance Record I (学生出勤记录 I)
 * 难度: Easy
 * 标签: String
 *
 * =============================================
 * 解法一: 一次遍历 (推荐)
 * =============================================
 *
 * 核心思路:
 *   遍历字符串, 维护两个计数器:
 *   - absentCount: 累计缺勤次数 (全局, 不重置)
 *   - lateStreak:  当前连续迟到次数 (遇到非 'L' 重置为 0)
 *   任一条件违反则立即返回 false
 *
 * 时间: O(n) — 一次遍历
 * 空间: O(1) — 两个计数器
 */

class Solution {
public:
    bool checkRecord(string s) {
        int absentCount = 0;   // 累计缺勤次数
        int lateStreak = 0;    // 当前连续迟到次数

        for (char c : s) {
            if (c == 'A') {
                absentCount++;
                // 缺勤达到 2 次, 不合格
                if (absentCount >= 2) return false;
                // 非 'L', 重置连续迟到计数
                lateStreak = 0;
            } else if (c == 'L') {
                lateStreak++;
                // 连续迟到达到 3 次, 不合格
                if (lateStreak >= 3) return false;
            } else {
                // 'P': 到场, 重置连续迟到
                lateStreak = 0;
            }
        }
        return true;
    }
};

/*
 * trace: s = "PPALLP"
 *   P → absent=0, streak=0
 *   P → absent=0, streak=0
 *   A → absent=1, streak=0 (重置)
 *   L → absent=1, streak=1
 *   L → absent=1, streak=2
 *   P → absent=1, streak=0 (重置)
 *   → return true ✓
 *
 * trace: s = "PPALLL"
 *   P → absent=0, streak=0
 *   P → absent=0, streak=0
 *   A → absent=1, streak=0
 *   L → absent=1, streak=1
 *   L → absent=1, streak=2
 *   L → absent=1, streak=3 >= 3 → return false ✓
 *
 * trace: s = "AAPLL"
 *   A → absent=1, streak=0
 *   A → absent=2 >= 2 → return false ✓
 */


/*
 * =============================================
 * 解法二: 子串查找 (简洁写法)
 * =============================================
 *
 * 利用 STL 的 count 和 find:
 *   count(A) < 2: 缺勤少于 2 次
 *   find("LLL") == npos: 不含连续 3 个 L
 *
 * 优点: 代码极简, 一行搞定
 * 缺点: 遍历了两次 (count 一次, find 一次)
 */

class Solution2 {
public:
    bool checkRecord(string s) {
        return count(s.begin(), s.end(), 'A') < 2
            && s.find("LLL") == string::npos;
    }
};

/*
 * 注意: 解法二虽然简洁, 但实际遍历了两次字符串
 * 面试中推荐解法一 (一次遍历, 效率更优, 展示了编码能力)
 */


/*
 * =============================================
 * 解法三: 提前终止优化版
 * =============================================
 *
 * 和解法一逻辑相同, 但用不同的分支结构
 * 先判断是否为 L, 再处理非 L 的情况
 * 这种写法把 lateStreak 的重置放在一个分支里
 */

class Solution3 {
public:
    bool checkRecord(string s) {
        int aCount = 0, lStreak = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            if (s[i] == 'L') {
                lStreak++;
                if (lStreak >= 3) return false;
            } else {
                lStreak = 0;
                if (s[i] == 'A') {
                    aCount++;
                    if (aCount >= 2) return false;
                }
            }
        }
        return true;
    }
};


/*
 * =============================================
 * 解法对比
 * =============================================
 *
 * | 解法       | 时间 | 空间 | 遍历次数 | 适用场景   |
 * |-----------|------|------|---------|----------|
 * | 一次遍历   | O(n) | O(1) | 1       | 面试推荐  |
 * | 子串查找   | O(n) | O(1) | 2       | 快速编码  |
 * | 提前终止版 | O(n) | O(1) | 1       | 同解法一  |
 *
 *
 * =============================================
 * 易错点
 * =============================================
 *
 * 1. >= 而非 >: "少于 2 天" 即 absentCount >= 2 就 false
 *    ✗ absentCount > 2     ✓ absentCount >= 2
 *
 * 2. lateStreak 重置时机: 遇到 'A' 也要重置, 不只是 'P'
 *    ✗ 只在 c=='P' 时重置   ✓ 只要 c!='L' 都重置
 *
 * 3. 连续 >= 3 而非 > 3: "连续 3 天或以上" 即 lateStreak >= 3 就 false
 *    ✗ lateStreak > 3      ✓ lateStreak >= 3
 *
 * 4. 两个条件是 AND 关系: 任一违反就 false, 不是两个都违反
 *    ✗ absentCount >= 2 && lateStreak >= 3
 *    ✓ absentCount >= 2 || lateStreak >= 3
 *
 *
 * =============================================
 * 面试追问
 * =============================================
 *
 * Q1: 如果条件泛化为 "缺勤 < k 天, 连续迟到 < m 天"?
 *     → 只需将 2 改为 k, 3 改为 m, 算法不变
 *
 * Q2: 和 552 (Student Attendance Record II) 的关系?
 *     → 551 是"检查一条记录", 552 是"计数所有合法记录"
 *     → 552 用 DP: dp[天数][A次数][连续L次数]
 *
 * Q3: 如果字符串极长 (10^9), 如何优化?
 *     → O(n) 一次遍历已是最优, 无法更快
 *     → 可以考虑提前终止 (两个条件任一违反后立即返回)
 *     → 如果允许并行, 可分段统计 A 个数和最大连续 L 长度
 *
 * 关联题目:
 *   552. Student Attendance Record II — DP 计数版
 *   485. Max Consecutive Ones — 类似连续计数
 *   1446. Consecutive Characters — 类似连续计数
 *   1004. Max Consecutive Ones III — 滑动窗口 + 连续计数
 *   674. Longest Continuous Increasing Subsequence — 连续计数
 */
