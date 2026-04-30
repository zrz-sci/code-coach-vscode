[file://solution.cpp](solution.cpp)

# 978. Longest Turbulent Subarray

## 1. Problem Summary

Given an integer array `arr`, find the length of the longest turbulent subarray. A subarray is turbulent if the comparison signs alternate between adjacent pairs: the differences between consecutive elements must alternate between positive and negative (strictly -- equalities break the pattern). For example, `[9,4,2,10,7,8]` has `9>4`, `4>2` (same sign, not turbulent) but `[4,2,10,7,8]` has `2<10`, `10>7`, `7<8` (alternating, turbulent of length 5). The answer is the maximum length among all turbulent subarrays.

## 2. Key Insights / "Aha!" Moments

- **This is a variant of the "longest alternating subsequence" but for contiguous subarrays.** The key constraint is contiguity -- you cannot skip elements. You need the longest window where every adjacent pair alternates in comparison direction.

- **Equal adjacent elements reset everything.** If `arr[k] == arr[k+1]`, neither `>` nor `<` holds, so the turbulent subarray cannot extend through this pair. Any window containing an equality must be split.

- **Track two running lengths.** At each position, maintain:
  - `inc`: length of the current turbulent subarray ending with an increase (`arr[i-1] < arr[i]`)
  - `dec`: length of the current turbulent subarray ending with a decrease (`arr[i-1] > arr[i]`)
  
  When `arr[i] > arr[i-1]`, the current position extends a subarray that previously ended with a decrease, so `inc = dec + 1` and `dec` resets to 1. Symmetrically for `arr[i] < arr[i-1]`. If equal, both reset to 1.

- **No need for explicit window management.** Unlike many sliding window problems, you do not need to maintain left and right pointers. The two-variable DP approach implicitly tracks the window boundaries through the `inc`/`dec` counters.

## 3. Approach Categories

| Approach | Time | Space | Notes |
|----------|------|-------|-------|
| Brute force: check all subarrays | O(n^2) | O(1) | For each starting index, extend right while turbulent. TLE for n=40000. |
| Sliding window with left/right pointers | O(n) | O(1) | Maintain a window [left, right]. Extend right when alternation continues; move left to right-1 when it breaks. |
| DP with inc/dec counters | O(n) | O(1) | Track two running lengths. Cleanest and most elegant approach. No explicit pointer management. |

## 4. Step-by-Step Solution (DP with inc/dec)

1. **Handle edge case.** If `arr` has length 0, return 0. If length 1, return 1.

2. **Initialize variables:**
   - `inc = 1` -- length of turbulent subarray ending with `arr[i-1] < arr[i]`
   - `dec = 1` -- length of turbulent subarray ending with `arr[i-1] > arr[i]`
   - `result = 1` -- global maximum

3. **Iterate from index 1 to n-1:**
   - If `arr[i] > arr[i-1]`:
     - This is an "increase". It can extend a subarray that previously ended with a decrease.
     - Set `inc = dec + 1`
     - Set `dec = 1` (a new subarray ending with decrease would start fresh here)
   - Else if `arr[i] < arr[i-1]`:
     - This is a "decrease". It can extend a subarray that previously ended with an increase.
     - Set `dec = inc + 1`
     - Set `inc = 1`
   - Else (`arr[i] == arr[i-1]`):
     - Equality breaks any turbulence. Reset both: `inc = 1`, `dec = 1`.
   - Update `result = max(result, max(inc, dec))`.

4. **Return `result`.**

## 5. Complexity Analysis

- **Time Complexity:** O(n) -- A single pass through the array. Each element is processed exactly once with O(1) work.

- **Space Complexity:** O(1) -- Only a constant number of variables (`inc`, `dec`, `result`) regardless of input size.

This is optimal because you must examine every element at least once to determine the answer (any element could be part of the longest turbulent subarray).

## 6. Common Pitfalls

- **Forgetting the equality case.** When `arr[i] == arr[i-1]`, both `inc` and `dec` must reset to 1. Forgetting this causes the algorithm to count flat segments as valid turbulence, producing an inflated answer.

- **Swapping the transition logic.** When `arr[i] > arr[i-1]` (increase), the new `inc` should extend `dec` (not `inc`). This is because turbulence requires alternation: an increase must follow a decrease and vice versa. Getting this backwards produces wrong results.

- **Not initializing result to 1.** A single element is trivially a turbulent subarray of length 1. If the array has no adjacent pair satisfying turbulence (e.g., all elements are equal), the answer is 1, not 0.

- **Off-by-one: returning `result - 1`.** The counters `inc` and `dec` track the number of elements, not the number of comparisons. A turbulent subarray of length 5 has 4 alternating comparisons. The problem asks for the number of elements (length), which is what we track.

- **Updating result before the transition.** Make sure you update `result` after computing the new `inc` and `dec`, not before. Otherwise you miss the current step's contribution.

## 7. Pattern Recognition

- **Alternating State DP:** Track two counters representing "ending with state A" and "ending with state B", where each state transitions from the other. This pattern appears in many problems with alternating or toggling conditions (buy/sell stocks, alternating subsequences, etc.).

- **Kadane's Algorithm Variant:** Just as Kadane's algorithm tracks a running maximum subarray sum, this problem tracks a running maximum turbulent length. The "reset" condition (equality) is analogous to the "restart" when the running sum drops below zero.

- **Sliding Window (Implicit):** The `inc`/`dec` counters implicitly define a window. When they reset, the window's left endpoint jumps forward. This is conceptually a sliding window even though no explicit left pointer is maintained.

## 8. Related Problems

- **LeetCode 376 - Wiggle Subsequence:** Find the longest subsequence (not subarray) where differences alternate in sign. The subsequence version allows skipping elements, making it a different problem, but the alternation concept is identical.

- **LeetCode 53 - Maximum Subarray:** Kadane's algorithm for maximum sum subarray. The structural pattern of maintaining a running "best ending here" and a global maximum is the same.

- **LeetCode 674 - Longest Continuous Increasing Subsequence:** Track the longest contiguous increasing run. Simpler than turbulence (only one direction to track), but the same "extend or reset" logic applies.

- **LeetCode 845 - Longest Mountain in Array:** Find the longest subarray that first increases then decreases. Another contiguous subarray problem with a state-transition structure (going up vs. going down).
