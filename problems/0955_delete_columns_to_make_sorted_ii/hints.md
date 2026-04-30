# LeetCode 955: Delete Columns to Make Sorted II

[View Solution](./solution.cpp)

## Problem Understanding

You are given an array of `n` strings, all of the same length. You can choose any set of column indices to delete. After deleting the characters at those column indices from every string, the remaining strings must be in lexicographic (dictionary) order. The goal is to find the minimum number of columns to delete. This is NOT the same as Problem 944 (Delete Columns to Make Sorted I), which only requires each column to be individually sorted. Here, the entire rows (strings) must end up in lexicographic order after column deletion. This is also different from Problem 960 (Delete Columns to Make Sorted III), which is the subsequence variant. The key difficulty is that whether a column needs deletion depends on which other columns you have already kept -- the decision is intertwined.

## Key Insights

1. **Greedy column-by-column scanning works.** Process columns from left to right. For each column, check whether keeping it would violate the lexicographic ordering. If it does, delete it (increment the answer). If it doesn't, keep it and update which pairs of adjacent strings are now "settled" (strictly ordered).

2. **The "settled" concept is crucial.** Two adjacent strings `strs[i]` and `strs[i+1]` are "settled" if at some kept column `c`, we already have `strs[i][c] < strs[i+1][c]` (strictly less). Once settled, no future column can break this pair's ordering. We only need to worry about unsettled pairs.

3. **A column is safe to keep if it doesn't create any inversion among unsettled pairs.** Specifically, for every pair `(i, i+1)` that is NOT yet settled, the current column's characters must satisfy `strs[i][col] <= strs[i+1][col]`. If any unsettled pair has `strs[i][col] > strs[i+1][col]`, this column must be deleted.

4. **After keeping a column, update settled pairs.** For each unsettled pair where `strs[i][col] < strs[i+1][col]` (strictly less), mark that pair as settled. They are now guaranteed to be in order regardless of future columns.

5. **Once all pairs are settled, we can stop early.** If every consecutive pair is settled, the answer won't change no matter what we do with remaining columns.

## Approach Categories

| Approach | Strategy | Time | Space |
|----------|----------|------|-------|
| Greedy with Settled Tracking | Scan columns L-to-R, track which pairs are determined | O(n * m) | O(n) |
| Greedy with String Building | Build result strings column by column, check order | O(n * m) | O(n * m) |
| Brute Force (Bitmask) | Try all 2^m subsets of columns, check order | O(2^m * n * m) | O(n * m) |

## Step-by-Step Approach (Greedy with Settled Tracking)

1. **Initialize a boolean array `settled` of size `n-1`**, all set to `false`. `settled[i]` indicates whether the pair `(strs[i], strs[i+1])` is already guaranteed to be in the correct lexicographic order based on columns kept so far.

2. **Initialize a deletion counter `deletions = 0`.**

3. **For each column `col` from 0 to m-1:**
   - Check if this column is safe to keep. Iterate through all unsettled pairs `(i, i+1)`. If any unsettled pair has `strs[i][col] > strs[i+1][col]`, this column is NOT safe.
   - If not safe: increment `deletions` and skip this column entirely (do not update settled).
   - If safe: iterate through unsettled pairs again. For each unsettled pair where `strs[i][col] < strs[i+1][col]` (strictly less), set `settled[i] = true`.

4. **Return `deletions`.**

## Complexity Analysis

**Time Complexity: O(n * m)**
- We process each of the `m` columns once.
- For each column, we check up to `n-1` pairs (only unsettled ones, but worst case all).
- Total: O(n * m), where n = number of strings, m = length of each string.

**Space Complexity: O(n)**
- The `settled` array has `n-1` boolean entries.
- No additional data structures needed beyond a few variables.
- Total auxiliary space: O(n).

## Common Mistakes

1. **Confusing this with Problem 944 (Make Sorted I).** Problem 944 checks each column independently for being sorted. This problem requires the entire rows to be in lexicographic order -- a fundamentally harder condition that depends on column interactions.

2. **Forgetting to distinguish between strict and non-strict comparisons.** When checking if a column is safe, you need `<=` (non-strict) for unsettled pairs. But when marking pairs as settled, you need `<` (strict). Equal characters at a column do NOT settle a pair because future columns could still cause a violation.

3. **Updating settled status when the column is deleted.** If you decide to delete a column, you must NOT update the settled array. Only kept columns contribute to the settled status.

4. **Not realizing greedy is optimal.** It might seem like keeping a column could be suboptimal (maybe deleting it now helps keep more columns later). But this is not the case: keeping a column never hurts because it can only settle more pairs, which makes future columns easier to keep.

5. **Off-by-one in pair indexing.** The settled array should have `n-1` entries (one per adjacent pair), not `n`. Confusing string count with pair count leads to bounds errors.

## Related Patterns

This problem belongs to the **Greedy Column Selection / Lexicographic Ordering** pattern. Related problems include:
- **LeetCode 944: Delete Columns to Make Sorted** -- the simpler version where each column must independently be sorted.
- **LeetCode 960: Delete Columns to Make Sorted III** -- the harder subsequence version requiring dynamic programming (LIS-like).
- **LeetCode 1061: Lexicographically Smallest Equivalent String** -- another problem involving lexicographic constraints with greedy or union-find.
- The "settled pairs" technique is a general pattern useful whenever you need to track which ordering constraints are already resolved.

## Code Template

```
class Solution {
public:
    int minDeletionSize(vector<string>& strs) {
        int n = strs.size();
        int m = strs[0].size();
        int deletions = 0;

        // settled[i] = true means strs[i] < strs[i+1] is already guaranteed
        vector<bool> settled(n - 1, false);

        for (int col = 0; col < m; col++) {
            // Check if keeping this column is safe
            bool safe = true;
            for (int i = 0; i < n - 1; i++) {
                if (!settled[i] && strs[i][col] > strs[i + 1][col]) {
                    safe = false;
                    break;
                }
            }

            if (!safe) {
                deletions++;
            } else {
                // Keep this column; update settled pairs
                for (int i = 0; i < n - 1; i++) {
                    if (!settled[i] && strs[i][col] < strs[i + 1][col]) {
                        settled[i] = true;
                    }
                }
            }
        }

        return deletions;
    }
};
```
