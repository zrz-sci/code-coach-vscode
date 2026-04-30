/*
 * LeetCode 875: 爱吃香蕉的珂珂 (Koko Eating Bananas)
 *
 * 【题目本质】
 * 在答案空间 [1, max(piles)] 上找满足"总耗时 ≤ h"的最小速度 k。
 * 因为 totalHours(k) 关于 k 单调递减，所以可以二分搜索答案。
 *
 * 【解法总览】
 * 解法1: 暴力枚举      — O(max × n) / O(1) — 最直觉但超时
 * 解法2: 二分搜索答案   — O(n × log(max)) / O(1) — 面试必须写出
 */

// ============================================================
// 解法1: 暴力枚举 — 从 k=1 逐一检查
// 时间: O(max(piles) × n)  空间: O(1)
//
// 【思路】
// 最朴素的想法：从最慢速度 k=1 开始，对每个 k 计算吃完所有堆
// 需要的总小时数。第一个满足 totalHours <= h 的 k 就是答案。
//
// 每堆需要 ceil(piles[i] / k) 小时，总时间 = 所有堆的小时数之和。
//
// 瓶颈：k 的范围 [1, 10^9]，线性枚举太慢。但这个解法的核心——
// 给定 k 计算 totalHours 的函数——会被解法2直接复用。
// ============================================================
class Solution1 {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int maxPile = *max_element(piles.begin(), piles.end());
        
        for (int k = 1; k <= maxPile; k++) {
            long long totalHours = 0;
            for (int pile : piles) {
                // 整数上取整: ceil(pile / k) = (pile + k - 1) / k
                totalHours += (pile + k - 1) / k;
            }
            // 第一个满足条件的 k 就是最小速度
            if (totalHours <= h) {
                return k;
            }
        }
        return maxPile;
    }
};

// ============================================================
// 解法2: 二分搜索答案 — 在 [1, max(piles)] 上二分
// 时间: O(n × log(max(piles)))  空间: O(1)
//
// 【思路】
// 解法1的瓶颈：线性枚举 k 从 1 到 max(piles)，最坏 10^9 次。
//
// 关键观察：totalHours(k) 是 k 的单调递减函数。
//   - k 越大 → 每堆花的时间越少 → 总时间越少
//   - 存在一个分界点：左边不满足(太慢)，右边满足(够快)
//   - 我们要找的就是这个分界点（最左边的满足条件的 k）
//
// 这就是经典的"二分搜索答案"：
//   搜索空间: [1, max(piles)]
//   判断条件: 以速度 mid 吃完所有堆是否 ≤ h 小时
//   收缩策略: 满足→往左找更小的, 不满足→往右找更大的
//
// 搜索过程可视化 (piles = [3,6,7,11], h = 8):
//
// Round1: [1 .............. 11]   mid=6  → hours=6 ≤ 8  → right=6
//                    ↑
// Round2: [1 ... 6]               mid=3  → hours=10 > 8 → left=4
//              ↑
// Round3: [4 . 6]                 mid=5  → hours=8 ≤ 8  → right=5
//             ↑
// Round4: [4 5]                   mid=4  → hours=8 ≤ 8  → right=4
//          ↑
// Round5: [4]                     left==right → 答案=4 ✓
//
// 验证 k=4: ceil(3/4)+ceil(6/4)+ceil(7/4)+ceil(11/4) = 1+2+2+3 = 8 ≤ 8 ✓
// 验证 k=3: ceil(3/3)+ceil(6/3)+ceil(7/3)+ceil(11/3) = 1+2+3+4 = 10 > 8 ✗
// ============================================================
class Solution2 {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int left = 1;
        int right = *max_element(piles.begin(), piles.end());
        
        // 二分搜索：找满足 totalHours <= h 的最小 k
        // 用 left < right 模板，因为 right = mid（不减一）
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            // 核心函数：以速度 mid 吃完所有堆需要多少小时
            long long hours = 0;
            for (int pile : piles) {
                // 整数上取整，避免浮点: ceil(a/b) = (a+b-1)/b
                hours += (long long)(pile + mid - 1) / mid;
            }
            
            if (hours <= h) {
                // mid 速度够快（甚至可以更慢），向左收缩寻找更小的 k
                // mid 本身可能就是答案，所以 right = mid（不排除）
                right = mid;
            } else {
                // mid 速度太慢，吃不完，必须更快
                // mid 一定不是答案，所以 left = mid + 1（排除）
                left = mid + 1;
            }
        }
        
        // 循环结束时 left == right，就是最小满足条件的速度
        return left;
    }
};

