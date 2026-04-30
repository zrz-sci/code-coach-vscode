/*
 * LeetCode 862: Shortest Subarray with Sum at Least K
 *
 * Approach: Prefix Sum + Monotonic Deque
 *
 * Key Insight:
 *   Build prefix sums. For each right endpoint r, we want the largest l < r
 *   such that prefix[r] - prefix[l] >= k (i.e., prefix[l] <= prefix[r] - k).
 *
 *   A monotonic deque maintains candidate left endpoints with strictly
 *   increasing prefix sums. Two pruning rules:
 *   1. Front pop: if prefix[r] - prefix[front] >= k, record the answer
 *      and pop front (it won't give a shorter subarray for any future r).
 *   2. Back pop: if prefix[back] >= prefix[r], pop back (r dominates:
 *      same or smaller prefix sum but closer to future right endpoints).
 *
 *   Each index enters and exits the deque at most once -> O(n) total.
 *
 * Time:  O(n)
 * Space: O(n)
 */

// ===================== Solution 1: Prefix Sum + Monotonic Deque (Optimal) =====================

class Solution {
public:
    int shortestSubarray(vector<int>& nums, int k) {
        int n = nums.size();

        // Step 1: Build prefix sum array (use long long to prevent overflow)
        // prefix[i] = sum of nums[0..i-1], prefix[0] = 0
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        // Step 2: Monotonic deque - stores indices into prefix array
        // Invariant: prefix values at indices in deque are strictly increasing
        deque<int> dq;
        int ans = INT_MAX;

        for (int r = 0; r <= n; r++) {
            // Front pop: greedily find shortest subarray ending at r
            // If prefix[r] - prefix[dq.front()] >= k, we found a valid subarray
            // Pop it because no future r' > r can give a shorter [front, r')
            while (!dq.empty() && prefix[r] - prefix[dq.front()] >= k) {
                ans = min(ans, r - dq.front());
                dq.pop_front();
            }

            // Back pop: maintain monotonic increasing prefix values
            // If prefix[dq.back()] >= prefix[r], then dq.back() is dominated by r:
            //   - prefix[r] <= prefix[dq.back()] (easier to satisfy >= k)
            //   - r > dq.back() (closer to future right endpoints -> shorter subarray)
            while (!dq.empty() && prefix[dq.back()] >= prefix[r]) {
                dq.pop_back();
            }

            dq.push_back(r);
        }

        return ans == INT_MAX ? -1 : ans;
    }
};

// ===================== Solution 2: Prefix Sum + Min-Heap =====================
// Less optimal O(n log n) but easier to understand

class SolutionV2 {
public:
    int shortestSubarray(vector<int>& nums, int k) {
        int n = nums.size();
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        // Min-heap: (prefix_value, index)
        // For each r, pop all heap entries where prefix[r] - entry.prefix >= k
        priority_queue<pair<long long, int>,
                       vector<pair<long long, int>>,
                       greater<pair<long long, int>>> minHeap;

        int ans = INT_MAX;

        for (int r = 0; r <= n; r++) {
            // Check all candidate left endpoints with smallest prefix sums first
            while (!minHeap.empty() && prefix[r] - minHeap.top().first >= k) {
                ans = min(ans, r - minHeap.top().second);
                minHeap.pop();
            }

            minHeap.push({prefix[r], r});
        }

        return ans == INT_MAX ? -1 : ans;
    }
};

// ===================== Solution 3: Detailed Step-by-Step =====================
// Same as Solution 1 but with explicit comments at each step

