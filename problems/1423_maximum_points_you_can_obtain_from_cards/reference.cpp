/*
 * LeetCode 1423: Maximum Points You Can Obtain from Cards
 * 可获得的最大点数
 *
 * 难度: Medium
 * 标签: Array, Prefix Sum, Sliding Window
 *
 * 核心洞察: 从两端拿 k 张 = 中间留下 n-k 张连续子数组
 * max(两端之和) = total - min(中间窗口之和)
 *
 * 提供两种解法:
 *   解法一: 滑动窗口（反向思维）  O(n) / O(1)  [推荐]
 *   解法二: 前缀和枚举（正向思维） O(n) / O(1)
 */

// ======================== 解法一: 滑动窗口（反向思维） ========================
// 思路:
//   从两端取 k 张 <=> 中间剩 n-k 张连续子数组
//   最大化取走的 = total - 最小化留下的
//   用定长滑动窗口找长度为 n-k 的最小和子数组
//
// 窗口滑动过程示意 (cardPoints = [1,2,3,4,5,6,1], k=3):
//
//   windowSize = 7-3 = 4
//   total = 22
//
//   [1, 2, 3, 4] 5  6  1    windowSum=10  minWindow=10
//    1 [2, 3, 4, 5] 6  1    windowSum=14  minWindow=10
//    1  2 [3, 4, 5, 6] 1    windowSum=18  minWindow=10
//    1  2  3 [4, 5, 6, 1]   windowSum=16  minWindow=10
//
//   answer = 22 - 10 = 12

class Solution {
public:
    int maxScore(vector<int>& cardPoints, int k) {
        int n = cardPoints.size();
        int windowSize = n - k;

        // 计算数组总和
        int total = 0;
        for (int x : cardPoints) total += x;

        // 特判: k == n，取走所有卡牌
        if (windowSize == 0) return total;

        // 滑动窗口: 找长度恰好为 windowSize 的最小和子数组
        int windowSum = 0;
        int minWindow = INT_MAX;

        for (int i = 0; i < n; i++) {
            // 右端进入窗口
            windowSum += cardPoints[i];

            // 左端移出窗口（窗口超过 windowSize 时）
            if (i >= windowSize) {
                windowSum -= cardPoints[i - windowSize];
            }

            // 窗口填满后（i >= windowSize - 1），开始比较
            if (i >= windowSize - 1) {
                minWindow = min(minWindow, windowSum);
            }
        }

        // 答案 = 总和 - 最小窗口和
        return total - minWindow;
    }
};


// ======================== 解法二: 前缀和枚举（正向思维） ========================
// 思路:
//   直接枚举从左拿 i 张(0<=i<=k)，从右拿 k-i 张
//   用滚动变量维护 leftSum 和 rightSum
//
// 枚举过程 (cardPoints = [1,2,3,4,5,6,1], k=3):
//
//   i=0: 左0张+右3张  0 + [5,6,1]=12  ans=12
//   i=1: 左1张+右2张  [1]+[6,1]=8     ans=12
//   i=2: 左2张+右1张  [1,2]+[1]=4     ans=12
//   i=3: 左3张+右0张  [1,2,3]+0=6     ans=12

class Solution_PrefixSum {
public:
    int maxScore(vector<int>& cardPoints, int k) {
        int n = cardPoints.size();

        // 初始状态: 全部从右边拿 k 张
        int rightSum = 0;
        for (int i = n - k; i < n; i++) {
            rightSum += cardPoints[i];
        }

        int ans = rightSum;
        int leftSum = 0;

        // 逐步: 左边多拿一张，右边少拿一张
        for (int i = 0; i < k; i++) {
            leftSum += cardPoints[i];              // 左边加入第 i 张
            rightSum -= cardPoints[n - k + i];     // 右边移除最左的一张
            ans = max(ans, leftSum + rightSum);
        }

        return ans;
    }
};


