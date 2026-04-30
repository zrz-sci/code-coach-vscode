// LeetCode 956: Tallest Billboard
// Difficulty: Hard
// Tags: Array, Dynamic Programming
// Link: https://leetcode.com/problems/tallest-billboard/
//
// Approach: DP on the difference between two support heights.
// dp[diff] = maximum height of the taller support when the
// difference between the two supports is exactly diff.
// Answer is dp[0] (both supports equal).
//
// Time:  O(n * S) where S = sum of all rods (<= 5000)
// Space: O(S) for the DP map

// ============================================================
// Solution 1: DP on Difference with HashMap (Recommended)
// ============================================================
// State: dp[diff] = max height of taller support when |left - right| = diff
// Transitions for each rod r:
//   1. Skip the rod: no change
//   2. Add to taller side: diff -> diff+r, height -> height+r
//   3. Add to shorter side: diff -> |diff-r|, height updates accordingly

class Solution {
public:
    int tallestBillboard(vector<int>& rods) {
        // dp[diff] = max height of the taller support
        // Base case: both supports empty -> diff=0, height=0
        unordered_map<int, int> dp;
        dp[0] = 0;

        for (int rod : rods) {
            // CRITICAL: copy dp before processing this rod
            // Without this, a single rod could be "used" multiple times
            auto prev = dp;

            for (auto& [diff, tall] : prev) {
                // Transition A: Add rod to the TALLER support
                // New difference = diff + rod (gap widens)
                // New taller height = tall + rod
                int keyA = diff + rod;
                dp[keyA] = max(dp.count(keyA) ? dp[keyA] : 0, tall + rod);

                // Transition B: Add rod to the SHORTER support
                // The shorter support height = tall - diff
                // After adding rod, shorter becomes (tall - diff + rod)
                //
                // Case B1: rod <= diff (shorter still shorter)
                //   New taller = tall (unchanged)
                //   New diff = diff - rod
                //
                // Case B2: rod > diff (shorter overtakes)
                //   New taller = tall - diff + rod = tall + (rod - diff)
                //   New diff = rod - diff
                //
                // Unified: new diff = abs(diff - rod)
                //          new tall = max(tall, tall - diff + rod)
                int keyB = abs(diff - rod);
                int newTall = max(tall, tall - diff + rod);
                dp[keyB] = max(dp.count(keyB) ? dp[keyB] : 0, newTall);
            }
        }

        // dp[0] = max height when both supports are equal
        return dp.count(0) ? dp[0] : 0;
    }
};


// ============================================================
// Solution 2: DP with Array (Fixed-size, faster)
// ============================================================
// Since sum(rods) <= 5000, the maximum difference is 5000.
// Use an array of size 5001 instead of a hash map.
// dp[d] = max height of taller support when difference = d.
// Initialize all to -1 (invalid), except dp[0] = 0.

class SolutionArray {
public:
    int tallestBillboard(vector<int>& rods) {
        int totalSum = 0;
        for (int r : rods) totalSum += r;

        // dp[d] = max taller height for difference d
        // -1 means this state is unreachable
        vector<int> dp(totalSum + 1, -1);
        dp[0] = 0;

        for (int rod : rods) {
            // Copy current state
            vector<int> prev = dp;

            for (int d = 0; d <= totalSum; d++) {
                if (prev[d] < 0) continue;  // unreachable state

                // Transition A: add rod to taller side
                // new diff = d + rod, new tall = prev[d] + rod
                if (d + rod <= totalSum) {
                    dp[d + rod] = max(dp[d + rod], prev[d] + rod);
                }

                // Transition B: add rod to shorter side
                // new diff = |d - rod|
                int newDiff = abs(d - rod);
                int newTall = max(prev[d], prev[d] - d + rod);
                dp[newDiff] = max(dp[newDiff], newTall);
            }
        }

        return max(dp[0], 0);
    }
};


// ============================================================
// Solution 3: Meet in the Middle
// ============================================================
// Split rods into two halves. For each half, enumerate all
// possible (leftSum, rightSum) pairs (3^(n/2) possibilities).
// Store results from first half in a map keyed by difference.
// For each result from second half, look up complementary
// difference in the map.

class SolutionMeetMiddle {
public:
    int tallestBillboard(vector<int>& rods) {
        int n = rods.size();
        int half = n / 2;

        // Generate all (left, right) sum pairs for a subset of rods
        // Returns map: difference -> max(left sum) for that difference
        auto generate = [&](int start, int end) {
            unordered_map<int, int> best;
            best[0] = 0;

            for (int i = start; i < end; i++) {
                auto prev = best;
                for (auto& [diff, leftSum] : prev) {
                    // rightSum = leftSum - diff (if diff = left - right)

                    // Add rod to left side
                    int d1 = diff + rods[i];
                    best[d1] = max(best.count(d1) ? best[d1] : 0,
                                   leftSum + rods[i]);

                    // Add rod to right side
                    int d2 = diff - rods[i];
                    best[d2] = max(best.count(d2) ? best[d2] : 0, leftSum);
                }
            }
            return best;
        };

        auto firstHalf = generate(0, half);
        auto secondHalf = generate(half, n);

        int result = 0;

        // Combine: for each state in firstHalf, find complementary
        // state in secondHalf where differences cancel out
        for (auto& [diff, leftSum] : firstHalf) {
            // Need secondHalf to have difference = -diff
            if (secondHalf.count(-diff)) {
                // Total left sum = leftSum + secondHalf[-diff]
                // At diff=0 combined, both supports equal
                result = max(result, leftSum + secondHalf[-diff]);
            }
        }

        return result;
    }
};


// ============================================================
// Walkthrough with Example: rods = [1, 2, 3, 6]
// ============================================================
//
// Using Solution 1 (DP on Difference):
//
// Initial: dp = {0: 0}
//
// Rod 1 (r=1):
//   From (diff=0, tall=0):
//     Add to taller: dp[1] = max(_, 0+1) = 1
//     Add to shorter: dp[1] = max(1, max(0, 0-0+1)) = max(1, 1) = 1
//   dp = {0: 0, 1: 1}
//
// Rod 2 (r=2):
//   From (0, 0): dp[2] = 2, dp[2] = max(2, 2) = 2
//   From (1, 1): dp[3] = 3, dp[1] = max(1, max(1, 1-1+2)) = 2
//   dp = {0: 0, 1: 2, 2: 2, 3: 3}
//
// Rod 3 (r=3):
//   From (0, 0): dp[3] = max(3, 3), dp[3] = max(3, 3) = 3
//   From (1, 2): dp[4] = 5, dp[2] = max(2, max(2, 2-1+3)) = 4
//   From (2, 2): dp[5] = 5, dp[1] = max(2, max(2, 2-2+3)) = 3
//   From (3, 3): dp[6] = 6, dp[0] = max(0, max(3, 3-3+3)) = 3
//   dp now has dp[0] = 3
//
// Rod 6 (r=6):
//   From (0, 3): dp[6] = max(_, 9), dp[6] = max(9, max(3, 3+6)) = 9
//   ... many transitions ...
//   From (3, 3+?) ... eventually dp[0] gets updated
//   After processing: dp[0] = 6
//
// Answer: dp[0] = 6
// Explanation: {1,2,3} on one side (sum=6), {6} on the other (sum=6).
