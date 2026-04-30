// =====================================================
// 698. Partition to K Equal Sum Subsets
// 标签: Bit Manipulation, Memoization, Array, Dynamic Programming, Backtracking, Bitmask
// 难度: Medium
// =====================================================

// 解法1: 回溯 + 剪枝（元素视角）⭐ — O(k^n) 最坏 / O(n + k)
//
// 核心思路:
//   维护 k 个桶，每个桶的目标和 = sum/k。
//   对每个元素（降序排列后），尝试放入每个桶。
//   三重剪枝保证效率:
//     1. 桶溢出: buckets[i] + nums[idx] > target → skip
//     2. 空桶等价: buckets[i] == 0 且递归失败 → break
//     3. 降序排序: 大元素先放，约束更强，更早剪枝
//
// 决策树 (元素视角):
//   nums 降序排列后 = [5,4,3,3,2,2,1], target = 5
//
//   放5: 桶0=[5] (满)
//     放4: 桶1=[4]
//       放3: 桶2=[3]
//         放3: 桶3=[3]
//           放2: 桶2=[3,2]=5(满) → 放2: 桶3=[3,2]=5(满)
//             放1: 桶1=[4,1]=5(满) → 所有元素放完 → true!
class Solution1 {
public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % k != 0) return false;
        int target = sum / k;

        // 降序排序: 大元素先放, 更早触发剪枝
        sort(nums.rbegin(), nums.rend());
        if (nums[0] > target) return false;

        vector<int> buckets(k, 0);
        return backtrack(nums, buckets, 0, target);
    }

private:
    bool backtrack(vector<int>& nums, vector<int>& buckets,
                   int idx, int target) {
        // 所有元素都已分配到桶中
        if (idx == (int)nums.size()) return true;

        for (int i = 0; i < (int)buckets.size(); i++) {
            // 剪枝1: 桶溢出, 放不下当前元素
            if (buckets[i] + nums[idx] > target) continue;

            // 做选择: 把 nums[idx] 放入桶 i
            buckets[i] += nums[idx];
            if (backtrack(nums, buckets, idx + 1, target)) return true;
            buckets[i] -= nums[idx]; // 撤销选择

            // 剪枝2: 空桶等价剪枝
            // 当前桶是空的(撤销后恢复为0), 放入后仍失败
            // → 后续任何空桶结果也一样, 直接跳出
            if (buckets[i] == 0) break;
        }
        return false;
    }
};


// =====================================================
// 解法2: 状态压缩 DP — O(n * 2^n) / O(2^n)
//
// 核心思路:
//   n <= 16, 用 16 位 bitmask 表示哪些元素已被选择。
//   dp[mask] = 选了 mask 对应元素后, "当前桶" 中已累积的和。
//   当 dp[mask] + nums[i] == target 时, 桶满了, 取模归零,
//   自动开始填下一个桶。
//
//   状态转移:
//     对于 mask 中未选的第 i 个元素:
//     if dp[mask] + nums[i] <= target:
//         dp[mask | (1<<i)] = (dp[mask] + nums[i]) % target
//
//   答案: dp[(1<<n) - 1] == 0 (所有元素都选了, 最后一个桶也恰好满)
class Solution2 {
public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int n = nums.size();
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % k != 0) return false;
        int target = sum / k;

        sort(nums.begin(), nums.end());
        if (nums.back() > target) return false;

        int totalMasks = 1 << n;
        // dp[mask] = 当前桶中已累积的和, -1 表示不可达
        vector<int> dp(totalMasks, -1);
        dp[0] = 0; // 初始: 没选任何元素, 当前桶为空

        for (int mask = 0; mask < totalMasks; mask++) {
            if (dp[mask] == -1) continue; // 不可达状态, 跳过

            for (int i = 0; i < n; i++) {
                // 第 i 个元素已经被选过了
                if (mask & (1 << i)) continue;

                // 当前桶剩余容量能放下 nums[i]
                if (dp[mask] + nums[i] <= target) {
                    int newMask = mask | (1 << i);
                    // 取模: 桶满了(和==target)自动归零, 开始填下一个桶
                    dp[newMask] = (dp[mask] + nums[i]) % target;
                }
            }
        }
        // 所有元素都被选中, 且最后一个桶恰好装满
        return dp[totalMasks - 1] == 0;
    }
};


