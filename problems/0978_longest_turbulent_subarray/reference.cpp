[file://solution.cpp](solution.cpp)

// =============================================================================
// LeetCode 978: Longest Turbulent Subarray
// Approach 1: DP with inc/dec Counters
//
// At each position, track:
//   inc = length of turbulent subarray ending here with arr[i-1] < arr[i]
//   dec = length of turbulent subarray ending here with arr[i-1] > arr[i]
//
// Transitions:
//   arr[i] > arr[i-1]  =>  inc = dec + 1, dec = 1
//   arr[i] < arr[i-1]  =>  dec = inc + 1, inc = 1
//   arr[i] == arr[i-1]  =>  inc = 1, dec = 1
//
// Time:  O(n)
// Space: O(1)
// =============================================================================

class Solution {
public:
    int maxTurbulenceSize(vector<int>& arr) {
        int n = arr.size();
        if (n <= 1) return n;

        int inc = 1;    // turbulent length ending with an increase
        int dec = 1;    // turbulent length ending with a decrease
        int result = 1; // global max

        for (int i = 1; i < n; i++) {
            if (arr[i] > arr[i - 1]) {
                // Current pair is increasing.
                // This extends a turbulent subarray that ended with a decrease.
                inc = dec + 1;
                dec = 1;
            } else if (arr[i] < arr[i - 1]) {
                // Current pair is decreasing.
                // This extends a turbulent subarray that ended with an increase.
                dec = inc + 1;
                inc = 1;
            } else {
                // Equal elements break turbulence entirely.
                inc = 1;
                dec = 1;
            }
            result = max(result, max(inc, dec));
        }

        return result;
    }
};

// =============================================================================
// LeetCode 978: Longest Turbulent Subarray
// Approach 2: Explicit Sliding Window
//
// Maintain a window [left, right]. Extend right as long as the turbulence
// condition holds. When it breaks, move left up to right (or right-1
// depending on whether the break was equality or same-direction).
//
// Time:  O(n)
// Space: O(1)
// =============================================================================

class SolutionSlidingWindow {
public:
    // Helper to get comparison sign: -1, 0, or 1
    int compare(int a, int b) {
        if (a < b) return -1;
        if (a > b) return 1;
        return 0;
    }

    int maxTurbulenceSize(vector<int>& arr) {
        int n = arr.size();
        if (n <= 1) return n;

        int result = 1;
        int left = 0;

        for (int right = 1; right < n; right++) {
            int currSign = compare(arr[right - 1], arr[right]);

            if (currSign == 0) {
                // Equal elements: window must restart after this pair
                left = right;
            } else if (right >= 2) {
                int prevSign = compare(arr[right - 2], arr[right - 1]);
                if (currSign == prevSign) {
                    // Same direction as previous pair: shrink window
                    // The new window starts at right - 1
                    left = right - 1;
                }
                // If signs differ, the window extends naturally
            }
            // else: right == 1 and currSign != 0, window [0,1] is valid

            result = max(result, right - left + 1);
        }

        return result;
    }
};

// =============================================================================
// LeetCode 978: Longest Turbulent Subarray
// Approach 3: DP Array (More Explicit)
//
// Use two full DP arrays instead of rolling variables, for clarity.
// dp_inc[i] = length of turbulent subarray ending at i with arr[i-1] < arr[i]
// dp_dec[i] = length of turbulent subarray ending at i with arr[i-1] > arr[i]
//
// This is functionally identical to Approach 1 but uses O(n) space.
// Useful for understanding the recurrence before optimizing to O(1) space.
//
// Time:  O(n)
// Space: O(n)
// =============================================================================

class SolutionDPArray {
public:
    int maxTurbulenceSize(vector<int>& arr) {
        int n = arr.size();
        if (n <= 1) return n;

        // dp_inc[i]: max turbulent subarray length ending at i
        // where arr[i-1] < arr[i] (last step was an increase)
        vector<int> dp_inc(n, 1);
        // dp_dec[i]: max turbulent subarray length ending at i
        // where arr[i-1] > arr[i] (last step was a decrease)
        vector<int> dp_dec(n, 1);

        int result = 1;

        for (int i = 1; i < n; i++) {
            if (arr[i] > arr[i - 1]) {
                // Increase: extend a subarray that ended with decrease
                dp_inc[i] = dp_dec[i - 1] + 1;
                dp_dec[i] = 1;
            } else if (arr[i] < arr[i - 1]) {
                // Decrease: extend a subarray that ended with increase
                dp_dec[i] = dp_inc[i - 1] + 1;
                dp_inc[i] = 1;
            } else {
                // Equal: both reset
                dp_inc[i] = 1;
                dp_dec[i] = 1;
            }
            result = max(result, max(dp_inc[i], dp_dec[i]));
        }

        return result;
    }
};
