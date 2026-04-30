/*
 * 【题目本质】
 * 给定有序攻击时间点数组和中毒持续时间，计算总中毒秒数。
 * 关键在于处理相邻攻击的区间重叠。
 *
 * 【解法总览】
 * 1. 线性扫描(间隔法) — O(n), 每次贡献 min(gap, duration)
 * 2. 区间合并思路 — O(n), 维护当前结束时间
 */

// ===================== Solution 1: 线性扫描(间隔法) =====================
// 时间: O(n)  空间: O(1)
// 核心: 每对相邻攻击贡献 min(间隔, duration), 最后 + duration
class Solution1 {
public:
    int findPoisonedDuration(vector<int>& timeSeries, int duration) {
        if (timeSeries.empty()) return 0;
        int total = 0;
        for (int i = 1; i < (int)timeSeries.size(); i++) {
            total += min(timeSeries[i] - timeSeries[i - 1], duration);
        }
        return total + duration;
    }
};

// ===================== Solution 2: 区间合并思路 =====================
// 时间: O(n)  空间: O(1)
// 核心: 维护中毒结束时间 end, 根据新攻击是否在 end 前决定贡献
class Solution2 {
public:
    int findPoisonedDuration(vector<int>& timeSeries, int duration) {
        if (timeSeries.empty()) return 0;
        int total = 0;
        int end = timeSeries[0] + duration;
        for (int i = 1; i < (int)timeSeries.size(); i++) {
            if (timeSeries[i] < end) {
                // 重叠: 只贡献间隔部分
                total += timeSeries[i] - timeSeries[i - 1];
            } else {
                // 不重叠: 贡献完整 duration
                total += duration;
            }
            end = timeSeries[i] + duration;
        }
        return total + duration; // 最后一次攻击
    }
};

/*
 * 【解法对比】
 * Solution1: 代码最简洁, min(gap, duration) 一行搞定
 * Solution2: 更贴近区间合并的思维模型, 扩展性好
 *
 * 【易错点】
 * 1. 忘记最后一次攻击的 duration -> 循环后要 + duration
 * 2. 数组越界: 应从 i=1 开始, 用 timeSeries[i]-timeSeries[i-1]
 * 3. timeSeries 为空时需特判返回 0
 *
 * 【面试追问】
 * Q1: 攻击无序? — 先排序 O(n log n)
 * Q2: 多种毒素不同持续? — 区间合并: 生成所有区间, 排序合并
 * Q3: O(1) 判断时刻 t 是否中毒? — 二分找最近攻击, 检查 t < ts[i] + d
 */
