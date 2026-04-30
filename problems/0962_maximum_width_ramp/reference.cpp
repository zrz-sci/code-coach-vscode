// LeetCode 962: Maximum Width Ramp
// ./solution.cpp

// ============================================================
// Approach 1: Monotonic Stack (Optimal)
// ============================================================
// Phase 1: Build a strictly decreasing stack of indices (left to right).
//          These are the only useful left endpoints.
// Phase 2: Scan right to left, matching and popping stack elements.
//
// Time:  O(n) - each index pushed/popped at most once
// Space: O(n)

class Solution {
public:
    int maxWidthRamp(vector<int>& nums) {
        int n = nums.size();
        stack<int> stk;

        // Phase 1: Build decreasing stack of candidate left endpoints
        // Only push index i if nums[i] is strictly less than stack top
        for (int i = 0; i < n; i++) {
            if (stk.empty() || nums[i] < nums[stk.top()]) {
                stk.push(i);
            }
        }

        // Phase 2: Scan from right to left, greedily match with stack
        int ans = 0;
        for (int j = n - 1; j >= 0; j--) {
            // While stack top can form a valid ramp with j
            while (!stk.empty() && nums[stk.top()] <= nums[j]) {
                ans = max(ans, j - stk.top());
                stk.pop(); // consumed: no larger j will come
            }
        }

        return ans;
    }
};


// ============================================================
// Approach 2: Sort by Value with Index Tracking
// ============================================================
// Create (value, index) pairs, sort by value. Sweep through,
// tracking the minimum index seen so far. The ramp width at
// each step is current_index - min_index.
//
// Time:  O(n log n)
// Space: O(n)

class Solution2 {
public:
    int maxWidthRamp(vector<int>& nums) {
        int n = nums.size();
        vector<pair<int, int>> valIdx(n);
        for (int i = 0; i < n; i++) {
            valIdx[i] = {nums[i], i};
        }

        // Sort by value ascending; ties broken by index ascending
        sort(valIdx.begin(), valIdx.end());

        int ans = 0;
        int minIdx = n; // track minimum original index seen

        for (auto& [val, idx] : valIdx) {
            // All previously seen entries have value <= val (due to sort)
            // so minIdx is a valid left endpoint
            ans = max(ans, idx - minIdx);
            minIdx = min(minIdx, idx);
        }

        return ans;
    }
};


// ============================================================
// Approach 3: Suffix Maximum + Two Pointers
// ============================================================
// Build suffMax[j] = max(nums[j..n-1]).
// Use two pointers: i starts at 0, j starts at 0.
// Move j forward while suffMax[j] >= nums[i], then advance i.
//
// Time:  O(n)
// Space: O(n) for suffix array

class Solution3 {
public:
    int maxWidthRamp(vector<int>& nums) {
        int n = nums.size();

        // Build suffix maximum array
        vector<int> suffMax(n);
        suffMax[n - 1] = nums[n - 1];
        for (int j = n - 2; j >= 0; j--) {
            suffMax[j] = max(nums[j], suffMax[j + 1]);
        }

        // Two pointers
        int ans = 0;
        int i = 0, j = 0;

        while (j < n) {
            // If suffMax[j] < nums[i], no valid j exists for this i
            // Move i forward to find a smaller left endpoint
            if (suffMax[j] < nums[i]) {
                i++;
            } else {
                // suffMax[j] >= nums[i] means there exists some k >= j
                // with nums[k] >= nums[i], so ramp width >= j - i
                ans = max(ans, j - i);
                j++;
            }
        }

        return ans;
    }
};


// ============================================================
// Approach 4: Binary Search on Suffix Minimums of Indices
// ============================================================
// Maintain a list of (value, index) where values are in decreasing
// order (suffix minimum of values scanning left to right).
// For each j from right, binary search for the smallest value
// in the list that is <= nums[j], giving the leftmost valid i.
//
// Time:  O(n log n)
// Space: O(n)

class Solution4 {
public:
    int maxWidthRamp(vector<int>& nums) {
        int n = nums.size();

        // Build a list of "prefix minimums" with their indices
        // These are indices where nums[i] is strictly decreasing
        // (same as the monotonic stack from Approach 1)
        vector<int> candidates;
        for (int i = 0; i < n; i++) {
            if (candidates.empty() || nums[i] < nums[candidates.back()]) {
                candidates.push_back(i);
            }
        }

        // For each j from right to left, binary search in candidates
        // for the leftmost index i where nums[i] <= nums[j]
        int ans = 0;

        for (int j = n - 1; j >= 0; j--) {
            // Binary search: find the leftmost candidate with value <= nums[j]
            int lo = 0, hi = (int)candidates.size() - 1;
            int bestI = -1;

            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                if (nums[candidates[mid]] <= nums[j]) {
                    bestI = candidates[mid];
                    hi = mid - 1; // try to find an even earlier (more left) candidate
                } else {
                    lo = mid + 1;
                }
            }

            if (bestI != -1) {
                ans = max(ans, j - bestI);
            }
        }

        return ans;
    }
};
