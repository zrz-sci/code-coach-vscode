/*
 * 【题目本质】
 * 求所有旋转方式下加权和的最大值。
 * 核心：相邻旋转的差值有固定递推关系，O(n)解决。
 *
 * 【解法总览】
 * Solution1: 递推公式  O(n) / O(1) ⭐推荐
 * Solution2: 暴力枚举  O(n^2) / O(1)（对比用）
 */

// ===================== Solution 1: 递推公式 =====================
// 推导：F(k) - F(k-1) = sum - n * nums[n-k]
// 旋转一位 -> 所有系数+1(贡献+sum) -> 末尾元素系数n-1归零(贡献-n*该元素)
class Solution1 {
public:
    int maxRotateFunction(vector<int>& nums) {
        int n = nums.size();
        long sum = 0, f = 0;
        for (int i = 0; i < n; i++) {
            sum += nums[i];
            f += (long)i * nums[i]; // F(0)
        }

        long maxVal = f;
        for (int k = 1; k < n; k++) {
            f = f + sum - (long)n * nums[n - k];
            maxVal = max(maxVal, f);
        }
        return (int)maxVal;
    }
};

// ===================== Solution 2: 暴力枚举 =====================
// 对每个旋转k，直接计算F(k)
// 时间O(n^2)，n=10^5会超时，仅作为验证
class Solution2 {
public:
    int maxRotateFunction(vector<int>& nums) {
        int n = nums.size();
        long maxVal = LONG_MIN;
        for (int k = 0; k < n; k++) {
            long val = 0;
            for (int i = 0; i < n; i++) {
                val += (long)i * nums[(i - k + n) % n];
            }
            maxVal = max(maxVal, val);
        }
        return (int)maxVal;
    }
};

/*
 * 【解法对比】
 * Solution1 递推：O(n)，利用数学关系，面试必考思路
 * Solution2 暴力：O(n^2)，n=10^5超时，仅用于小数据验证
 *
 * 【易错点】
 * 1. 中间计算必须用long（n*nums[i]最大10^5*100=10^7，
 *    累积后可达10^12，超出int范围）
 * 2. 递推中 nums[n-k] 的下标：k从1到n-1，下标从n-1到1
 * 3. 返回值强转int前确保范围（题目保证32-bit）
 *
 * 【面试追问】
 * Q: 如果数组长度为0怎么办？
 * A: 题目保证n>=1。若n==1，F(0)=0*nums[0]=0，直接返回0。
 *    递推循环不执行，maxVal=f=0，正确。
 */
