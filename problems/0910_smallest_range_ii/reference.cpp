// LeetCode 910: Smallest Range II
// Approach: Sorting + Greedy Partition
// Time: O(n log n), Space: O(1)

// ============================================================
// Core Idea:
// After sorting, the optimal choice is to pick a partition index i
// such that nums[0..i] all get +k and nums[i+1..n-1] all get -k.
//
// Proof sketch: If sorted a < b and a gets -k while b gets +k,
// the spread increases unnecessarily. Swapping their operations
// (a gets +k, b gets -k) never makes the range worse. So the
// optimal assignment is always a prefix of +k and suffix of -k
// (or vice versa, but that is equivalent to the "all same" case).
// ============================================================

class Solution {
public:
    int smallestRangeII(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size();

        // Base case: all elements get the same operation (+k or -k)
        // The range stays the same as the original
        int result = nums[n - 1] - nums[0];

        // Try each partition point: nums[0..i] get +k, nums[i+1..n-1] get -k
        for (int i = 0; i < n - 1; i++) {
            // After modification:
            // Left group max = nums[i] + k
            // Right group max = nums[n-1] - k
            // Left group min = nums[0] + k
            // Right group min = nums[i+1] - k
            int high = max(nums[i] + k, nums[n - 1] - k);
            int low = min(nums[0] + k, nums[i + 1] - k);
            result = min(result, high - low);
        }

        return result;
    }
};

// ============================================================
// Detailed Walkthrough: nums = [1, 3, 6], k = 3
// ============================================================
// Sorted: [1, 3, 6]
// n = 3, initial result = 6 - 1 = 5
//
// Partition i=0: left=[1], right=[3, 6]
//   Modified: left=[1+3=4], right=[3-3=0, 6-3=3]
//   All values: {4, 0, 3}
//   high = max(1+3, 6-3) = max(4, 3) = 4
//   low  = min(1+3, 3-3) = min(4, 0) = 0
//   score = 4 - 0 = 4
//   result = min(5, 4) = 4
//
// Partition i=1: left=[1, 3], right=[6]
//   Modified: left=[1+3=4, 3+3=6], right=[6-3=3]
//   All values: {4, 6, 3}
//   high = max(3+3, 6-3) = max(6, 3) = 6
//   low  = min(1+3, 6-3) = min(4, 3) = 3
//   score = 6 - 3 = 3
//   result = min(4, 3) = 3
//
// Answer: 3
// ============================================================

// ============================================================
// Walkthrough: nums = [0, 10], k = 2
// ============================================================
// Sorted: [0, 10]
// n = 2, initial result = 10 - 0 = 10
//
// Partition i=0: left=[0], right=[10]
//   high = max(0+2, 10-2) = max(2, 8) = 8
//   low  = min(0+2, 10-2) = min(2, 8) = 2
//   score = 8 - 2 = 6
//   result = min(10, 6) = 6
//
// Answer: 6
// ============================================================

// ============================================================
// Edge Case Analysis
// ============================================================
//
// Case 1: Single element nums = [5], k = 3
//   result = 5 - 5 = 0. Loop does not execute. Answer: 0.
//   (The single element becomes either 8 or 2, range is 0.)
//
// Case 2: k = 0, nums = [1, 5, 10]
//   All operations are +0 or -0 = no change.
//   result = 10 - 1 = 9.
//   All partitions: high = max(nums[i], nums[n-1]) = nums[n-1] = 10,
//   low = min(nums[0], nums[i+1]) = nums[0] = 1.
//   score = 10 - 1 = 9. Answer: 9. Correct.
//
// Case 3: All same nums = [3, 3, 3], k = 5
//   result = 3 - 3 = 0.
//   Partition i=0: high = max(3+5, 3-5) = max(8, -2) = 8
//                  low  = min(3+5, 3-5) = min(8, -2) = -2
//                  score = 10. result = min(0, 10) = 0.
//   Answer: 0. (All get +5 or all get -5, range is 0.)
// ============================================================

// ============================================================
// Comparison with LC 908 (Smallest Range I):
// ============================================================
// LC 908: Each element can change by any amount in [-k, k].
//   Answer: max(0, (max - min) - 2*k)
//   Much simpler -- just shrink the range by 2k.
//
// LC 910: Each element MUST change by exactly +k or -k.
//   The partition approach is needed because the binary choice
//   creates a more complex optimization landscape.
//
// Key difference: In 908, you can "perfectly center" all elements.
// In 910, you can only shift elements up or down by a fixed amount,
// creating a two-level structure that requires the partition analysis.
// ============================================================

// ============================================================
// Mathematical Proof of Optimality:
// ============================================================
// Claim: In the optimal solution, there exists a threshold T such
// that all nums[i] <= T get +k and all nums[i] > T get -k.
//
// Proof by contradiction: Suppose sorted a < b, a gets -k, b gets +k.
// Then a-k < a+k and b+k > b-k.
// New range includes [a-k, b+k] which has spread (b+k) - (a-k) = b-a+2k.
// If we swap: a gets +k, b gets -k.
// New range includes [a+k, b-k] (if a+k <= b-k, spread = b-a-2k, better).
// If a+k > b-k (i.e., b-a < 2k), the swap still does not make the
// overall range worse because the global max/min are determined by
// the boundary elements of the partition, not internal elements.
// ============================================================
