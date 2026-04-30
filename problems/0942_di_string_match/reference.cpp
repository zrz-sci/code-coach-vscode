/*
 * LeetCode 942 - DI String Match
 *
 * Approach: Greedy Two-Pointer
 *
 * Key Idea:
 *   Maintain a range [lo, hi] initialized to [0, n].
 *   - On 'I', assign lo (smallest available) so the next value is guaranteed larger.
 *   - On 'D', assign hi (largest available) so the next value is guaranteed smaller.
 *   After processing all characters, lo == hi; assign the remaining value to the last position.
 *
 * Time Complexity: O(n) - single pass through the string
 * Space Complexity: O(n) - for the output array; O(1) auxiliary
 *
 * Reference: ./solution.cpp
 */

// ============================================================
// Solution 1: Greedy Two-Pointer (Optimal)
// ============================================================
class Solution {
public:
    vector<int> diStringMatch(string s) {
        int n = s.size();
        int lo = 0, hi = n;
        vector<int> result(n + 1);

        for (int i = 0; i < n; ++i) {
            if (s[i] == 'I') {
                // Need perm[i] < perm[i+1], so pick the smallest available
                result[i] = lo;
                ++lo;
            } else {
                // Need perm[i] > perm[i+1], so pick the largest available
                result[i] = hi;
                --hi;
            }
        }

        // After the loop, lo == hi. Assign the last remaining number.
        result[n] = lo; // equivalently, result[n] = hi

        return result;
    }
};

// ============================================================
// Solution 2: Stack-Based Group Processing
// ============================================================
//
// Alternative approach: Process groups of consecutive characters.
// For a group of consecutive 'D' characters of length k starting
// at position i, we need a decreasing sequence of length k+1.
// We push indices onto a stack during 'D' runs and pop/assign
// when we encounter 'I' or reach the end of the string.
//
// This approach is conceptually interesting because it reveals
// the structure: each "D-group" forms a locally decreasing block
// and each "I" is a natural separator.
//
// Time: O(n), Space: O(n) -- same asymptotic complexity as Solution 1
// but with higher constant factor due to stack operations.
class Solution2 {
public:
    vector<int> diStringMatch(string s) {
        int n = s.size();
        vector<int> result(n + 1);

        // We use a stack to buffer indices during 'D' runs.
        // When we see 'I' or reach the end, we pop and assign
        // increasing values to the buffered positions.
        vector<int> stk; // using vector as stack for clarity
        int counter = 0; // next value to assign

        for (int i = 0; i <= n; ++i) {
            stk.push_back(i);

            // Flush the stack when we see 'I' or when we reach the end
            if (i == n || s[i] == 'I') {
                while (!stk.empty()) {
                    result[stk.back()] = counter;
                    stk.pop_back();
                    ++counter;
                }
            }
        }

        return result;
    }
};

// ============================================================
// Detailed Walkthrough with Example
// ============================================================
//
// Example: s = "IDID"  =>  n = 4, range [0, 4]
//
// Step 0: s[0] = 'I' => result[0] = lo = 0, lo becomes 1
//         range is now [1, 4]
//
// Step 1: s[1] = 'D' => result[1] = hi = 4, hi becomes 3
//         range is now [1, 3]
//
// Step 2: s[2] = 'I' => result[2] = lo = 1, lo becomes 2
//         range is now [2, 3]
//
// Step 3: s[3] = 'D' => result[3] = hi = 3, hi becomes 2
//         range is now [2, 2]
//
// Final:  result[4] = lo = hi = 2
//
// Result: [0, 4, 1, 3, 2]
//
// Verification:
//   perm[0]=0 < perm[1]=4  => 'I' CORRECT
//   perm[1]=4 > perm[2]=1  => 'D' CORRECT
//   perm[2]=1 < perm[3]=3  => 'I' CORRECT
//   perm[3]=3 > perm[4]=2  => 'D' CORRECT
//
// ============================================================
// Why Greedy is Correct: Proof Sketch
// ============================================================
//
// Claim: The two-pointer greedy always produces a valid permutation.
//
// Proof: We maintain the invariant that after processing i characters,
//   - lo and hi satisfy lo + (number of 'I' seen) + (number of 'D' seen) == hi + i (*)
//   - All assigned values are distinct (each is a unique boundary value)
//   - The constraint for position i is satisfied:
//     * If s[i] == 'I': we assigned lo to position i. The next assignment
//       (whether lo+1 or some hi') satisfies lo < lo+1 and lo < hi' since
//       lo < hi at every step (until convergence).
//     * If s[i] == 'D': we assigned hi to position i. The next assignment
//       satisfies hi > hi-1 and hi > lo' for similar reasoning.
//
// Since we use each integer in [0, n] exactly once (lo starts at 0 and
// increases, hi starts at n and decreases, they meet in the middle),
// the result is a valid permutation of [0, n].
//
// ============================================================
// Edge Cases
// ============================================================
//
// s = "I"     => [0, 1]          (single increase)
// s = "D"     => [1, 0]          (single decrease)
// s = "III"   => [0, 1, 2, 3]   (fully increasing)
// s = "DDD"   => [3, 2, 1, 0]   (fully decreasing)
// s = "DDII"  => [4, 3, 0, 1, 2] (mixed pattern)
// s = "IIDD"  => [0, 1, 4, 3, 2] (mixed pattern)
