# LeetCode 961 - N-Repeated Element in Size 2N Array

[View Solution](./solution.cpp)

## Problem Understanding

You are given an integer array `nums` of length `2n`. The array contains `n + 1` unique values: exactly one value appears `n` times, and the other `n` values each appear exactly once. Your task is to find and return the element that appears `n` times. For example, in `[1, 2, 3, 3]` (length 4, so n=2), the element `3` appears 2 times, which is the answer. The key structural property is that the repeated element takes up exactly half the array.

## Key Insights

1. **Hash set / Hash map approach**: The most straightforward approach is to use a hash set. Iterate through the array and insert each element. The first time an insertion fails (element already in set), that element is the repeated one. Since the repeated element appears `n` times out of `2n` slots, it must be encountered as a duplicate within the first `n + 1` elements at most.

2. **Pigeonhole / spacing argument**: Since the repeated element occupies `n` out of `2n` positions, it occupies exactly half the array. By the pigeonhole principle, among any 3 consecutive elements, at least two must be the repeated element. This means we only need to check windows of size 2 or 3: if `nums[i] == nums[i+1]` or `nums[i] == nums[i+2]` for any `i`, we have found the answer. Special case: also check `nums[0] == nums[n-1]` (or similar wrap-around).

3. **Sorting approach**: Sort the array. The repeated element must be at either `nums[n-1]` or `nums[n]` (the two middle positions), because `n` copies of the same value must cluster around the center after sorting.

4. **Randomized approach**: Pick a random element. With probability 1/2, it is the repeated element. Verify by scanning. Expected O(n) time with very high probability of fast termination.

## Approach Categories

| Approach | Strategy | Time | Space |
|----------|----------|------|-------|
| Hash Set | Insert until duplicate found | O(n) | O(n) |
| Spacing / Pigeonhole | Check pairs at distance 1 and 2 | O(n) | O(1) |
| Sorting | Sort, check middle elements | O(n log n) | O(1) or O(n) |
| Randomized | Random pick + verify | O(n) expected | O(1) |
| Boyer-Moore Voting (modified) | Candidate voting | O(n) | O(1) |

## Step-by-Step Solution (Hash Set)

1. Create an empty unordered_set `seen`.
2. Iterate through each element `x` in `nums`:
   - If `x` is already in `seen`, return `x` immediately.
   - Otherwise, insert `x` into `seen`.
3. This loop is guaranteed to find the duplicate within the first `n + 1` elements because only `n` unique values exist besides the repeated one.

## Step-by-Step Solution (Spacing / Pigeonhole - O(1) Space)

1. The repeated element appears `n` times in an array of size `2n`. So on average every other element is the repeated one. This means the repeated element must appear at distance 1 or 2 from another copy of itself.
2. Iterate from `i = 0` to `len - 2`:
   - If `nums[i] == nums[i + 1]`, return `nums[i]`.
3. If no adjacent duplicates found, iterate from `i = 0` to `len - 3`:
   - If `nums[i] == nums[i + 2]`, return `nums[i]`.
4. Edge case: if the repeated elements are spread as far apart as possible (alternating pattern), the first and last elements will match. Check `nums[0] == nums[nums.size() - 1]` as a fallback.
5. One of these checks is guaranteed to succeed.

## Complexity Analysis

- **Hash Set**: Time O(n), Space O(n). Simple and reliable.
- **Spacing / Pigeonhole**: Time O(n), Space O(1). Optimal in both dimensions.
- **Sorting**: Time O(n log n), Space O(1) if in-place sort. Overkill for this problem.
- **Randomized**: Expected Time O(n), Space O(1). Each random pick has 50% chance of being the answer.

## Common Mistakes

1. **Overcomplicating the problem**: This is an Easy problem. Using complex algorithms like segment trees or binary indexed trees is unnecessary. A simple hash set or the spacing trick is sufficient.
2. **Missing the spacing edge case**: When using the O(1) space approach, forgetting to check distance-2 pairs. The repeated elements might not always be adjacent (e.g., `[1, 2, 1, 2, 1]` -- no adjacent duplicates but distance-2 duplicates exist).
3. **Wrong middle index after sorting**: After sorting, the repeated element is at index `n-1` or `n`. Checking only one of these indices may miss the answer (e.g., `[1, 2, 2, 3]` has the answer at index 1-2, while `[1, 1, 2, 3]` has it at index 0-1).
4. **Assuming the repeated element is the majority**: The repeated element appears exactly `n` times in `2n` elements, so it is exactly half, not a strict majority. Boyer-Moore voting needs adjustment.

## Related Problems

- **LeetCode 136 - Single Number**: The complementary problem: find the unique element among duplicates. XOR-based approach.
- **LeetCode 169 - Majority Element**: Find the element appearing more than n/2 times. Boyer-Moore voting applies directly there.
- **LeetCode 217 - Contains Duplicate**: Basic duplicate detection using hash set, same core technique.
- **LeetCode 442 - Find All Duplicates in an Array**: Finding duplicates with O(1) space using array index tricks.
