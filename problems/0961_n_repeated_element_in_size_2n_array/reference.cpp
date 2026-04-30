// LeetCode 961: N-Repeated Element in Size 2N Array
// ./solution.cpp

// ============================================================
// Approach 1: Hash Set - First Duplicate Found
// ============================================================
// Iterate through the array, insert each element into a set.
// The first element that already exists in the set is the answer.
//
// Time:  O(n)
// Space: O(n)

class Solution {
public:
    int repeatedNTimes(vector<int>& nums) {
        unordered_set<int> seen;
        for (int x : nums) {
            // If already seen, this is the n-repeated element
            if (seen.count(x)) {
                return x;
            }
            seen.insert(x);
        }
        return -1; // unreachable given problem constraints
    }
};


// ============================================================
// Approach 2: Spacing / Pigeonhole (O(1) Space)
// ============================================================
// Since n copies fill half the 2n array, the repeated element
// must appear within distance 1 or 2 of another copy.
// We check all adjacent pairs and all pairs at distance 2.
//
// Time:  O(n)
// Space: O(1)

class Solution2 {
public:
    int repeatedNTimes(vector<int>& nums) {
        int len = nums.size();

        // Check distance 1 (adjacent pairs)
        for (int i = 0; i + 1 < len; i++) {
            if (nums[i] == nums[i + 1]) {
                return nums[i];
            }
        }

        // Check distance 2
        for (int i = 0; i + 2 < len; i++) {
            if (nums[i] == nums[i + 2]) {
                return nums[i];
            }
        }

        // If we reach here, the repeated elements are at positions
        // 0 and len-1 (maximally spread with alternating pattern)
        // e.g., [5, 1, 2, 3, 4, 5] for n=3 wouldn't reach here,
        // but [5, 1, 5, 2, 5, 3] would be caught by distance 2.
        // The only remaining case is first and last.
        return nums[0];
    }
};


// ============================================================
// Approach 3: Sorting + Middle Check
// ============================================================
// After sorting, n copies of the repeated element cluster together.
// They must occupy either the left half center or right half center.
// Specifically, nums[n-1] or nums[n] must be the repeated element.
//
// Time:  O(n log n)
// Space: O(1) if in-place sort

class Solution3 {
public:
    int repeatedNTimes(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size() / 2;

        // After sorting, n identical elements sit together.
        // They either start at or before index n-1, or end at or after index n.
        // So the repeated element is at position n-1 or n.
        //
        // Example: [1, 2, 2, 3] -> n=2, nums[1]=2 (n-1 index)
        // Example: [1, 3, 3, 3] -> n=2, nums[1]=3 or nums[2]=3
        //
        // If nums[n-1] == nums[n], either works.
        // If not, the cluster is entirely in one half:
        //   - If cluster is in left half: nums[n-1] == nums[0] (or neighbors match)
        //   - If cluster is in right half: nums[n] == nums[2*n - 1]
        // But simply: check if nums[n] == nums[n-1], else check both.

        // Simpler: the repeated value must equal nums[n-1] or nums[n]
        // because n identical values must span at least one of these positions.
        if (nums[n - 1] == nums[n]) return nums[n];
        if (n >= 2 && nums[n - 1] == nums[n - 2]) return nums[n - 1];
        return nums[n];
    }
};


// ============================================================
// Approach 4: Randomized Approach
// ============================================================
// Pick two random indices. If they hold the same value, that value
// is the answer (since the repeated element is the only one with
// duplicates). The probability of picking two copies of the
// repeated element is (n/2n)^2 = 1/4 per trial, so expected
// ~4 trials to succeed.
//
// Time:  O(1) expected (O(n) worst case for verification)
// Space: O(1)

class Solution4 {
public:
    int repeatedNTimes(vector<int>& nums) {
        int len = nums.size();

        // Use a deterministic "random" by just checking specific patterns
        // In a real randomized approach, we'd use rand()
        // Here we simulate by cycling through pairs
        for (int gap = 1; gap <= 3; gap++) {
            for (int i = 0; i + gap < len; i++) {
                if (nums[i] == nums[i + gap]) {
                    return nums[i];
                }
            }
        }
        return nums[0]; // fallback, should not reach
    }
};


// ============================================================
// Approach 5: Counting with Array (when value range is small)
// ============================================================
// Since 0 <= nums[i] <= 10^4, we can use a counting array.
// The first element whose count reaches 2 is the answer.
//
// Time:  O(n)
// Space: O(max_val) = O(10^4)

class Solution5 {
public:
    int repeatedNTimes(vector<int>& nums) {
        vector<int> count(10001, 0);
        for (int x : nums) {
            count[x]++;
            if (count[x] >= 2) {
                return x;
            }
        }
        return -1; // unreachable
    }
};
