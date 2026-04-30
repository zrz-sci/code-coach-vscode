/*
 * LeetCode 935 - Knight Dialer
 * Approach: Bottom-Up Dynamic Programming with rolling array
 *
 * The phone pad layout:
 *   1 2 3
 *   4 5 6
 *   7 8 9
 *   * 0 #
 *
 * We precompute for each digit which digits a knight can jump TO.
 * Then dp[digit] = number of sequences of current length ending at that digit.
 * Transition: new_dp[d] = sum of dp[prev] for each prev that can reach d.
 *
 * Time:  O(n)  -- 10 digits, at most 3 neighbors each, iterated n times
 * Space: O(1)  -- only two arrays of size 10
 */

const int MOD = 1e9 + 7;

// ----------------------------------------------------------------
// Adjacency map: jumps[d] = list of digits a knight on d can move to
// ----------------------------------------------------------------
// 0 -> 4, 6       (from bottom-center, L-moves reach 4 and 6)
// 1 -> 6, 8       2 -> 7, 9       3 -> 4, 8
// 4 -> 0, 3, 9    5 -> (none)     6 -> 0, 1, 7
// 7 -> 2, 6       8 -> 1, 3       9 -> 2, 4
//
// Note: digit 5 is completely isolated on the phone pad -- no valid
// knight move lands on another numeric cell from 5.

class Solution {
public:
    int knightDialer(int n) {
        // jumps[d] lists all digits reachable from digit d via one knight move
        vector<vector<int>> jumps = {
            {4, 6},       // 0
            {6, 8},       // 1
            {7, 9},       // 2
            {4, 8},       // 3
            {0, 3, 9},    // 4
            {},           // 5 -- dead end, no valid moves
            {0, 1, 7},    // 6
            {2, 6},       // 7
            {1, 3},       // 8
            {2, 4}        // 9
        };

        // Base case: sequences of length 1.
        // There is exactly 1 way to form a length-1 number ending at each digit.
        vector<long long> dp(10, 1);

        // Build up from length 2 to length n
        for (int step = 2; step <= n; step++) {
            vector<long long> newDp(10, 0);
            for (int digit = 0; digit < 10; digit++) {
                // For each digit, accumulate contributions from all
                // digits that can jump INTO this digit.
                // Equivalently, if knight is at 'prev' and jumps to 'digit',
                // then newDp[digit] += dp[prev].
                // We iterate the other direction: for each digit, look at
                // its neighbor list (which represents where it CAN GO),
                // but we need where it CAME FROM.
                //
                // Since the graph is directed but symmetric in structure,
                // we can build a reverse adjacency or simply iterate:
                // for each prev in [0..9], for each dest in jumps[prev],
                //   newDp[dest] += dp[prev]
                //
                // Let us use that approach for clarity:
            }
            // Actually, let's restructure the loop for correctness:
            // For each source digit, distribute its count to all destinations
            newDp.assign(10, 0);
            for (int src = 0; src < 10; src++) {
                for (int dest : jumps[src]) {
                    newDp[dest] = (newDp[dest] + dp[src]) % MOD;
                }
            }
            dp = newDp;
        }

        // Sum up all digits: total distinct numbers of length n
        long long total = 0;
        for (int digit = 0; digit < 10; digit++) {
            total = (total + dp[digit]) % MOD;
        }
        return static_cast<int>(total);
    }
};

/*
 * Walkthrough with n = 2:
 *
 * Initial dp (length 1): [1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
 *
 * Step 2 -- distribute each source to its destinations:
 *   src=0: dest 4 += 1, dest 6 += 1
 *   src=1: dest 6 += 1, dest 8 += 1
 *   src=2: dest 7 += 1, dest 9 += 1
 *   src=3: dest 4 += 1, dest 8 += 1
 *   src=4: dest 0 += 1, dest 3 += 1, dest 9 += 1
 *   src=5: (nothing)
 *   src=6: dest 0 += 1, dest 1 += 1, dest 7 += 1
 *   src=7: dest 2 += 1, dest 6 += 1
 *   src=8: dest 1 += 1, dest 3 += 1
 *   src=9: dest 2 += 1, dest 4 += 1
 *
 * newDp: [2, 2, 2, 2, 3, 0, 3, 2, 2, 2]
 *   (digit 5 gets 0, digit 4 and 6 each get 3, rest get 2)
 *
 * Total = 2+2+2+2+3+0+3+2+2+2 = 20  -- matches expected output
 *
 * ---------------------------------------------------------------
 * Alternative: Matrix Exponentiation
 *
 * For extremely large n, we could represent the transition as a
 * 10x10 matrix M where M[i][j] = 1 if knight can move from j to i.
 * Then the answer is sum of entries in M^(n-1) * [1,1,...,1]^T.
 * This runs in O(10^3 * log n) but is unnecessary for n <= 5000.
 * ---------------------------------------------------------------
 */
