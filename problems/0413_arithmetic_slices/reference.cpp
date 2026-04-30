// 【题目本质】
// 统计连续子数组中等差数列的个数，核心是递推：延续等差段时新增子数组数+1

// 【解法总览】
// Solution1: DP 空间优化 — O(n) / O(1) — 面试首选
// Solution2: 数学计数（段长公式） — O(n) / O(1) — 一次性计算

// ===================== Solution 1: DP 空间优化 =====================
// dp 表示"以当前位置结尾的等差子数组个数"
// 满足等差条件时 dp++，否则重置为 0
class Solution1 {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size(), dp = 0, total = 0;
        for (int i = 2; i < n; i++) {
            if (nums[i] - nums[i-1] == nums[i-1] - nums[i-2]) {
                dp++;          // 延续等差段，新增 dp 个子数组
                total += dp;   // 累加到总数
            } else {
                dp = 0;        // 等差段中断，重置
            }
        }
        return total;
    }
};

// ===================== Solution 2: 数学计数（段长公式） =====================
// 连续等差段长度为 k 时，包含 (k-1)(k-2)/2 个等差子数组
// 例: 段长4 → (3)(2)/2 = 3 个
class Solution2 {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size(), total = 0, len = 2;
        for (int i = 2; i < n; i++) {
            if (nums[i] - nums[i-1] == nums[i-1] - nums[i-2]) {
                len++;
            } else {
                if (len >= 3) {
                    total += (long)(len - 1) * (len - 2) / 2;
                }
                len = 2;
            }
        }
        // 别忘了处理最后一段
        if (len >= 3) {
            total += (long)(len - 1) * (len - 2) / 2;
        }
        return total;
    }
};

// 【解法对比】
// Solution1: 逐步累加，逻辑清晰，容易推导正确性
// Solution2: 按段统计，代码略多但数学直觉更强，注意处理尾段

// 【易错点】
// 1. dp 在等差段中断时必须重置为 0
// 2. 段长公式法：循环结束后要处理最后一段（容易遗漏）
// 3. 本题是 subarray（连续），不是 subsequence

// 【面试追问】
// Q1: 子序列版本? → LC 446，dp[i][diff] 哈希表，O(n^2)
// Q2: dp+1 的直觉? → 延续等差段时，之前所有子数组各延伸一个 + 新长度3子数组
// Q3: 滑动窗口? → 等价于段长法，维护 [l,r] 区间
