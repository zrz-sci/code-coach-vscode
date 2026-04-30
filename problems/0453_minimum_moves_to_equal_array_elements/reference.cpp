/*
 * 【题目本质】
 * 每次给 n-1 个元素 +1 等价于给 1 个元素 -1。
 * 答案 = 所有元素减到最小值的总步数 = sum(nums) - n * min(nums)。
 *
 * 【解法总览】
 * Solution1: 数学等价变换 O(n)/O(1) ⭐ 最优
 * Solution2: 排序后累差 O(n log n)/O(1) 直观
 */

// ===================== Solution 1: 数学等价变换 =====================
// 给 n-1 个元素 +1 ≡ 给 1 个元素 -1
// 所有元素减到 min，答案 = Σ(nums[i] - min)
class Solution1 {
public:
    int minMoves(vector<int>& nums) {
        int mn = *min_element(nums.begin(), nums.end());
        int res = 0;
        for (int x : nums) {
            res += x - mn;
        }
        return res;
    }
};

// ===================== Solution 2: 排序后逐差 =====================
// 排序后每个元素与最小值的差即为贡献
// 排序帮助直观理解：高的元素需要更多步减下来
class Solution2 {
public:
    int minMoves(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int res = 0;
        for (int i = 1; i < (int)nums.size(); i++) {
            res += nums[i] - nums[0];
        }
        return res;
    }
};

/*
 * 【解法对比】
 * Solution1: O(n) 一次遍历找 min 并累加差值
 * Solution2: O(n log n) 排序后累加，逻辑更直观
 *
 * 【易错点】
 * 1. 不要模拟操作过程，直接数学推导
 * 2. 等价变换方向：+1 给 n-1 个 ≡ -1 给 1 个
 * 3. 累加差值而非 sum - n*min，避免大数溢出
 *
 * 【面试追问】
 * Q1: 如何证明等价性？
 *     A: 设操作 k 次后值为 v，则 sum + k*(n-1) = n*v
 *     等价于每次选一个不加，该元素相对于其他减少 1
 * Q2: 如果改成每次可以给 n-1 个 +任意值呢？
 *     A: 等价于每次选一个减任意值，答案 = max - min
 * Q3: nums[i] 可能为负数，会影响正确性吗？
 *     A: 不影响，差值运算对负数同样成立
 */
