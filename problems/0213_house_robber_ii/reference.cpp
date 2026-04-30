/*
 * LeetCode 213: 打家劫舍 II (House Robber II)
 *
 * 【题目本质】
 * 环形排列的房屋中，选择不相邻的子集使总金额最大。
 * 环形约束 = 首尾不能同时选，拆成两条链各跑一次线性 House Robber。
 *
 * 【系列关系】
 * 198 House Robber I: 线性排列 → 标准线性DP
 * 213 House Robber II: 环形排列 → 拆环为两条链，调用两次198
 *     代码差异：仅在外层加了 max(rob(0,n-2), rob(1,n-1))
 * 337 House Robber III: 树形排列 → 树形DP（后序遍历）
 *
 * 【解法总览】
 * 解法1: 暴力递归        — O(2^n) / O(n)   — 展示问题本质
 * 解法2: DP 数组         — O(n)   / O(n)   — 标准DP
 * 解法3: DP 滚动变量     — O(n)   / O(1)   — 面试首选
 */

// ============================================================
// 解法1: 暴力递归 — 枚举每个房子偷/不偷
// 时间: O(2^n)  空间: O(n) 递归栈
//
// 【思路】
// 最直觉的做法：对每个房子做"偷"或"不偷"的决策。
// 环形约束处理：分两种情况——可能选第一个(排除最后一个)，
// 或者排除第一个(可能选最后一个)。
// 有大量重叠子问题，效率极低，但能帮助理解问题结构。
//
// 决策树 (以 nums=[1,2,3,1], 情况A范围[0,2] 为例):
//
//               rob(0)
//              /       \
//         偷0(+1)    不偷0
//           |           |
//         rob(2)      rob(1)
//        /    \       /    \
//    偷2(+3) 不偷2 偷1(+2) 不偷1
//      |       |     |       |
//   return3  return0 rob(3) rob(2)
//                    超出    ...
//
//   偷0路径最优: 1+3=4
// ============================================================
class Solution1 {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return nums[0];
        return max(helper(nums, 0, n - 2), helper(nums, 1, n - 1));
    }

private:
    int helper(vector<int>& nums, int start, int end) {
        if (start > end) return 0;
        // 偷 start: 拿 nums[start]，跳过 start+1，从 start+2 继续
        // 不偷 start: 从 start+1 继续
        return max(nums[start] + helper(nums, start + 2, end),
                   helper(nums, start + 1, end));
    }
};

// ============================================================
// 解法2: DP 数组 — 拆环为链 + 线性 DP
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 暴力递归的瓶颈：大量重叠子问题，如 helper(3,5) 被重复计算。
// 改用自底向上的递推：dp[i] 表示考虑 nums[start..start+i] 能偷的最大值。
// 状态转移: dp[i] = max(dp[i-1], dp[i-2] + nums[start+i])
//   - dp[i-1]: 不偷第 i 个，最优解和前 i-1 个一样
//   - dp[i-2] + nums[start+i]: 偷第 i 个，加上跳过 i-1 后的最优
//
// dp 填表方向 (nums=[1,2,3,1], 情况A: [1,2,3]):
//   dp[0] = 1                    (只有一个房子)
//   dp[1] = max(1, 2) = 2        (两个房子选大的)
//   dp[2] = max(dp[1], dp[0]+3)  = max(2, 1+3) = 4
//   → 情况A结果: 4
//
// 情况B: [2,3,1]:
//   dp[0] = 2
//   dp[1] = max(2, 3) = 3
//   dp[2] = max(dp[1], dp[0]+1) = max(3, 2+1) = 3
//   → 情况B结果: 3
//
// 答案 = max(4, 3) = 4
// ============================================================
class Solution2 {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return nums[0];
        // 和198题唯一的区别: 调用两次线性DP, 取max
        return max(robLinear(nums, 0, n - 2), robLinear(nums, 1, n - 1));
    }

private:
    // 这就是198题 House Robber I 的标准解法
    int robLinear(vector<int>& nums, int start, int end) {
        int len = end - start + 1;
        if (len == 1) return nums[start];

        vector<int> dp(len);
        dp[0] = nums[start];
        dp[1] = max(nums[start], nums[start + 1]);

        for (int i = 2; i < len; i++) {
            // 核心转移: 偷第i个 vs 不偷第i个
            dp[i] = max(dp[i - 1], dp[i - 2] + nums[start + i]);
        }
        return dp[len - 1];
    }
};

