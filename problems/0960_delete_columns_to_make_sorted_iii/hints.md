# LeetCode 960 - Delete Columns to Make Sorted III

[View Solution](./solution.cpp)

## Problem Understanding

You are given an array of `n` strings `strs`, all of the same length `m`. You may choose a set of column indices to delete. After deletion, every remaining string (row) must be individually in lexicographic (non-decreasing) order. The goal is to return the minimum number of columns that must be deleted to achieve this. Note that the rows do not need to be sorted relative to each other -- each row must independently be non-decreasing when read left to right after the chosen columns are removed.

## Key Insights

1. **Reframing as Longest Increasing Subsequence (LIS)**: Instead of minimizing columns deleted, maximize columns kept. The kept columns must form a subsequence such that for every row, the characters at those column positions are non-decreasing. This is exactly a variant of LIS, but the comparison involves ALL rows simultaneously.

2. **Column-wise comparison**: Column `j` can follow column `i` (i < j) in the kept set if and only if `strs[k][i] <= strs[k][j]` for every row `k`. This multi-row constraint replaces the simple `a[i] <= a[j]` comparison in standard LIS.

3. **DP on columns**: Let `dp[j]` = the length of the longest valid subsequence of columns ending at column `j`. For each column `j`, check all previous columns `i < j` to see if column `j` can extend column `i`'s subsequence.

4. **Answer derivation**: The answer is `m - max(dp[j])` for all `j`, since we want to delete as few columns as possible, which is the total columns minus the longest valid subsequence.

## Approach Categories

| Approach | Strategy | Time | Space |
|----------|----------|------|-------|
| DP (LIS variant) | dp[j] = longest valid column subsequence ending at j | O(m^2 * n) | O(m) |
| DP with patience sorting | Optimize LIS with binary search (hard due to multi-row) | O(m^2 * n) | O(m) |

## Step-by-Step Solution (DP - LIS Variant)

1. Let `m = strs[0].length()` and `n = strs.size()`.
2. Create array `dp` of size `m`, initialized to 1 (each column alone is a valid subsequence of length 1).
3. For each column `j` from 1 to m-1:
   - For each column `i` from 0 to j-1:
     - Check if column `i` can precede column `j`: verify `strs[k][i] <= strs[k][j]` for all `k` in [0, n-1].
     - If yes, update `dp[j] = max(dp[j], dp[i] + 1)`.
4. Find the maximum value in `dp`. Call it `maxKept`.
5. Return `m - maxKept`.

## Step-by-Step Solution (Optimized Inner Check)

1. Same framework as above, but precompute a boolean compatibility matrix `canFollow[i][j]` for all column pairs `(i, j)` where `i < j`.
2. `canFollow[i][j] = true` if for all rows `k`, `strs[k][i] <= strs[k][j]`.
3. This precomputation takes O(m^2 * n) time but separates the checking logic from the DP logic, making the code cleaner.
4. Run the LIS DP using the precomputed matrix:
   - For each `j`, for each `i < j`: if `canFollow[i][j]`, then `dp[j] = max(dp[j], dp[i] + 1)`.
5. Return `m - max(dp)`.

## Complexity Analysis

- **Time Complexity**: O(m^2 * n). There are O(m^2) column pairs to check, and each check examines all `n` rows. The DP itself is O(m^2) with O(n) per transition.
- **Space Complexity**: O(m) for the DP array. If precomputing the compatibility matrix, O(m^2) additional space is used, though this is optional.

## Common Mistakes

1. **Confusing with Delete Columns I/II**: Problem 944 (I) checks if columns are sorted across rows. Problem 955 (II) requires rows to be sorted relative to each other. This problem (III) requires each row to be individually sorted. They have very different solution structures.
2. **Trying to apply standard LIS binary search optimization**: The standard O(n log n) LIS optimization using patience sorting does not directly apply because the comparison is multi-dimensional (must check all rows). The O(m^2 * n) approach is the standard solution.
3. **Wrong direction of optimization**: Trying to minimize deletions directly rather than maximizing kept columns. The LIS formulation (maximize kept) is much cleaner.
4. **Checking row order instead of column order**: The non-decreasing constraint is within each row (left to right), not between rows (top to bottom).

## Related Problems

- **LeetCode 300 - Longest Increasing Subsequence**: The core algorithmic pattern. This problem is a multi-dimensional generalization of LIS.
- **LeetCode 944 - Delete Columns to Make Sorted**: Simpler variant where you check if each column is sorted across rows.
- **LeetCode 955 - Delete Columns to Make Sorted II**: Intermediate variant where rows must be in lexicographic order relative to each other.
- **LeetCode 1143 - Longest Common Subsequence**: Another subsequence DP problem with similar O(n^2) DP structure.