// ============================================================
// 解法2 变体: 抽取判断函数，更清晰
// 和解法2 完全相同的逻辑，只是把"计算总小时数"抽成独立函数
// 面试时推荐这种写法——结构更清晰，容易讨论和修改
// ============================================================
class Solution2b {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int left = 1;
        int right = *max_element(piles.begin(), piles.end());
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (canFinish(piles, mid, h)) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        return left;
    }
    
private:
    // 判断以速度 k 能否在 h 小时内吃完所有堆
    bool canFinish(vector<int>& piles, int k, int h) {
        long long hours = 0;
        for (int pile : piles) {
            hours += (long long)(pile + k - 1) / k;
            // 优化：一旦超过 h，提前返回，避免不必要的累加
            if (hours > h) return false;
        }
        return hours <= h;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间               | 空间 | 适用场景           |
// |------------|--------------------|------|--------------------|
// | 暴力枚举   | O(max × n) ≈ 10^13 | O(1) | 理解题意，不能AC   |
// | 二分答案   | O(n × log(max))     | O(1) | 面试必须写出       |
// | 二分(抽函数)| 同上               | O(1) | 面试推荐，更清晰   |
//
// 核心区别: 
//   暴力线性扫描答案空间 → 二分将搜索次数从 10^9 降到 ~30 次(log2(10^9)≈30)
//   每次检验 O(n)，总计 O(30n) ≈ 3×10^5，轻松通过。
//
// ============================================================
// 【易错点】
//
// 1. 上取整写成下取整:
//    ✗ hours += pile / k;           → 7/4=1，但实际需要2小时
//    ✓ hours += (pile + k - 1) / k; → (7+3)/4=2 ✓
//
// 2. 右边界错误:
//    ✗ right = piles.size();     → 这是堆的数量，不是速度的上界
//    ✗ right = INT_MAX;          → 没必要这么大，且可能导致 mid 溢出
//    ✓ right = *max_element(...); → 速度最大到最大堆即可
//
// 3. hours 累加溢出:
//    ✗ int hours = 0;    → 当 k=1 时 hours=sum(piles)，piles[i]可达10^9，
//                           共10^4堆，sum可达10^13，超int
//    ✓ long long hours = 0; 或者在累加过程中一旦>h就提前break
//
// 4. 二分收缩方向搞反:
//    ✗ if (hours <= h) left = mid + 1; → 找的是"刚好不满足"的位置
//    ✓ if (hours <= h) right = mid;    → 满足时向左收缩，找最小的满足值
//
// 5. 死循环:
//    使用 left < right + right = mid 模板时不会死循环
//    如果误写成 left <= right + right = mid → left==right 时进入循环
//    且 mid=left=right, right=mid 不缩小区间 → 死循环！
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 暴力解的复杂度是多少？能优化到什么程度？
//   A: 暴力 O(max × n)，瓶颈是线性枚举 k。利用 totalHours(k) 的单调性，
//      用二分搜索将枚举从 O(max) 降到 O(log(max))，总 O(n log(max))。
//
// Q2: 如果 piles 非常长(10^8)但 max(piles) 很小(100)，哪个解法更好？
//   A: 此时 log(max) ≈ 7 很小，但 n=10^8 很大。二分仍然最优 O(7 × 10^8)。
//      暴力是 O(100 × 10^8) = O(10^10)，差很多。
//      如果追问"能否减少每次检验的 O(n)"——可以预排序 + 前缀和优化，
//      但一般面试不要求。
//
// Q3: 如何验证你的二分写法是否正确？
//   A: 检查三点:
//      1) 搜索空间是否覆盖所有可能答案 [1, max]
//      2) 每次迭代区间是否严格缩小（left<right 保证）
//      3) 最终 left==right 是否就是答案（验证 canFinish(left, h)==true
//         且 canFinish(left-1, h)==false）
//
// Q4: 1011题"在D天内送达包裹的能力"和这题有什么区别？
//   A: 结构几乎一模一样。搜索空间从"吃的速度"变成"船的载重量"，
//      判断函数从 sum(ceil(pile/k)) 变成贪心装箱（按顺序装，装满换天）。
//      二分框架完全复用，只改 canFinish 函数。
//
// ============================================================
// 【相关题型】
//
// - 1011. 在 D 天内送达包裹的能力
//     复用: 完全相同的二分答案框架，只改判断函数（贪心分天）
//
// - 410. 分割数组的最大值
//     复用: 二分答案框架，搜索空间是"子数组和的最大值"
//     区别: 判断函数是"能否分成≤m组使每组和≤mid"
//
// - 69. x 的平方根
//     复用: 最简单的二分答案，入门级别，搜索空间 [0,x]
//
// - 1482. 制作 m 束花所需的最少天数
//     复用: 二分答案框架，搜索空间是"天数"
// ============================================================