// =====================================================
// 解法3: 回溯（桶视角，对比用）— O(2^n * k) / O(n)
//
// 另一种回溯视角: 逐个填桶, 每个桶从剩余元素中选子集使和为 target。
// 当一个桶填满后递归填下一个桶, 直到所有桶填满。
// 用 visited 数组标记元素是否已被使用。
class Solution3 {
public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % k != 0) return false;
        int target = sum / k;

        sort(nums.rbegin(), nums.rend());
        if (nums[0] > target) return false;

        vector<bool> visited(nums.size(), false);
        return fillBucket(nums, visited, k, target, 0, 0);
    }

private:
    bool fillBucket(vector<int>& nums, vector<bool>& visited,
                    int bucketsLeft, int target,
                    int curSum, int startIdx) {
        // 所有桶都填满了
        if (bucketsLeft == 0) return true;

        // 当前桶刚好装满, 开始填下一个桶
        if (curSum == target) {
            return fillBucket(nums, visited, bucketsLeft - 1,
                            target, 0, 0);
        }

        for (int i = startIdx; i < (int)nums.size(); i++) {
            if (visited[i]) continue;
            if (curSum + nums[i] > target) continue;

            // 剪枝: 相同值的元素, 如果前一个没选, 后一个也不用尝试
            if (i > 0 && nums[i] == nums[i-1] && !visited[i-1]) continue;

            visited[i] = true;
            if (fillBucket(nums, visited, bucketsLeft, target,
                          curSum + nums[i], i + 1))
                return true;
            visited[i] = false;
        }
        return false;
    }
};


/*
 * =====================================================
 * 解法对比
 * =====================================================
 *
 * | 解法              | 时间           | 空间    | 特点              |
 * |-------------------|---------------|---------|------------------|
 * | Solution1 回溯    | O(k^n) 最坏    | O(n+k) | 剪枝后极快, 推荐   |
 * | Solution2 状压DP  | O(n * 2^n)    | O(2^n) | 稳定无最坏情况     |
 * | Solution3 桶视角  | O(2^n * k)    | O(n)   | 另一种思路, 对比用  |
 *
 * =====================================================
 * 易错点 (具体 bug)
 * =====================================================
 *
 * 1. 忘记降序排序:
 *    不排序 → 小元素先填 → 大元素最后放不下 → 回溯极深 → TLE
 *
 * 2. 空桶剪枝写成 continue:
 *    错误: if (buckets[i] == 0) continue;  // 继续尝试下一个空桶
 *    正确: if (buckets[i] == 0) break;     // 空桶等价, 跳出
 *
 * 3. 状压 DP 中忘记取模:
 *    dp[newMask] = dp[mask] + nums[i]  // 错! 没有"桶满归零"的效果
 *    dp[newMask] = (dp[mask] + nums[i]) % target  // 对!
 *
 * 4. 回溯中 idx 和 i 搞混:
 *    idx 是当前要放置的元素编号, i 是桶编号, 递归传 idx+1 不是 i+1
 *
 * 5. 预检查遗漏:
 *    必须检查 sum%k != 0 和 max > target, 否则无效输入也会跑满搜索
 *
 * =====================================================
 * 面试追问
 * =====================================================
 *
 * Q1: 能否贪心?
 * A:  不能。贪心无法保证全局最优。
 *     反例: nums=[1,1,1,1,1,5,5,5,5,5] k=5 target=6
 *
 * Q2: n 达到 100 怎么办?
 * A:  NP-hard, 需要近似算法如 LPT 贪心启发式。
 *
 * Q3: 和 416. Partition Equal Subset Sum 的关系?
 * A:  416 是 k=2 特殊情况, 可用 0-1 背包 O(n*sum)。
 *     k>2 时背包不适用, 必须回溯或状压。
 *
 * Q4: 两种回溯视角哪个更好?
 * A:  元素视角(Solution1): 空桶剪枝强, 代码简洁, 面试首选。
 *     桶视角(Solution3): k 远小于 n 时可能更优。
 */