class SolutionV3 {
public:
    int shortestSubarray(vector<int>& nums, int k) {
        int n = nums.size();

        // Build prefix sums
        // prefix[0] = 0 (empty subarray sum)
        // prefix[i] represents sum(nums[0..i-1])
        // subarray sum [l, r) = prefix[r] - prefix[l]
        vector<long long> pre(n + 1);
        pre[0] = 0;
        for (int i = 1; i <= n; i++) {
            pre[i] = pre[i - 1] + (long long)nums[i - 1];
        }

        // Deque stores indices in increasing order of pre[] values
        deque<int> window;
        int shortest = INT_MAX;

        for (int right = 0; right <= n; right++) {
            // --- Phase 1: Try to shrink from the left ---
            // Any valid left endpoint can be consumed (popped) because
            // future right endpoints would only make the subarray longer
            while (!window.empty()) {
                int left = window.front();
                if (pre[right] - pre[left] >= (long long)k) {
                    shortest = min(shortest, right - left);
                    window.pop_front();
                } else {
                    break;  // Deque is monotonic, if front doesn't work, none will
                }
            }

            // --- Phase 2: Maintain monotonic invariant ---
            // Remove back elements with prefix >= current prefix
            // They can never be a better left endpoint than 'right'
            while (!window.empty() && pre[window.back()] >= pre[right]) {
                window.pop_back();
            }

            // --- Phase 3: Add current index as future candidate ---
            window.push_back(right);
        }

        return shortest == INT_MAX ? -1 : shortest;
    }
};

// ===================== Walkthrough with Examples =====================
/*
 * Example 1: nums = [1], k = 1
 *   prefix = [0, 1]
 *   r=0: dq=[], push 0 -> dq=[0]
 *   r=1: prefix[1]-prefix[0]=1>=1 -> ans=1, pop front -> dq=[]
 *         push 1 -> dq=[1]
 *   Return 1
 *
 * Example 2: nums = [1,2], k = 4
 *   prefix = [0, 1, 3]
 *   r=0: push 0 -> dq=[0]
 *   r=1: prefix[1]-prefix[0]=1<4, push 1 -> dq=[0,1]
 *   r=2: prefix[2]-prefix[0]=3<4, push 2 -> dq=[0,1,2]
 *   Return -1  (max sum is 3 < 4)
 *
 * Example 3: nums = [2,-1,2], k = 3
 *   prefix = [0, 2, 1, 3]
 *   r=0: push 0 -> dq=[0]
 *   r=1: prefix[1]-prefix[0]=2<3
 *         prefix[0]=0 < prefix[1]=2, push 1 -> dq=[0,1]
 *   r=2: prefix[2]-prefix[0]=1<3
 *         prefix[1]=2 >= prefix[2]=1, pop back -> dq=[0]
 *         prefix[0]=0 < prefix[2]=1, push 2 -> dq=[0,2]
 *   r=3: prefix[3]-prefix[0]=3>=3 -> ans=3, pop front -> dq=[2]
 *         prefix[3]-prefix[2]=3-1=2<3
 *         prefix[2]=1 < prefix[3]=3, push 3 -> dq=[2,3]
 *   Return 3  (entire array: 2 + (-1) + 2 = 3)
 *
 * More complex example: nums = [84,-37,32,40,95], k = 167
 *   prefix = [0, 84, 47, 79, 119, 214]
 *   r=0: push 0 -> dq=[0]
 *   r=1: 84-0=84<167, push -> dq=[0,1]
 *   r=2: 47-0=47<167, prefix[1]=84>=47 pop back -> dq=[0], push -> dq=[0,2]
 *   r=3: 79-0=79<167, push -> dq=[0,2,3]
 *   r=4: 119-0=119<167, push -> dq=[0,2,3,4]
 *   r=5: 214-0=214>=167 -> ans=5, pop 0 -> dq=[2,3,4]
 *         214-47=167>=167 -> ans=min(5,3)=3, pop 2 -> dq=[3,4]
 *         214-79=135<167
 *         push -> dq=[3,4,5]
 *   Return 3  (subarray [32,40,95] = 167)
 */

// ===================== Complexity Analysis =====================
/*
 * Solution 1 (Monotonic Deque):
 *   Time:  O(n) - each element enters/exits deque at most once
 *   Space: O(n) - prefix array + deque
 *
 * Solution 2 (Min-Heap):
 *   Time:  O(n log n) - heap operations
 *   Space: O(n) - prefix array + heap
 *
 * Key Differences from Problem 209:
 *   - Problem 209: all positive numbers -> sliding window works
 *   - Problem 862: negative numbers allowed -> need monotonic deque
 *   - The negative numbers mean prefix sums are NOT monotonically increasing
 *   - This prevents the simple two-pointer approach
 *
 * Edge Cases:
 *   - All negative: return -1
 *   - Single element >= k: return 1
 *   - k = 0: any non-empty subarray with sum >= 0 works (edge case per constraints)
 *   - Very large values: prefix sums can reach 10^10, need long long
 */
