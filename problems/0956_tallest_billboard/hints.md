# LeetCode 956: Tallest Billboard

[View Solution](./solution.cpp)

## Problem Understanding

You want to install a billboard with two steel supports of equal height. You have a collection of rods that can be welded together. Each rod can be placed on the left support, the right support, or not used at all. The two supports must have equal total height. You need to find the maximum possible height for both supports. If no equal-height partition is possible, return 0. This is fundamentally a variant of the subset-sum / partition problem: partition some subset of rods into two groups with equal sum, and maximize that sum. The constraint that `rods.length <= 20` and `sum(rods) <= 5000` gives us room for DP approaches that would be infeasible for larger inputs.

## Key Insights

1. **The "difference" DP formulation is the key breakthrough.** Instead of tracking the heights of both supports separately (which would require 2D state), define `dp[diff]` = the maximum height of the TALLER support when the difference between the two supports is `diff`. This collapses the state space dramatically. When `diff = 0`, the two supports are equal, so `dp[0]` is our answer.

2. **Each rod has three choices.** For every rod of length `r`, you can: (a) skip it entirely, (b) add it to the taller/left support, or (c) add it to the shorter/right support. These three transitions update the DP state.

3. **The difference-based transitions are elegant.** If the current difference is `d` and we add rod `r`:
   - Skip: `dp[d]` stays the same.
   - Add to taller side: difference becomes `d + r`, max height becomes `dp[d] + r`.
   - Add to shorter side: difference becomes `|d - r|`, max height becomes `max(dp[d], dp[d] + (r - d))` depending on whether `r > d` or not.

4. **The maximum possible difference is `sum(rods)/2`.** Since both supports pull from the same pool, the difference between them cannot exceed half the total sum. This bounds our DP table size.

5. **Meet-in-the-middle is an alternative for tight constraints.** Split rods into two halves, enumerate all possible (sum_left, sum_right) pairs for each half, then combine. This gives O(3^(n/2)) which is feasible for n=20.

## Approach Categories

| Approach | Strategy | Time | Space |
|----------|----------|------|-------|
| DP on Difference | dp[diff] = max taller height for given diff | O(n * S) | O(S) |
| DP with HashMap | Use map instead of array for sparse differences | O(n * 3^?) | O(states) |
| Meet in the Middle | Split rods, enumerate halves, combine | O(3^(n/2)) | O(3^(n/2)) |
| Knapsack Variant | 2D DP on (rod_index, difference) | O(n * S) | O(n * S) |

## Step-by-Step Approach (DP on Difference)

1. **Initialize the DP map.** Use `unordered_map<int, int> dp` where `dp[diff] = max height of the taller support`. Start with `dp[0] = 0` (both supports empty, difference is 0, max height is 0). All other states are initially undefined (use -infinity or absence from the map).

2. **For each rod `r` in the rods array:**
   - Create a copy of the current DP state (`new_dp = dp`).
   - For each existing state `(diff, tallHeight)` in the OLD dp:
     - **Add to taller side:** new difference = `diff + r`, new height = `tallHeight + r`. Update `new_dp[diff + r] = max(new_dp[diff + r], tallHeight + r)`.
     - **Add to shorter side:** new difference = `abs(diff - r)`, new height = `max(tallHeight, tallHeight - diff + r)`. The logic: if `r <= diff`, shorter side grows but doesn't overtake, so taller stays at `tallHeight`, diff becomes `diff - r`. If `r > diff`, shorter side overtakes and becomes the new taller, height = `tallHeight + (r - diff)`, diff becomes `r - diff`.
   - Set `dp = new_dp`.

3. **Return `dp[0]`.** This is the maximum height when both supports are equal (difference = 0).

## Complexity Analysis

**Time Complexity: O(n * S)** where S = sum of all rods
- For each of the n rods, we iterate through all existing states in the DP.
- The number of distinct difference values is at most S (from 0 to sum).
- Each rod processes each state in O(1).
- Total: O(n * S) where S <= 5000.

**Space Complexity: O(S)**
- The DP map stores at most S+1 entries (differences from 0 to S).
- We use an additional copy for the transition step.
- Total: O(S).

## Common Mistakes

1. **Using a 2D DP on (left_height, right_height) directly.** This leads to O(S^2) states which is too large. The difference formulation reduces this to O(S) states.

2. **Incorrect transition when adding a rod to the shorter side.** When `r > diff`, the shorter side overtakes the taller side. The new taller height is NOT `tallHeight + r`; it is `tallHeight + (r - diff)` because only the excess beyond the gap contributes to increasing the maximum. Many implementations get this wrong.

3. **Modifying the DP map while iterating over it.** The three choices for each rod must be based on the state BEFORE processing this rod. If you update in-place without a copy, you get cascading updates where a single rod is effectively used multiple times.

4. **Forgetting that `dp[0] = 0` is the base case, not `dp[0] = -infinity`.** The initial state represents two empty supports with difference 0 and height 0.

5. **Not handling the "skip" transition.** The skip is implicitly handled by copying the old DP into new_dp before processing transitions. If you don't copy first, you lose states where the current rod is not used.

## Related Patterns

This problem belongs to the **Subset Sum / Equal Partition DP** pattern with a twist on state representation. Related problems include:
- **LeetCode 416: Partition Equal Subset Sum** -- can we split into two equal-sum subsets? (boolean version of this problem).
- **LeetCode 494: Target Sum** -- assign + or - to each number to reach a target, similar three-choice DP.
- **LeetCode 805: Split Array With Same Average** -- another partition problem with meet-in-the-middle.
- **LeetCode 1049: Last Stone Weight II** -- minimize the remaining weight, equivalent to minimizing the partition difference.
- The "DP on difference" technique is reusable whenever you need to track two quantities whose relationship matters more than their absolute values.

## Code Template

```
class Solution {
public:
    int tallestBillboard(vector<int>& rods) {
        // dp[diff] = max height of taller support when difference = diff
        unordered_map<int, int> dp;
        dp[0] = 0;

        for (int r : rods) {
            // Must copy to avoid using same rod twice
            unordered_map<int, int> newDp = dp;

            for (auto& [diff, tall] : dp) {
                // Option 1: Add rod to taller support
                int newDiff1 = diff + r;
                newDp[newDiff1] = max(newDp[newDiff1], tall + r);

                // Option 2: Add rod to shorter support
                int newDiff2 = abs(diff - r);
                int newTall2 = max(tall, tall - diff + r);
                newDp[newDiff2] = max(newDp[newDiff2], newTall2);
            }

            dp = move(newDp);
        }

        return dp[0];
    }
};
```
