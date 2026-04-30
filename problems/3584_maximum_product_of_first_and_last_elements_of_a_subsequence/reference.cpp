/*
 * LeetCode 3584: Maximum Product of First and Last Elements of a Subsequence
 * (子序列首尾元素的最大乘积)
 *
 * 难度: Medium | 标签: Array, Two Pointers, Sorting
 *
 * 【解法一】后缀极值 + 枚举首元素 -- O(n) / O(n) 【推荐】
 *
 * 核心观察:
 *   选长度为 m 的子序列, 只关心首元素和尾元素。
 *   中间 m-2 个元素可以是区间内任意元素, 对答案无影响。
 *
 *   设首元素在位置 i, 尾元素在位置 j:
 *   - i 的范围: [0, n-m] (后面至少留 m-1 个位置)
 *   - j 的约束: j >= i + m - 1 (中间要放 m-2 个元素)
 *
 *   对于固定的首元素 nums[i]:
 *   - 若 nums[i] > 0: 想让乘积最大, 尾元素越大越好 -> suffMax
 *   - 若 nums[i] < 0: 负负得正, 尾元素越小越好 -> suffMin
 *   - 若 nums[i] = 0: 乘积为 0
 *   同时检查 suffMax 和 suffMin 即可覆盖所有情况。
 *
 *   m == 1 特殊处理: 首尾是同一元素, 乘积 = nums[i]^2
 *
 * 示例: nums = [2,-1,2,-6,5,2,-5,7], m = 2
 *   suffMax = [7,7,7,7,7,7,7,7]
 *   suffMin = [-6,-6,-6,-6,-5,-5,-5,7]
 *   枚举首元素:
 *     i=0: 2*suffMax[1]=14, 2*suffMin[1]=-12 => 14
 *     i=1: -1*7=-7, -1*(-6)=6 => 6
 *     i=2: 2*7=14, 2*(-6)=-12 => 14
 *     i=3: -6*7=-42, -6*(-5)=30 => 30
 *     i=4: 5*7=35, 5*(-5)=-25 => 35  <-- 最大!
 *     i=5: 2*7=14, 2*(-5)=-10 => 14
 *     i=6: -5*7=-35, -5*7=-35 => -35
 *   答案 = 35 (子序列 [5, 7])
 */


class Solution {
public:
    long long maximumProduct(vector<int>& nums, int m) {
        int n = nums.size();

        // ==================== 特殊情况: m == 1 ====================
        // 首尾是同一个元素, 乘积 = nums[i] * nums[i] = nums[i]^2
        // 答案 = 绝对值最大的元素的平方
        if (m == 1) {
            long long ans = LLONG_MIN;
            for (int x : nums) {
                ans = max(ans, (long long)x * x);
            }
            return ans;
        }

        // ==================== 预处理后缀极值 ====================
        // suffMax[j] = max(nums[j], nums[j+1], ..., nums[n-1])
        // suffMin[j] = min(nums[j], nums[j+1], ..., nums[n-1])
        //
        // 从右向左扫描一遍即可
        vector<int> suffMax(n), suffMin(n);
        suffMax[n - 1] = suffMin[n - 1] = nums[n - 1];
        for (int j = n - 2; j >= 0; j--) {
            suffMax[j] = max(nums[j], suffMax[j + 1]);
            suffMin[j] = min(nums[j], suffMin[j + 1]);
        }

        // ==================== 枚举首元素 ====================
        // 首元素位置 i 的范围: [0, n-m]
        // 对应的尾元素最早位置: j = i + m - 1
        //
        // 对于每个 i, 考虑两种候选:
        //   prod1 = nums[i] * suffMax[j]  (正数首元素偏好大尾)
        //   prod2 = nums[i] * suffMin[j]  (负数首元素偏好小尾)
        // 取 max 覆盖所有情况
        long long ans = LLONG_MIN;
        for (int i = 0; i <= n - m; i++) {
            int j = i + m - 1; // 尾元素最早可能的位置
            long long prod1 = (long long)nums[i] * suffMax[j];
            long long prod2 = (long long)nums[i] * suffMin[j];
            ans = max({ans, prod1, prod2});
        }

        return ans;
    }
};

/*
 * ==================== 解法二: 排序 + 极值组合 ====================
 *
 * 排序后, 最大乘积只可能来自四种组合:
 *   - nums[0] * nums[m-1]     (最小首 * 最小尾)
 *   - nums[0] * nums[n-1]     (最小首 * 最大尾)
 *   - nums[n-m] * nums[m-1]   (最大首 * 最小尾)
 *   - nums[n-m] * nums[n-1]   (最大首 * 最大尾)
 *
 * 这是因为排序后:
 *   - 首元素范围 [0, n-m], 极值在 nums[0] 和 nums[n-m]
 *   - 尾元素范围 [m-1, n-1], 极值在 nums[m-1] 和 nums[n-1]
 *
 * 时间: O(n log n), 空间: O(1) (原地排序)

class Solution {
public:
    long long maximumProduct(vector<int>& nums, int m) {
        int n = nums.size();
        sort(nums.begin(), nums.end());

        if (m == 1) {
            long long ans = LLONG_MIN;
            for (int x : nums)
                ans = max(ans, (long long)x * x);
            return ans;
        }

        long long ans = LLONG_MIN;
        // 四种极值组合
        ans = max(ans, (long long)nums[0] * nums[m - 1]);
        ans = max(ans, (long long)nums[0] * nums[n - 1]);
        ans = max(ans, (long long)nums[n - m] * nums[m - 1]);
        ans = max(ans, (long long)nums[n - m] * nums[n - 1]);
        return ans;
    }
};
*/

