/*
 * LeetCode 152: Maximum Product Subarray (乘积最大子数组)
 *
 * 【题目本质】
 * 在数组中找一段连续子数组，使得乘积最大。
 * 和 53 题（最大子数组和）类似，但乘法有"负负得正"和"乘零归零"两个特殊性质，
 * 导致不能只追踪最大值，还必须同时追踪最小值。
 *
 * 【解法总览】
 * 解法1: 暴力枚举         — O(n²) / O(1)  — 枚举所有子数组
 * 解法2: DP 数组版        — O(n)  / O(n)  — 同时维护 dpMax 和 dpMin
 * 解法3: DP 空间优化      — O(n)  / O(1)  — 滚动变量 ⭐面试首选
 * 解法4: 前后缀乘积       — O(n)  / O(1)  — 数学角度，不用DP
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举所有子数组的乘积
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 最直觉的做法：固定起点 i，向右累乘到 j，记录最大乘积。
// 瓶颈：双重循环，n=2*10^4 时约 4*10^8 次操作，可能超时。
// ============================================================
class Solution1 {
public:
    int maxProduct(vector<int>& nums) {
        int n = nums.size();
        int ans = nums[0];
        for (int i = 0; i < n; i++) {
            int product = 1;
            for (int j = i; j < n; j++) {
                product *= nums[j];  // 累乘，不需要从头算
                ans = max(ans, product);
            }
        }
        return ans;
    }
};

// ============================================================
// 解法2: DP 数组版 — 同时维护以 i 结尾的最大和最小乘积
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 类比 53 题（最大子数组和），定义:
//   dpMax[i] = 以 nums[i] 结尾的最大乘积
//   dpMin[i] = 以 nums[i] 结尾的最小乘积
//
// 为什么需要 dpMin？因为乘法有"负负得正":
//   dpMin[i-1] 可能是一个绝对值很大的负数，
//   乘上一个负的 nums[i] 后，反而变成最大的正数。
//
// 转移方程（三个候选取 max/min）:
//   dpMax[i] = max(nums[i], dpMax[i-1]*nums[i], dpMin[i-1]*nums[i])
//   dpMin[i] = min(nums[i], dpMax[i-1]*nums[i], dpMin[i-1]*nums[i])
//   nums[i] 自身 → 代表"不接前面，重新开始一段"
//
// DP 填表过程 (nums = [2, 3, -2, 4]):
//
// index:  0    1     2     3
// nums:   2    3    -2     4
// dpMax:  2    6    -2     4
// dpMin:  2    3   -12   -48
// ans:    2    6     6     6
//
// index=2: 候选 = {-2, 6*(-2)=-12, 3*(-2)=-6}
//          dpMax[2] = max(-2,-12,-6) = -2
//          dpMin[2] = min(-2,-12,-6) = -12
// ============================================================
class Solution2 {
public:
    int maxProduct(vector<int>& nums) {
        int n = nums.size();
        vector<int> dpMax(n), dpMin(n);
        dpMax[0] = dpMin[0] = nums[0];
        int ans = nums[0];

        for (int i = 1; i < n; i++) {
            // 三个候选：重新开始 / 接上最大 / 接上最小
            dpMax[i] = max({nums[i], dpMax[i - 1] * nums[i], dpMin[i - 1] * nums[i]});
            dpMin[i] = min({nums[i], dpMax[i - 1] * nums[i], dpMin[i - 1] * nums[i]});
            ans = max(ans, dpMax[i]);
        }
        return ans;
    }
};

// ============================================================
// 解法3: DP 空间优化 — 滚动变量 ⭐面试首选
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法2中 dpMax[i] 和 dpMin[i] 只依赖 i-1 的值，
// 所以不需要数组，两个变量滚动即可。
//
// 关键：更新 curMax 时会覆盖旧值，但计算 curMin 还需要旧的 curMax，
// 所以必须先保存旧值（或用 swap 技巧）。
//
// 写法A: 保存旧值
// 写法B: 负数时先 swap，更直观
// ============================================================

// 写法A: 保存旧值
class Solution3A {
public:
    int maxProduct(vector<int>& nums) {
        int curMax = nums[0], curMin = nums[0];
        int ans = nums[0];

        for (int i = 1; i < (int)nums.size(); i++) {
            // 保存旧值，防止 curMax 被覆盖后影响 curMin 的计算
            int prevMax = curMax, prevMin = curMin;
            curMax = max({nums[i], prevMax * nums[i], prevMin * nums[i]});
            curMin = min({nums[i], prevMax * nums[i], prevMin * nums[i]});
            ans = max(ans, curMax);
        }
        return ans;
    }
};

// 写法B: 负数交换（更直观）
class Solution3B {
public:
    int maxProduct(vector<int>& nums) {
        int curMax = nums[0], curMin = nums[0];
        int ans = nums[0];

        for (int i = 1; i < (int)nums.size(); i++) {
            // 核心洞察：nums[i] 为负时，乘上它后最大变最小、最小变最大
            // 提前交换，之后就可以正常处理（和加法的逻辑一样）
            if (nums[i] < 0) swap(curMax, curMin);

            // 交换后的逻辑和 53 题完全一样
            curMax = max(nums[i], curMax * nums[i]);
            curMin = min(nums[i], curMin * nums[i]);
            ans = max(ans, curMax);
        }
        return ans;
    }
};

// ============================================================
// 解法4: 前后缀乘积 — 数学角度
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 完全不同的角度，不用 DP，从数学性质出发。
//
// 核心观察：如果数组没有 0，最大乘积子数组一定是某个前缀或某个后缀。
// 证明（直觉）：
//   - 负数个数为偶数 → 整个数组乘积最大（所有负号抵消）
//   - 负数个数为奇数 → 要去掉一个负数（及其一侧），
//     最优策略是去掉最左边的负数（保留右半段=后缀）
//     或去掉最右边的负数（保留左半段=前缀）
//
// 有 0 怎么办？0 把数组分成若干段，每段独立处理。
// 实现技巧：遇到 0 时把累乘重置为 1。
//
// 例: nums = [2, 3, -2, 4]
// prefix 从左往右: 2, 6, -12, -48  → max = 6
// suffix 从右往左: 4, -8, -24, -48 → max = 4
// 总体 ans = 6
//
// 例: nums = [-2, 0, -1]
// prefix: -2, →0→reset, -1   → 比较 -2, 0, -1
// suffix: -1, →0→reset, -2   → 比较 -1, 0, -2
// ans = 0
// ============================================================
class Solution4 {
public:
    int maxProduct(vector<int>& nums) {
        int n = nums.size();
        int ans = nums[0];
        int prefix = 0, suffix = 0;

        for (int i = 0; i < n; i++) {
            // 遇到 0 后重置为 1（0 本身会通过乘法结果参与 ans 比较）
            prefix = (prefix == 0 ? 1 : prefix) * nums[i];
            suffix = (suffix == 0 ? 1 : suffix) * nums[n - 1 - i];
            ans = max(ans, max(prefix, suffix));
        }
        return ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间   | 空间  | 特点                                |
// |-------------|--------|-------|-------------------------------------|
// | 暴力枚举     | O(n²)  | O(1)  | 简单，但大数据量超时                  |
// | DP 数组版    | O(n)   | O(n)  | 逻辑清晰，便于理解                    |
// | DP 空间优化  | O(n)   | O(1)  | ⭐面试首选，代码简短                  |
// | 前后缀乘积   | O(n)   | O(1)  | 数学思维，不走 DP 路线               |
//
// 和 LeetCode 53（最大子数组和）的关键区别：
//   53 题: dp[i] = max(dp[i-1] + nums[i], nums[i]) — 只维护一个值
//   152题: 需要同时维护 max 和 min — 因为负负得正
//   代码改动: 增加 curMin 变量 + 三候选取 max/min
//
// ============================================================
// 【易错点】
//
// 1. 空间优化时没保存旧值:
//    ✗ curMax = max({nums[i], curMax*nums[i], curMin*nums[i]});
//      curMin = min({nums[i], curMax*nums[i], curMin*nums[i]});
//      // curMin 用的是更新后的 curMax！
//    ✓ 先 int prevMax = curMax; 再计算
//    或者用 swap 技巧彻底规避这个问题
//
// 2. ans 初始化为 0:
//    ✗ int ans = 0; // nums = [-3, -2] 时答案应该是 6，但初始 0 不会被更新
//    实际上 ans=0 在这个例子中会被更新，但 nums = [-3] 时答案应是 -3
//    ✓ int ans = nums[0]; 从第一个元素开始
//
// 3. 忘记 nums[i] 自身作为候选:
//    ✗ curMax = max(prevMax * nums[i], prevMin * nums[i]);
//    // 如果前面乘积是 0*5=0 和 0*5=0，但 nums[i]=5 本身就是最大
//    ✓ curMax = max({nums[i], prevMax * nums[i], prevMin * nums[i]});
//
// 4. 前后缀解法中 prefix/suffix 初始化为 1:
//    ✗ int prefix = 1; // 第一个元素为负时，ans 初始为 nums[0]，
//      但 prefix 从 1 开始乘，如果整个数组都是负数可能出问题
//    ✓ int prefix = 0; 用 0 作哨兵，遇到 0 时重置为 1
//
// ============================================================
// 【面试追问】
//
// Q1: 和 53 题（最大子数组和）的区别？为什么需要同时维护 max 和 min？
// → 加法中负的前缀和一定拖累后面，可以直接丢弃。
//   乘法中负的前缀积遇到负数会翻正，不能丢弃。
//   所以必须同时记住"最小乘积"以等待翻盘。
//
// Q2: 如果改成"乘积最大子序列（不要求连续）"？
// → 变成贪心问题：选所有正数，负数按绝对值从大到小两两配对选取，
//   零不选。特殊情况：全非正数时选最大的那个。
//
// Q3: 如果要返回子数组的起止下标？
// → 在 DP 中额外维护 start 变量：当选择"重新开始"(nums[i] 自身最大)时
//   start = i；当 curMax > ans 时记录 bestStart = start, bestEnd = i。
// ============================================================