// ============================================================
// 解法3: DP + 滚动变量空间优化 — O(n) / O(1) ⭐ 面试首选
//
// 【思路】
// 解法2中 dp[i] 只依赖 dp[i-1] 和 dp[i-2]，不需要存整个数组。
// 用两个变量 prev2(dp[i-2]) 和 prev1(dp[i-1]) 滚动即可。
// 这是线性DP的标准空间优化手法。
//
// 滚动过程 (nums=[1,2,3,1], 情况A: 索引0~2):
//
//   初始: prev2=0, prev1=0
//   i=0: curr = max(0, 0+1) = 1   → prev2=0, prev1=1
//   i=1: curr = max(1, 0+2) = 2   → prev2=1, prev1=2
//   i=2: curr = max(2, 1+3) = 4   → prev2=2, prev1=4
//   返回 prev1=4
//
//   情况B: 索引1~3:
//   初始: prev2=0, prev1=0
//   i=1: curr = max(0, 0+2) = 2   → prev2=0, prev1=2
//   i=2: curr = max(2, 0+3) = 3   → prev2=2, prev1=3
//   i=3: curr = max(3, 2+1) = 3   → prev2=3, prev1=3
//   返回 prev1=3
//
//   答案 = max(4, 3) = 4 ✓
// ============================================================
class Solution3 {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        // 特判: 只有一个房子, 必须单独处理
        // 否则 robLinear(0, -1) 区间为空会返回0, 丢失唯一的房子
        if (n == 1) return nums[0];

        // 环形约束: 第一个和最后一个不能同时偷
        // 拆成两条链: [0, n-2] 排除最后一个, [1, n-1] 排除第一个
        return max(robLinear(nums, 0, n - 2), robLinear(nums, 1, n - 1));
    }

private:
    // 198题的空间优化版: 在 nums[start..end] 上做线性打家劫舍
    int robLinear(vector<int>& nums, int start, int end) {
        int prev2 = 0;  // dp[i-2], 初始化为0而非nums值, 让循环统一处理
        int prev1 = 0;  // dp[i-1]

        for (int i = start; i <= end; i++) {
            // 核心决策: 偷第i个(prev2 + nums[i]) vs 不偷(prev1)
            int curr = max(prev1, prev2 + nums[i]);
            // 滚动: 为下一轮迭代准备
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间   | 空间 | 特点                     |
// |-------------|--------|------|--------------------------|
// | 暴力递归    | O(2^n) | O(n) | 展示决策结构, 不可用     |
// | DP 数组     | O(n)   | O(n) | 标准DP, 便于理解填表过程 |
// | DP 滚动变量 | O(n)   | O(1) | 面试首选, 最终答案       |
//
// 递进关系: 暴力递归 → 发现重叠子问题 → DP数组递推 → 
//          发现只依赖两个前驱 → 滚动变量
//
// 【与系列题的代码差异】
// 198 → 213 的变化:
//   198: return robLinear(nums, 0, n-1);  // 整个数组
//   213: return max(robLinear(nums, 0, n-2), robLinear(nums, 1, n-1));
//        ↑ 仅此一行不同: 拆环为两条链
//
// 【易错点】
// 1. 忘记特判 n=1:
//    ✗ 直接 max(robLinear(0,n-2), robLinear(1,n-1)), n=1时 robLinear(0,-1) 返回0
//    ✓ if (n == 1) return nums[0]; 放在最前面
//
// 2. 拆环范围搞错:
//    ✗ robLinear(0, n-1) 和 robLinear(1, n) — 第一个没排除末尾, 第二个越界
//    ✓ robLinear(0, n-2) 和 robLinear(1, n-1) — 分别排除末尾和首部
//
// 3. 滚动变量初始化:
//    ✗ prev2 = nums[start], prev1 = nums[start+1] — start==end时越界
//    ✓ prev2 = 0, prev1 = 0 — 让for循环自然从第一个元素开始累积
//
// 4. 误认为两种情况有遗漏:
//    "如果首尾都不偷呢?" — 这种方案同时被两种情况覆盖, 取max不影响正确性
//
// 【面试追问】
// Q1: 为什么拆成两条链是完备的? 会不会遗漏某些方案?
//     → 任何合法方案中首尾不能同时选, 所以要么不含末尾(被[0,n-2]覆盖),
//       要么不含首部(被[1,n-1]覆盖). 两者的并集 = 所有合法方案.
//
// Q2: 如果 n 很大(10^6), 这个解法还能用吗? 瓶颈在哪?
//     → O(n) 时间 O(1) 空间, 完全可以. 线性扫描两次, 没有瓶颈.
//       如果要求只扫描一次, 可以用更复杂的环形DP状态定义, 但没有必要.
//
// Q3: 如果从环变成树(每个节点有多个孩子), 怎么做?
//     → 337. House Robber III, 用树形DP: 每个节点返回 (偷自己, 不偷自己)
//       的最大值对, 后序遍历自底向上合并. 决策框架相同, 数据结构从数组变成树.
// ============================================================
