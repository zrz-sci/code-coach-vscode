/*
 * 487. Max Consecutive Ones II
 * 难度: Medium | 标签: Array, Dynamic Programming, Sliding Window
 *
 * 【题目本质】
 * 二进制数组中翻转至多 1 个 0，求最长连续 1 的长度。
 * 等价于：找最长子数组使其中至多含 1 个 0。经典滑动窗口模型。
 *
 * 【解法总览】
 * Solution1: 滑动窗口          O(n)  O(1)  ⭐ 推荐
 * Solution2: DP (dp0/dp1)      O(n)  O(1)  思路直接
 * Solution3: 记录上一个0位置    O(n)  O(1)  流式输入友好
 */

// ===================== Solution1: 滑动窗口 =====================
// 维护窗口 [left, right] 使得窗口内 0 的个数 <= 1
class Solution1 {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int n = nums.size();
        int left = 0, zeroCount = 0, ans = 0;

        for (int right = 0; right < n; right++) {
            // 扩展右边界
            if (nums[right] == 0) zeroCount++;

            // 0 超过 1 个时收缩左边界
            while (zeroCount > 1) {
                if (nums[left] == 0) zeroCount--;
                left++;
            }

            // 当前窗口长度
            ans = max(ans, right - left + 1);
        }

        return ans;
    }
};

// ===================== Solution2: DP 双状态 =====================
// dp0: 以当前位结尾, 未翻转 0 的最长连续 1
// dp1: 以当前位结尾, 已翻转 1 个 0 的最长连续 1
class Solution2 {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int dp0 = 0; // 未翻转
        int dp1 = 0; // 已翻转一个 0
        int ans = 0;

        for (int num : nums) {
            if (num == 1) {
                // 两种状态都延长
                dp0++;
                dp1++;
            } else {
                // num == 0
                // 翻转当前 0: 在未翻转的连续1基础上 +1
                dp1 = dp0 + 1;
                // 不翻转: 连续 1 断了
                dp0 = 0;
            }
            ans = max(ans, dp1);
        }

        return ans;
    }
};

// ===================== Solution3: 记录上一个0位置 =====================
// 适合流式输入 (follow-up): 不需要存储整个数组
// 思路: 翻转位置 prevZero 把两段连续1拼接起来
class Solution3 {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int ans = 0;
        int prevZeroIdx = -1; // 上一个 0 的下标, -1 表示还没遇到
        int left = 0;         // 当前有效段的左边界

        for (int i = 0; i < (int)nums.size(); i++) {
            if (nums[i] == 0) {
                // 新的左边界 = 上一个 0 的下一个位置
                left = prevZeroIdx + 1;
                prevZeroIdx = i;
            }
            // 窗口 [left, i] 内至多 1 个 0 (就是 prevZeroIdx)
            ans = max(ans, i - left + 1);
        }

        return ans;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | Solution1 滑动窗口 | O(n) | O(1) | 通用模板, 易推广到 k 个 0 |
 * | Solution2 DP | O(n) | O(1) | 状态清晰, 无需双指针 |
 * | Solution3 记录0位置 | O(n) | O(1) | 流式输入最优, follow-up 答案 |
 *
 * 【易错点】
 * 1. 滑动窗口条件: zeroCount > 1 才收缩, 不是 > 0 (允许1个0)
 * 2. DP 遇到 0 时: dp1 = dp0 + 1, 不是 dp1 = 0
 *    - dp1 表示"翻了当前这个 0", 长度 = 前面连续1(dp0) + 翻的这个1
 * 3. prevZeroIdx 初始化 -1: 表示还没遇到过 0
 * 4. 全 1 数组: 三种解法都能正确处理, ans = n
 * 5. 推广到 k 个 0: Solution1 只需改条件为 zeroCount > k
 *
 * 【面试追问】
 * Q1: Follow-up 流式输入怎么做?
 *     用 Solution2 或 Solution3, 都只需 O(1) 空间。
 *     Solution3 更直观: 只记录上一个 0 的位置。
 * Q2: 推广到翻转 k 个 0 (LC 1004)?
 *     Solution1: while(zeroCount > k) 即可。
 *     Solution2: 需要 k+1 个 dp 状态, 或用队列记录 k 个 0 的位置。
 *     Solution3: 用队列存最近 k 个 0 的位置。
 * Q3: 如果数组很大(10^8), 哪个解法最快?
 *     三者都是 O(n) 一次遍历。Solution2 最快(无内循环), 但差异极小。
 *     关键是 O(1) 空间避免缓存失效。
 */
