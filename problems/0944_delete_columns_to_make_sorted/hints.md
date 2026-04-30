# LeetCode 944 - Delete Columns to Make Sorted

[View Solution](./solution.cpp)

## 1. Problem Understanding (Read & Rephrase)

You are given an array of `n` strings `strs`, all of the same length. If you arrange them into a grid (one string per row), you want to identify which columns are NOT sorted in non-decreasing (lexicographic) order from top to bottom. Return the count of such unsorted columns -- these are the columns you would need to delete.

A column `j` is "sorted" if for every pair of consecutive rows `i` and `i+1`, we have `strs[i][j] <= strs[i+1][j]`. If any adjacent pair violates this, the entire column is considered unsorted and must be deleted.

**Restatement:** Count how many column indices `j` have at least one pair of adjacent rows where the character in the upper row is strictly greater than the character in the lower row.

**Important clarification:** "Sorted" means non-decreasing (equal values are allowed). A column like `['a', 'a', 'b']` is sorted. A column like `['b', 'a', 'c']` is NOT sorted because `'b' > 'a'`.

## 2. Key Observations & Insights

1. **Independent columns:** Each column can be checked independently. Whether column `j` is sorted has no bearing on whether column `k` is sorted.

2. **Early termination per column:** As soon as you find one adjacent pair where `strs[i][j] > strs[i+1][j]`, you can mark column `j` as unsorted and move to the next column. No need to check the rest of the rows for that column.

3. **Character comparison:** Since all strings consist of lowercase English letters, character comparison is just ASCII comparison. `'a' < 'b' < ... < 'z'`.

4. **The problem only asks for the COUNT of columns to delete**, not which columns, nor the resulting strings. This simplifies the task to pure counting.

5. **Grid dimensions:** Let `m` = number of strings (rows) and `L` = length of each string (columns). You iterate over all `L` columns, and for each column, you check up to `m - 1` consecutive pairs.

## 3. Approach Categories

| Approach | Time | Space | Notes |
|---|---|---|---|
| Column-by-column scan | O(m * L) | O(1) | Optimal, straightforward |
| Extract columns and check sorted | O(m * L) | O(m) | Same time, extra space for extracted column |
| Transpose matrix then check | O(m * L) | O(m * L) | Wastes space, no benefit |

**Recommended:** Column-by-column scan with early break per column. Simple, optimal, and clean.

## 4. Step-by-Step Solution (Column Scan)

1. Let `m = strs.size()` (number of rows) and `L = strs[0].size()` (number of columns).
2. Initialize a counter `deletions = 0`.
3. For each column `j` from `0` to `L - 1`:
   a. For each row `i` from `0` to `m - 2`:
      - If `strs[i][j] > strs[i+1][j]`, increment `deletions`, then break out of the inner loop (no need to check further rows for this column).
   b. If the inner loop completes without breaking, the column is sorted -- do nothing.
4. Return `deletions`.

**Why this works:** We are directly implementing the definition of "unsorted column." A column is unsorted if any adjacent pair is out of order. We check every column and count the unsorted ones.

## 5. Complexity Analysis

**Time Complexity:** O(m * L) where `m` is the number of strings and `L` is the length of each string.
- In the worst case (all columns are sorted), we check every character in the grid.
- In the best case (every column is unsorted at the first row pair), we do O(L) work.
- Expected case with early termination is often better than worst case.

**Space Complexity:** O(1) -- we only use a counter variable and loop indices. The input is read in place.

## 6. Common Mistakes & Edge Cases

- **Comparing wrong direction:** Make sure you compare `strs[i][j]` with `strs[i+1][j]` (top to bottom within a column), not `strs[i][j]` with `strs[i][j+1]` (left to right within a row). The problem asks about column-wise sorting, not row-wise.
- **Using `<` instead of `<=`:** A column where all characters are the same is sorted. The condition for "unsorted" is `strs[i][j] > strs[i+1][j]` (strictly greater), not `>=`.
- **Forgetting the break optimization:** While not a correctness issue, forgetting to break early means you do unnecessary work. For large inputs this matters for constant-factor performance.
- **Edge case -- single row:** If there is only one string (`m = 1`), every column is trivially sorted. The answer is `0`.
- **Edge case -- single character strings:** If each string has length 1 (`L = 1`), there is exactly one column to check.
- **Edge case -- all columns unsorted:** `strs = ["zyx", "wvu", "tsr"]` -- every column is strictly decreasing, so all 3 columns are deleted. Answer is 3.
- **Edge case -- all columns sorted:** `strs = ["abc", "bcd", "cde"]` -- every column is non-decreasing. Answer is 0.

## 7. Related LeetCode Problems

| Problem | Relationship |
|---|---|
| **955. Delete Columns to Make Sorted II** | Harder variant: columns must make entire rows lexicographically sorted after deletion (greedy approach needed) |
| **960. Delete Columns to Make Sorted III** | Even harder: LIS-based approach on columns, finding max columns to keep |
| **1122. Relative Sort Array** | Sorting with custom ordering |
| **242. Valid Anagram** | Basic string/character comparison operations |
| **56. Merge Intervals** | Array scanning pattern (iterate and decide) |

## 8. Interview Tips & Follow-ups

- **This is a warm-up problem:** In an interview, you should solve this quickly (under 5 minutes) and use the saved time for follow-up discussion or harder problems.
- **Mention the early break:** Even though the optimization does not change the asymptotic complexity, interviewers appreciate seeing that you think about constant-factor improvements.
- **Clarify "sorted" means non-decreasing:** Ask the interviewer whether equal values are considered sorted. (They are, based on the problem statement.)
- **Follow-up -- Delete Columns to Make Sorted II (LC 955):** The interviewer might escalate to this harder variant where the goal is not just to check each column independently, but to ensure that the remaining columns, when read row by row, produce lexicographically sorted rows. This requires a greedy approach tracking which row pairs are already "settled."
- **Follow-up -- Delete Columns to Make Sorted III (LC 960):** The hardest variant asks for the minimum columns to delete so that the remaining columns form rows that are each individually sorted. This is equivalent to finding the longest increasing subsequence among columns.
- **Follow-up -- return the actual column indices:** A trivial extension: instead of counting, collect the column indices that are unsorted. Same algorithm, just store indices instead of incrementing a counter.
- **Discuss the problem structure:** This problem is essentially "count inversions per column, but only check if there is at least one inversion." This perspective connects it to the broader topic of inversion counting in arrays.