/*
 * ======================== 详细执行示例 ========================
 *
 * 示例 1: cardPoints = [1,2,3,4,5,6,1], k = 3
 *
 * 解法一 (滑动窗口):
 *   total = 22, windowSize = 4
 *   i=0: windowSum=1
 *   i=1: windowSum=3
 *   i=2: windowSum=6
 *   i=3: windowSum=10, minWindow=10  (窗口 [1,2,3,4])
 *   i=4: windowSum=10+5-1=14, minWindow=10
 *   i=5: windowSum=14+6-2=18, minWindow=10
 *   i=6: windowSum=18+1-3=16, minWindow=10
 *   答案 = 22 - 10 = 12 ✓
 *
 * 解法二 (前缀和):
 *   初始 rightSum = 5+6+1 = 12, ans = 12
 *   i=0: leftSum=1, rightSum=12-5=7, total=8, ans=12
 *   i=1: leftSum=3, rightSum=7-6=1, total=4, ans=12
 *   i=2: leftSum=6, rightSum=1-1=0, total=6, ans=12
 *   答案 = 12 ✓
 *
 *
 * 示例 2: cardPoints = [2,2,2], k = 2
 *   total = 6, windowSize = 1
 *   窗口滑过: min(2, 2, 2) = 2
 *   答案 = 6 - 2 = 4 ✓
 *
 *
 * 示例 3: cardPoints = [9,7,7,9,7,7,9], k = 7
 *   windowSize = 0 -> 特判返回 total = 55 ✓
 *
 *
 * ======================== 边界情况 ========================
 *
 * 1. k == n: windowSize = 0, 直接返回 total
 * 2. k == 1: windowSize = n-1, 选最大的端点值
 * 3. 所有值相同: 任何方案得分一样
 * 4. 单元素: n = 1, k = 1, 直接返回该值
 *
 *
 * ======================== 等价转换的威力 ========================
 *
 * 原问题: "从两端拿 k 张，最大化得分"
 *   - 两端操作，看似需要 DP 或贪心
 *   - 贪心不成立: [1, 1000, 1] k=1, 贪心选1，但无论选哪端都是1
 *
 * 转换后: "找长度为 n-k 的最小和连续子数组"
 *   - 标准滑动窗口问题
 *   - 简洁、高效、不易出错
 *
 * 这种"反向思维"是面试中的强力武器:
 *   - LeetCode 1658 也用了同样技巧
 *   - LeetCode 918 环形子数组也可以用
 *
 *
 * ======================== 复杂度分析 ========================
 *
 * 解法一 (滑动窗口):
 *   时间: O(n)  -- 一次遍历求 total, 一次遍历滑动窗口
 *   空间: O(1)  -- 只用 total, windowSum, minWindow 三个变量
 *
 * 解法二 (前缀和枚举):
 *   时间: O(n)  -- O(k) 初始化右侧和 + O(k) 枚举 = O(n)
 *   空间: O(1)  -- 只用 leftSum, rightSum, ans 三个变量
 *
 * 两种解法时空复杂度相同，解法二的循环次数更少 (k 次 vs n 次)
 * 但解法一思路更通用（适用于更多"反向滑窗"问题）
 *
 *
 * ======================== 总结 ========================
 *
 * 【解法对比】
 *   - 滑动窗口（反向思维）: 面试首选，展示"正难则反"的思维能力
 *     把"两端取 k 张"转化为"中间留 n-k 张连续最小和"
 *   - 前缀和枚举（正向思维）: 更直觉，循环次数 O(k) 更少
 *     直接枚举左取 i 张 + 右取 k-i 张
 *   - 两者时空复杂度均为 O(n) / O(1)，但反向思维更通用
 *
 * 【易错点】
 *   1. 贪心不成立: 不能每次取两端较大的那张
 *      反例: [100, 1, 1, 1, 100] k=2，贪心取 100+1=101，最优 100+100=200
 *   2. 滑动窗口忘记特判 k==n: windowSize=0 时无窗口可滑，直接返回 total
 *   3. 前缀和解法中 rightSum 的索引计算:
 *      初始右侧起点 = n-k，每轮右边少一张: rightSum -= cardPoints[n-k+i]
 *      下标 off-by-one 是最常见 bug
 *   4. 窗口填满的判断: i >= windowSize-1 才开始更新 minWindow
 *      遗漏此判断会用不完整的窗口去比较
 *
 * 【面试追问】
 *   Q1: 如果不是从两端取，而是可以从任意位置取 k 张（不要求连续），最大和？
 *       → 排序后取最大的 k 个即可，O(n log n)
 *       → 或用 nth_element 做到 O(n) 平均
 *
 *   Q2: 如果卡牌排成环形（首尾相连），从任意位置取连续 k 张，最大和？
 *       → 即 LeetCode 918 的变体: 环形子数组最大和
 *       → 同样用反向思维: max(连续k张) = total - min(连续n-k张)
 *       → 环形场景下把数组复制一份拼接，或者分两种情况讨论
 *
 *   Q3: 如果每张卡牌有"正面/反面"两个值，取出后可以选择翻面，怎么最大化？
 *       → 先用滑窗确定取哪些位置的卡牌
 *       → 取出后每张独立选 max(front, back)
 *       → 但"取哪些位置"的决策会变复杂，可能需要 DP
 *       → 开放问题，面试中分析到这个层次即可
 */
