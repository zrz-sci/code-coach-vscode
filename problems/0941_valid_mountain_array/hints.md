[file://solution.cpp](solution.cpp)

# LeetCode 941 - Valid Mountain Array

## 1. Problem Understanding (Restate & Clarify)

Given an integer array `arr`, determine whether it is a valid mountain array. A valid mountain array must satisfy ALL of the following conditions:
1. The array length is at least 3.
2. There exists an index `i` (with 0 < i < arr.length - 1) such that the elements strictly increase from the beginning up to index `i`, and then strictly decrease from index `i` to the end.
3. The increase and decrease must be STRICT -- no equal adjacent elements allowed.

In other words, the array must go strictly up, reach exactly one peak (not at the first or last position), and then go strictly down.

Key clarifications:
- The peak cannot be at index 0 or index n-1 (there must be both an ascending and descending part).
- Adjacent elements cannot be equal -- the problem requires strict inequality in both directions.
- The array can have up to 10^4 elements with values up to 10^4.

## 2. Approach Brainstorming (At Least 2 Approaches)

**Approach A: Find Peak Then Verify**
Scan the array to find the index of the maximum element. Then verify that the array strictly increases from start to this peak and strictly decreases from this peak to the end. Handle edge cases where the peak is at position 0 or n-1.

**Approach B: Two-Pointer Climb**
Use two pointers starting from opposite ends of the array. The left pointer climbs up (moves right while the next element is strictly larger). The right pointer climbs up from the right (moves left while the next element is strictly larger). If both pointers meet at the same index that is not at either boundary, it is a valid mountain.

**Approach C: Single Pass State Machine**
Walk through the array maintaining a state: ASCENDING or DESCENDING. Start in ASCENDING state. When the first decrease is encountered, switch to DESCENDING. If another increase is found after that, or if equal elements appear, or if we never enter DESCENDING state, return false.

## 3. Optimal Approach Walkthrough (Step by Step)

We choose **Approach B: Two-Pointer Climb** for elegance and simplicity.

Step 1 -- Check the length. If arr.length < 3, return false immediately.

Step 2 -- Initialize two pointers: `left = 0` and `right = n - 1`.

Step 3 -- Climb from the left: while `left + 1 < n` and `arr[left] < arr[left + 1]`, increment `left`. After this loop, `left` points to the first local peak from the left side.

Step 4 -- Climb from the right: while `right - 1 >= 0` and `arr[right] < arr[right - 1]`, decrement `right`. After this loop, `right` points to the first local peak from the right side.

Step 5 -- Validate: the array is a valid mountain if and only if:
  - `left == right` (both climbs reached the same peak)
  - `left != 0` (there is a real ascending portion)
  - `right != n - 1` (there is a real descending portion)

If all three conditions hold, return true. Otherwise, return false.

## 4. Implementation Notes

- The two-pointer approach is very concise -- the entire logic is about 10 lines of code.
- Be careful with the while-loop conditions: `left + 1 < n` prevents out-of-bounds access when comparing `arr[left]` with `arr[left + 1]`.
- Similarly, `right - 1 >= 0` prevents negative indexing on the right side.
- The condition `left != 0 && right != n - 1` ensures that the peak is not at a boundary. If the entire array is non-decreasing, `left` reaches `n - 1` and `right` stays at `n - 1`, so `left == right == n - 1`, and the check `right != n - 1` fails correctly.
- No extra space is needed beyond two integer variables.
- For Approach A (find max), beware of duplicate maximum values -- finding the max index alone may be ambiguous if the max appears more than once.

## 5. Complexity Analysis

**Time Complexity:** O(n) where n is the length of the array. The left pointer traverses some portion of the array, and the right pointer traverses the remainder. Combined, each element is visited at most twice (once by each pointer), so total work is O(n).

**Space Complexity:** O(1). Only two integer pointers and a few comparisons. No extra data structures needed.

All three approaches are O(n) time and O(1) space. The two-pointer approach is preferred for its clarity and minimal branching.

## 6. Edge Cases & Pitfalls

- **Array too short:** Length 0, 1, or 2 cannot form a mountain. Return false immediately.
- **Flat array (all equal):** [3, 3, 3]. Neither pointer moves. left = 0, right = n-1. Since left != right, and left = 0 means no ascending part, return false.
- **Strictly increasing only:** [1, 2, 3, 4]. Left pointer reaches index 3 (end), right stays at 3. left == right but right == n-1, so return false. Correct -- no descending part.
- **Strictly decreasing only:** [4, 3, 2, 1]. Left pointer stays at 0, right reaches 0. left == right == 0, but left == 0, so return false. Correct -- no ascending part.
- **Plateau at peak:** [1, 3, 3, 2]. Left stops at index 1 (arr[1]=3 is not < arr[2]=3). Right stops at index 2 (arr[2]=3 is not < arr[1]=3). left != right, so return false. Correct -- equal adjacent elements violate strict mountain.
- **V-shape (valley):** [3, 1, 3]. Left stays at 0 (arr[0]=3 > arr[1]=1). Right stays at 2 (arr[2]=3 > arr[1]=1). left=0, right=2, left != right, return false. Correct.
- **Perfect mountain:** [0, 3, 2, 1]. Left reaches 1, right reaches 1. left == right == 1, left != 0, right != 3. Return true.

## 7. Mnemonics / Memory Hooks

**"Two climbers meet at the peak"** -- Imagine one climber starting from the left base and another from the right base, both climbing uphill. If they meet at the same point and both actually climbed (neither started at the peak), it is a valid mountain.

**"Not at edges"** -- The peak must be strictly interior: not at index 0, not at index n-1. This eliminates monotonically increasing or decreasing arrays.

**"Strict means no flats"** -- Equal adjacent elements invalidate the mountain. The while-loops use strict less-than (<), so plateaus naturally cause the pointers to stop early, leading to a mismatch.

## 8. Related Problems & Patterns

- **LeetCode 852 - Peak Index in a Mountain Array:** Given a guaranteed mountain array, find the peak index. Uses binary search for O(log n). This problem asks whether the array IS a mountain, while 852 assumes it is.
- **LeetCode 162 - Find Peak Element:** Find any peak in an array (not necessarily a mountain). Binary search approach.
- **LeetCode 845 - Longest Mountain in Array:** Find the longest mountain subarray. Extends the mountain concept to subarrays and requires tracking multiple mountains.
- **LeetCode 1671 - Minimum Number of Removals to Make Mountain Array:** DP problem built on the mountain array concept, requiring LIS/LDS computation.
- **Pattern: "Two-Pointer Convergence"** -- Start pointers at both ends and move them toward each other based on local conditions. When they meet, check the meeting point for validity. This pattern appears in many array problems (container with most water, trapping rain water, etc.).
- **Pattern: "State Machine on Arrays"** -- Process an array element-by-element, maintaining a state (e.g., ASCENDING/DESCENDING). Transitions between states are triggered by comparing adjacent elements. Invalid transitions indicate the array does not match the desired pattern.
