/*
 * LeetCode 1431: Kids With the Greatest Number of Candies
 * (拥有最多糖果的孩子)
 *
 * 【题目本质】
 * 给定糖果数组和额外糖果数，对每个孩子判断：把额外糖果全给他后，
 * 他的糖果数是否 >= 原始数组的最大值。
 * 关键洞察：额外糖果给的是"假设性"操作，每次独立，不影响其他人。
 *
 * 【解法总览】
 * 解法1: 暴力枚举          — O(n^2) / O(1)   — 理解题意
 * 解法2: 先求最大值再判断   — O(n)   / O(1)   — 面试首选 ⭐
 * 解法3: STL transform     — O(n)   / O(1)   — 展示 C++ 功底
 */


// ============================================================
// 解法1: 暴力枚举 — 对每个孩子，遍历数组看是否是最大
// 时间: O(n^2)  空间: O(1)
//
// 【思路】
// 最直觉的做法：对于第 i 个孩子，把 extraCandies 给他，
// 然后遍历所有孩子检查是否有人比他多。
//
// 【为什么可以优化？】
// 每次内层循环都在找同一个值 — 数组最大值。
// 而这个最大值不随 i 变化（假设性操作是独立的），
// 所以可以提前算好，避免重复计算。
// ============================================================
class Solution1 {
public:
    vector<bool> kidsWithCandies(vector<int>& candies, int extraCandies) {
        int n = candies.size();
        vector<bool> result(n);

        for (int i = 0; i < n; i++) {
            // 假设把 extraCandies 全给第 i 个孩子
            int total = candies[i] + extraCandies;

            // 检查是否 >= 所有人
            bool isGreatest = true;
            for (int j = 0; j < n; j++) {
                if (candies[j] > total) {
                    isGreatest = false;
                    break;  // 找到一个比他多的就够了
                }
            }
            result[i] = isGreatest;
        }
        return result;
    }
};

// ============================================================
// 解法2: 先求最大值再判断 — 两遍遍历，O(n) 解法
// 时间: O(n)  空间: O(1) (不计输出数组)
//
// 【核心观察】
// 判断条件: candies[i] + extraCandies >= maxCandy
// 其中 maxCandy 是原数组的最大值，是常量，只需算一次。
//
// 【具体过程】(candies=[2,3,5,1,3], extraCandies=3)
//
// 第一遍: 找最大值
//   candies = [2, 3, 5, 1, 3]
//                    ^
//             maxCandy = 5
//
// 第二遍: 逐个判断
//   i=0: 2+3=5  >= 5 ? true
//   i=1: 3+3=6  >= 5 ? true
//   i=2: 5+3=8  >= 5 ? true
//   i=3: 1+3=4  >= 5 ? false   <-- 唯一不够的
//   i=4: 3+3=6  >= 5 ? true
//
// 结果: [true, true, true, false, true]
//
// 【为什么 O(n) 是最优？】
// 至少需要看每个孩子一次来产出答案，下界就是 O(n)。
// ============================================================
class Solution2 {
public:
    vector<bool> kidsWithCandies(vector<int>& candies, int extraCandies) {
        // Step 1: O(n) 找最大值
        int maxCandy = *max_element(candies.begin(), candies.end());

        // Step 2: O(n) 逐个判断
        vector<bool> result;
        result.reserve(candies.size());  // 预分配避免多次扩容

        for (int c : candies) {
            // 关键判断: 加上额外糖果后是否 >= 最大值
            // 注意: >= 而非 >，因为题目允许多人并列最多
            result.push_back(c + extraCandies >= maxCandy);
        }
        return result;
    }
};

// ============================================================
// 解法3: STL transform — 函数式风格，一行完成映射
// 时间: O(n)  空间: O(1) (不计输出数组)
//
// 【思路】
// 用 STL 的 transform 算法把"对每个元素的判断"写成函数式映射。
// transform(begin, end, out, func) 对 [begin, end) 中每个元素
// 应用 func，结果写入 out。
//
// 这种写法在面试中可以作为"更优雅的版本"展示，
// 体现对 C++ STL 的熟练度。
//
// 【等价的 Python 写法】
//   return [c + extraCandies >= mx for c in candies]
// ============================================================
class Solution3 {
public:
    vector<bool> kidsWithCandies(vector<int>& candies, int extraCandies) {
        int mx = *max_element(candies.begin(), candies.end());

        vector<bool> result(candies.size());
        transform(candies.begin(), candies.end(), result.begin(),
                  [mx, extraCandies](int c) {
                      return c + extraCandies >= mx;
                  });
        return result;
    }
};

// ============================================================
// 【常见错误与陷阱】
//
// 错误1: 用 > 而非 >=
//   result.push_back(c + extraCandies > maxCandy);  // WRONG!
//   题目说"greatest"允许并列，应该用 >=
//
// 错误2: 修改原数组
//   candies[i] += extraCandies;  // WRONG! 影响后续判断
//   if (candies[i] >= maxCandy)...
//   正确做法: 用临时变量 c + extraCandies
//
// 错误3: 遗漏 extraCandies
//   result.push_back(c >= maxCandy);  // WRONG! 忘了加额外糖果
//
// 【面试 Follow-up】
// Q: 如果 extraCandies 可以分给多个孩子（每人可分不同数量），
//    怎么让最多人达到"拥有最多糖果"？
// A: 排序后贪心。先确定目标值（最大值），从差额最小的孩子开始
//    分配，直到额外糖果用完。类似 "分糖果" 系列问题。
//
// Q: 如果数组很长（10^8），需要并行处理怎么办？
// A: 第一步 max 可以用并行 reduce (Fork-Join)，
//    第二步判断是 embarrassingly parallel，直接分段。
//    C++17 可用 std::execution::par 策略:
//    auto mx = *max_element(execution::par, begin, end);
// ============================================================

// ============================================================
// 【总结】
//
// ——— 解法对比 ———
// | 解法              | 时间   | 空间  | 适用场景          |
// |------------------|--------|-------|------------------|
// | 暴力枚举          | O(n^2) | O(1)  | 理解题意          |
// | 先求max再判断      | O(n)   | O(1)  | 面试首选 ⭐       |
// | STL transform    | O(n)   | O(1)  | 展示C++功底       |
//
// 三种写法本质一样，都是 candies[i]+extra >= max。
// 暴力法重复计算 max，优化就是把它提出来。
//
// ——— 易错点 ———
// 1. >= vs > : 题目允许并列最多，必须用 >=
// 2. 修改原数组: candies[i] += extra 会影响后续判断，用临时变量
// 3. 独立假设: 每个孩子的判断互相独立，不是"分配"问题
//
// ——— 面试追问 ———
// Q1: 如果 extraCandies 可以拆分给多个孩子，最多几人达到最大？
//     → 排序+贪心。算每人到达当前 max 的差额，从小到大贪心分配。
//
// Q2: 如果糖果数是流式输入(streaming)，如何在线维护答案？
//     → 维护一个 running max。新元素进来：更新 max，然后对历史
//       中 candies[i]+extra < newMax 的翻转为 false（单调性：
//       max 只增不减，所以 true→false 不可逆，无需回溯）。
//
// Q3: 数组长度 10^8，单机内存放不下怎么办？
//     → 两遍扫描：第一遍流式求 max（O(1)空间），第二遍流式
//       输出每个判断结果。完全不需要把整个数组放入内存。
// ============================================================
