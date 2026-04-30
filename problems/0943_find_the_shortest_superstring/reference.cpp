/*
 * LeetCode 943 - Find the Shortest Superstring
 *
 * Approach: Bitmask DP (Travelling Salesman Problem reduction)
 *
 * Key Idea:
 *   1. Precompute overlap[i][j] = max suffix of words[i] matching prefix of words[j].
 *   2. Use DP with state (bitmask of included words, last word index) to find the
 *      ordering that maximizes total overlap (equivalently minimizes total length).
 *   3. Reconstruct the actual string by tracing back through parent pointers.
 *
 * Time Complexity:  O(n^2 * 2^n + n^2 * L)  where n <= 12, L <= 20
 * Space Complexity: O(n * 2^n)
 *
 * Reference: ./solution.cpp
 */

// ============================================================
// Solution 1: Bitmask DP (Standard Optimal Approach)
// ============================================================
class Solution {
public:
    string shortestSuperstring(vector<string>& words) {
        int n = words.size();

        // Phase 1: Precompute the overlap between every pair of words.
        // overlap[i][j] = length of longest suffix of words[i] that matches
        //                  a prefix of words[j].
        vector<vector<int>> overlap(n, vector<int>(n, 0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                int maxOvlp = min(words[i].size(), words[j].size());
                for (int k = maxOvlp; k >= 1; --k) {
                    // Check if last k chars of words[i] == first k chars of words[j]
                    if (words[i].substr(words[i].size() - k) == words[j].substr(0, k)) {
                        overlap[i][j] = k;
                        break;
                    }
                }
            }
        }

        // Phase 2: Bitmask DP
        // dp[mask][i] = maximum total overlap when we have placed the words
        //               indicated by 'mask', and the last word placed is words[i].
        int fullMask = (1 << n) - 1;
        vector<vector<int>> dp(1 << n, vector<int>(n, 0));
        vector<vector<int>> parent(1 << n, vector<int>(n, -1));

        // No base case initialization needed beyond 0, since a single word
        // has 0 overlap with "nothing before it."

        for (int mask = 0; mask <= fullMask; ++mask) {
            for (int last = 0; last < n; ++last) {
                // Skip if 'last' is not in the current mask
                if (!(mask & (1 << last))) continue;

                // Try to extend by adding word 'next'
                for (int next = 0; next < n; ++next) {
                    if (mask & (1 << next)) continue; // already included

                    int newMask = mask | (1 << next);
                    int newOverlap = dp[mask][last] + overlap[last][next];

                    if (newOverlap > dp[newMask][next]) {
                        dp[newMask][next] = newOverlap;
                        parent[newMask][next] = last;
                    }
                }
            }
        }

        // Phase 3: Find the best ending word
        int bestLast = 0;
        for (int i = 1; i < n; ++i) {
            if (dp[fullMask][i] > dp[fullMask][bestLast]) {
                bestLast = i;
            }
        }

        // Phase 4: Reconstruct the ordering by tracing parent pointers
        vector<int> order;
        int mask = fullMask;
        int cur = bestLast;

        while (cur != -1) {
            order.push_back(cur);
            int prev = parent[mask][cur];
            mask ^= (1 << cur); // remove cur from mask
            cur = prev;
        }

        // order is built in reverse, so reverse it
        reverse(order.begin(), order.end());

        // Phase 5: Build the superstring
        string result = words[order[0]];
        for (int i = 1; i < (int)order.size(); ++i) {
            int prev = order[i - 1];
            int curr = order[i];
            // Append only the non-overlapping suffix of words[curr]
            result += words[curr].substr(overlap[prev][curr]);
        }

        return result;
    }
};

// ============================================================
// Solution 2: Greedy Approximation (Not Optimal, but Instructive)
// ============================================================
//
// Greedy: Repeatedly merge the pair with the maximum overlap until
// one string remains. This is a well-known approximation algorithm
// for the Shortest Superstring problem.
//
// WARNING: This does NOT always produce the optimal answer!
// It gives a 4-approximation in the worst case.
// Included here for educational comparison.
class Solution2 {
public:
    string shortestSuperstring(vector<string>& words) {
        while (words.size() > 1) {
            // Find the pair (i, j) with maximum overlap
            int bestI = 0, bestJ = 1, bestOverlap = 0;
            string bestMerged;

            for (int i = 0; i < (int)words.size(); ++i) {
                for (int j = 0; j < (int)words.size(); ++j) {
                    if (i == j) continue;
                    int ovlp = computeOverlap(words[i], words[j]);
                    if (ovlp > bestOverlap) {
                        bestOverlap = ovlp;
                        bestI = i;
                        bestJ = j;
                        bestMerged = words[i] + words[j].substr(ovlp);
                    }
                }
            }

            // If no overlap found, just concatenate the first two
            if (bestOverlap == 0) {
                bestMerged = words[0] + words[1];
                bestI = 0;
                bestJ = 1;
            }

            // Remove the two words and add the merged result
            // Remove larger index first to avoid shifting issues
            int removeFirst = max(bestI, bestJ);
            int removeSecond = min(bestI, bestJ);
            words.erase(words.begin() + removeFirst);
            words.erase(words.begin() + removeSecond);
            words.push_back(bestMerged);
        }

        return words[0];
    }

private:
    int computeOverlap(const string& a, const string& b) {
        int maxK = min(a.size(), b.size());
        for (int k = maxK; k >= 1; --k) {
            if (a.substr(a.size() - k) == b.substr(0, k)) {
                return k;
            }
        }
        return 0;
    }
};

// ============================================================
// Detailed Walkthrough with Example
// ============================================================
//
// words = ["catg", "ctaagt", "gcta", "ttca", "atgcatc"]
//
// Overlap matrix (partial):
//   overlap["catg"]["atgcatc"]   = 3  ("atg" matches)
//   overlap["gcta"]["ctaagt"]    = 3  ("cta" matches)
//   overlap["ttca"]["catg"]      = 2  ("ca"  matches)
//   overlap["ctaagt"]["ttca"]    = 2  ("tt"  is NOT, actually "t"->1)
//
// The bitmask DP explores all 2^5 = 32 subsets, tracking the best
// last word for each subset.
//
// Optimal ordering found: gcta -> ctaagt -> ttca -> catg -> atgcatc
// Build: "gcta" + "agt" + "tca" + "tg" + "catc" = "gctaagttcatgcatc"
// Length: 16 (vs naive concatenation length = 27)
//
// ============================================================
// Key Implementation Notes
// ============================================================
//
// 1. The DP table has (2^n) * n entries. For n=12: 4096 * 12 = 49,152.
//    Each transition checks n candidates, so total work = 49152 * 12 ~ 590K.
//
// 2. Parent tracking: parent[mask][j] stores which word 'last' was before 'j'
//    in the optimal path ending at state (mask, j). This is essential for
//    reconstruction but easy to forget.
//
// 3. Overlap computation: Use substr comparison. For words up to length 20,
//    this is fast enough. For longer words, KMP or Z-algorithm could speed
//    up the overlap computation, but it is unnecessary here.
//
// 4. The reconstruction loop traces back through the bitmask, removing one
//    word at a time. The order is reversed at the end.