/*
 * ==================== 复杂度分析 ====================
 *
 * 解法一 (后缀极值):
 *   时间: O(n) -- 后缀预处理 O(n) + 枚举 O(n)
 *   空间: O(n) -- 后缀 max/min 数组
 *
 * 解法二 (排序):
 *   时间: O(n log n) -- 排序主导
 *   空间: O(1)       -- 原地排序
 *
 * ==================== 关键易错点 ====================
 *
 * 1. m == 1 时首尾是同一个元素:
 *    乘积 = nums[i]^2, 不是 nums[i] * nums[j] (i != j)
 *    (-9)^2 = 81, 绝对值最大的元素平方就是答案
 *
 * 2. long long 溢出:
 *    nums[i] 可达 10^5, 乘积可达 10^10
 *    必须 (long long)nums[i] * nums[j], 否则 int * int 溢出
 *
 * 3. 正负分类讨论:
 *    首元素为正 -> 尾元素越大越好 (查 suffMax)
 *    首元素为负 -> 尾元素越小越好, 负负得正 (查 suffMin)
 *    两者都检查, 取 max 即可
 *
 * 4. 排序解法中首尾范围:
 *    首元素: sorted[0..n-m], 极值在 sorted[0] 和 sorted[n-m]
 *    尾元素: sorted[m-1..n-1], 极值在 sorted[m-1] 和 sorted[n-1]
 *    排序后无需显式约束 j >= i + m - 1, 天然满足
 *
 * 5. 中间元素的存在性:
 *    首元素 i, 尾元素 j, 需要 j - i >= m - 1
 *    且 [i+1, j-1] 中至少有 m-2 个元素
 *    后缀极值方法中 j = i + m - 1 自动满足
 *
 *
 * ======================== 总结 ========================
 *
 * 【解法对比】
 *   - 后缀极值 O(n)/O(n): 面试首选，线性时间，思路清晰
 *     预处理 suffMax/suffMin，枚举首元素时 O(1) 查尾元素极值
 *   - 排序极值组合 O(n log n)/O(1): 代码更短，只需检查 4 种组合
 *     排序后首元素极值在两端 [0] 和 [n-m]，尾元素极值在 [m-1] 和 [n-1]
 *   - 后缀极值更优（线性），但排序写法更不容易出错
 *
 * 【易错点】
 *   1. m==1 忘记特判: 首尾是同一个元素，乘积 = x^2 而非 x*y
 *      (-9)^2 = 81，绝对值最大的元素平方是答案
 *   2. long long 溢出: nums[i] 可达 10^5，乘积可达 10^10
 *      必须 (long long)nums[i] * nums[j]，int*int 会溢出
 *   3. 正负分类遗漏: 只查 suffMax 会漏掉负负得正的情况
 *      必须同时查 suffMax 和 suffMin，取两者乘积的 max
 *   4. 排序解法中首尾范围搞错:
 *      首元素范围 [0, n-m]（不是 [0, n-1]），尾元素范围 [m-1, n-1]
 *   5. 后缀极值的起始下标: 尾元素最早在 j = i+m-1，不是 i+m 也不是 i+1
 *
 * 【面试追问】
 *   Q1: 如果要求子序列首尾乘积最小（而非最大），怎么改？
 *       → 正数首元素想要小尾(suffMin)，负数首元素想要大尾(suffMax)
 *       → 仍然两个都检查，取 min 而非 max
 *       → 框架完全一样，只改 max → min
 *
 *   Q2: 如果不限制子序列长度（m 可以是 1 到 n 的任意值），最大乘积？
 *       → m=1: max(x^2) for all x
 *       → m>=2: 枚举所有 (i, j) 对，i<j，max(nums[i]*nums[j])
 *       → 等价于从数组中选两个元素使乘积最大，经典问题:
 *         排序后只需比较 nums[0]*nums[1] 和 nums[n-1]*nums[n-2]
 *       → 综合答案 = max(m=1 的答案, m>=2 的答案)
 *
 *   Q3: 如果改成三个元素的乘积（首、中、尾），怎么优化？
 *       → 固定首尾后，中间元素可以是区间 [i+1, j-1] 内的任意值
 *       → 需要区间极值查询 → 用 Sparse Table 预处理 O(n log n)，查询 O(1)
 *       → 正负讨论变成 8 种组合（三个位置各有正负），逐一检查取 max
 */
