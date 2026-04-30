# LeetCode 962 - Maximum Width Ramp

[View Solution](./solution.cpp)

## Problem Understanding

A "ramp" in an integer array `nums` is a pair of indices `(i, j)` where `i < j` and `nums[i] <= nums[j]`. The "width" of the ramp is `j - i`. Given the array, you need to find the maximum width among all valid ramps. If no ramp exists (the array is strictly decreasing), return 0. For example, in `[6, 0, 8, 2, 1, 5]`, the widest ramp is `(1, 5)` with `nums[1]=0 <= nums[5]=5`, giving width 4.

## Key Insights

1. **Monotonic stack for candidate left endpoints**: Build a decreasing stack of indices from left to right. Only push index `i` if `nums[i]` is strictly less than the top of the stack. This stack contains all potential "best" left endpoints. Why? If `nums[i] >= nums[stack.top()]` and `i > stack.top()`, then `stack.top()` is always a better (or equal) left endpoint than `i` for any future `j`, so `i` is never needed.

2. **Right-to-left scan for maximum width**: After building the decreasing stack, scan from right to left. For each `j` from `n-1` down to 0, while the stack is non-empty and `nums[stack.top()] <= nums[j]`, pop the stack and update the answer with `j - stack.top()`. Since `j` is decreasing, once a left endpoint is matched, it cannot do better with a smaller `j`.

3. **Sorting with index tracking**: Create pairs `(value, index)`, sort by value. Then sweep through the sorted pairs, tracking the minimum index seen so far. For each pair, the ramp width is `current_index - min_index_so_far`. This works because sorting ensures `nums[min_index] <= nums[current_index]`.

4. **Suffix maximum array**: Build a suffix maximum array where `suffMax[j] = max(nums[j..n-1])`. Then for each `i` from left, use binary search or two-pointer to find the largest `j` where `suffMax[j] >= nums[i]`.

## Approach Categories

| Approach | Strategy | Time | Space |
|----------|----------|------|-------|
| Monotonic Stack | Decreasing stack + right-to-left scan | O(n) | O(n) |
| Sort by Value | Sort (value, index) pairs, track min index | O(n log n) | O(n) |
| Suffix Maximum + Two Pointers | Build suffix max, sweep with two pointers | O(n) | O(n) |
| Brute Force | Check all pairs (i, j) | O(n^2) | O(1) |

## Step-by-Step Solution (Monotonic Stack)

1. Initialize an empty stack `stk` and push index 0.
2. **Build decreasing stack (left to right)**: For `i` from 1 to `n-1`, if `nums[i] < nums[stk.top()]`, push `i`. This ensures the stack contains indices with strictly decreasing values -- these are the only candidates for left endpoints of a maximum-width ramp.
3. **Scan right to left for matches**: Initialize `ans = 0`. For `j` from `n-1` down to 0:
   - While `stk` is non-empty and `nums[stk.top()] <= nums[j]`:
     - Update `ans = max(ans, j - stk.top())`.
     - Pop `stk.top()` (this index is consumed; no larger `j` exists to the right).
4. Return `ans`.

## Step-by-Step Solution (Sort by Value)

1. Create an array of pairs `{nums[i], i}` for all `i`.
2. Sort this array by value (ascending). Break ties by index (ascending).
3. Initialize `minIdx = n` and `ans = 0`.
4. Iterate through the sorted pairs:
   - `minIdx = min(minIdx, current_index)`.
   - `ans = max(ans, current_index - minIdx)`.
5. Return `ans`. The sorting guarantees that for any pair we examine, all previously seen pairs have values <= the current value, so `minIdx` is always a valid left endpoint.

## Complexity Analysis

- **Monotonic Stack**: Time O(n), Space O(n). Each index is pushed and popped at most once.
- **Sort by Value**: Time O(n log n) for sorting, Space O(n) for the pairs array.
- **Suffix Maximum + Two Pointers**: Time O(n), Space O(n) for the suffix array.
- **Brute Force**: Time O(n^2), Space O(1). Too slow for n up to 5 * 10^4.

## Common Mistakes

1. **Wrong stack direction**: Building an increasing stack instead of a decreasing one. The stack must contain strictly decreasing values to capture all useful left endpoints.
2. **Scanning left-to-right for matching**: After building the stack, scanning left-to-right for `j` misses the property that we want the maximum `j` for each stack element. The right-to-left scan with popping is essential for O(n) time.
3. **Not popping the stack during matching**: Keeping matched elements on the stack leads to incorrect results. Once index `i` from the stack is matched with `j`, no future (smaller) `j` can produce a wider ramp with `i`.
4. **Tie-breaking in sort approach**: When sorting pairs with equal values, not handling index order correctly can lead to negative ramp widths or missed optimal pairs.

## Related Problems

- **LeetCode 1124 - Longest Well-Performing Interval**: Uses a similar monotonic stack + right-to-left scan pattern.
- **LeetCode 84 - Largest Rectangle in Histogram**: Classic monotonic stack problem with similar push/pop logic.
- **LeetCode 739 - Daily Temperatures**: Monotonic stack for finding the next greater element, related pattern.
- **LeetCode 1019 - Next Greater Node In Linked List**: Monotonic stack application for next-greater queries.
