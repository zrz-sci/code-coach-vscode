/*
 * 275. H-Index II
 *
 * 【题目本质】
 * 已排序数组中二分查找 h-index。
 * 核心：找最左的位置 mid，使得 citations[mid] >= n - mid。
 * 答案 = n - mid，表示从 mid 到末尾有 n-mid 篇论文，每篇引用 >= n-mid。
 *
 * 【解法总览】
 * Solution1: 二分查找(找左边界) — O(log n) 时间, O(1) 空间 [推荐]
 * Solution2: 线性扫描 — O(n) 时间, O(1) 空间 (不满足题目 O(log n) 要求)
 */

// ===================== Solution 1: 二分查找 =====================
// 思路: 在已排序数组中，找最左的 mid 使得 citations[mid] >= n - mid
// 时间 O(log n), 空间 O(1)
class Solution1 {
public:
    int hIndex(vector<int>& citations) {
        int n = citations.size();
        int lo = 0, hi = n - 1;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (citations[mid] >= n - mid) {
                // 满足条件，尝试向左找更大的 h
                hi = mid - 1;
            } else {
                // 不满足，向右找
                lo = mid + 1;
            }
        }

        // lo 是第一个满足 citations[lo] >= n-lo 的位置
        // h-index = n - lo
        return n - lo;
    }
};

// ===================== Solution 2: 线性扫描 =====================
// 思路: 从左到右扫描，找第一个 citations[i] >= n-i 的位置
// 时间 O(n), 空间 O(1)
// 注意: 不满足题目 O(log n) 的要求，仅作对比
class Solution2 {
public:
    int hIndex(vector<int>& citations) {
        int n = citations.size();
        for (int i = 0; i < n; i++) {
            // 从位置 i 到末尾有 n-i 篇论文
            // 如果 citations[i] >= n-i，说明这 n-i 篇都引用 >= n-i
            if (citations[i] >= n - i) {
                return n - i;
            }
        }
        return 0;  // 所有论文引用都为 0
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | Solution1 二分查找 | O(log n) | O(1) | 满足题目要求，最优 |
 * | Solution2 线性扫描 | O(n) | O(1) | 简单直观，但不满足要求 |
 *
 * 【易错点】
 * 1. 返回值搞混
 *    ✗ return lo;  (lo 是索引)
 *    ✓ return n - lo;  (n - lo 才是 h-index)
 *
 * 2. 二分条件方向反了
 *    ✗ citations[mid] >= n - mid → lo = mid + 1 (向右找)
 *    ✓ citations[mid] >= n - mid → hi = mid - 1 (向左找更大的h)
 *    目标是找最左满足位置，满足时应收缩右边界
 *
 * 3. 边界: 全部不满足
 *    lo = n, 返回 n - n = 0 ✓
 *
 * 4. 边界: 全部满足
 *    hi = -1, lo = 0, 返回 n - 0 = n ✓
 *
 * 【面试追问】
 * Q1: 与 274 H-Index 的区别?
 *     --> 274 未排序，需 O(n log n) 排序或 O(n) 计数排序
 *     --> 275 已排序，直接 O(log n) 二分
 *
 * Q2: 为什么找最左位置对应最大 h?
 *     --> mid 越小，n-mid 越大，h-index 越大
 *     --> 第一个满足条件的位置给出最大 h
 *
 * Q3: h-index 的实际应用?
 *     --> 学术界衡量研究影响力的指标
 *     --> Google Scholar 等平台自动计算
 */
