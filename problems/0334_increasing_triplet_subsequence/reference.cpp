/*
 * 【题目本质】
 * 判断数组中是否存在 i<j<k 使得 nums[i]<nums[j]<nums[k]。
 * 本质是 LIS>=3 的判定，用贪心双变量 O(n)/O(1) 解决。
 *
 * 【解法总览】
 * Solution1: 贪心双变量 — O(n) 时间, O(1) 空间 ⭐推荐
 * Solution2: 前后缀数组 — O(n) 时间, O(n) 空间
 */

// ===== Solution1: 贪心双变量 =====
// 思路：维护 first(最小值) 和 second(次小值)，找到 > second 的即返回
// 关键：first 更新到后面位置不影响 second 的语义
class Solution1 {
public:
    bool increasingTriplet(vector<int>& nums) {
        int first = INT_MAX, second = INT_MAX;
        for (int num : nums) {
            if (num <= first) {
                first = num;         // 更新最小值
            } else if (num <= second) {
                second = num;        // 更新次小值（隐含：之前有个 < second 的值）
            } else {
                return true;         // num > second → 三元组存在
            }
        }
        return false;
    }
};

// ===== Solution2: 前后缀数组 =====
// 思路：leftMin[i] = min(nums[0..i]), rightMax[i] = max(nums[i..n-1])
// 若存在 i 使得 leftMin[i] < nums[i] < rightMax[i]，则三元组存在
class Solution2 {
public:
    bool increasingTriplet(vector<int>& nums) {
        int n = nums.size();
        if (n < 3) return false;

        // leftMin[i]: nums[0..i] 中的最小值
        vector<int> leftMin(n), rightMax(n);
        leftMin[0] = nums[0];
        for (int i = 1; i < n; i++)
            leftMin[i] = min(leftMin[i - 1], nums[i]);

        // rightMax[i]: nums[i..n-1] 中的最大值
        rightMax[n - 1] = nums[n - 1];
        for (int i = n - 2; i >= 0; i--)
            rightMax[i] = max(rightMax[i + 1], nums[i]);

        // 检查是否存在 nums[i] 同时大于左侧最小值、小于右侧最大值
        for (int i = 1; i < n - 1; i++) {
            if (leftMin[i] < nums[i] && nums[i] < rightMax[i])
                return true;
        }

        return false;
    }
};

/*
 * 【解法对比】
 * Solution1: O(n)/O(1) — 最优解，面试首选
 *   - 优点：空间最优，代码极简
 *   - 难点：理解 first 更新不影响 second 语义
 * Solution2: O(n)/O(n) — 直观易理解
 *   - 优点：思路清晰，容易解释
 *   - 缺点：额外 O(n) 空间
 *
 * 【易错点】
 * 1. 比较用 <= 不是 <：num <= first 时更新 first（等于时不推进层级）
 * 2. first 被更新到后面位置是安全的：second 隐含"之前存在更小的值"
 * 3. INT_MAX 作初始值：保证第一个元素一定进入 first
 * 4. 返回 false 的情况：数组单调递减或只有两种不同值
 *
 * 【面试追问】
 * Q1: first 更新后为什么 second 仍然有效？
 *     → second 表示"存在某个之前的值 < second"，不依赖当前 first
 *     → 例如 [1,5,0,6]: first=0 但 second=5 仍有效（1 < 5 < 6）
 * Q2: 推广到长度 k 的递增子序列？
 *     → 维护长度 k-1 的 tails 数组，二分更新，O(n log k) 时间
 * Q3: 如何输出具体的三元组？
 *     → 额外记录每层对应的下标，注意 first_idx 更新时保留旧值给 second 用
 */
