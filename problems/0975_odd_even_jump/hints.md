[file://solution.cpp](solution.cpp)

# 975. Odd Even Jump

## 1. Problem Summary

Given an integer array `arr`, you start at some index and make a series of jumps. Odd-numbered jumps (1st, 3rd, 5th...) go to the index of the smallest value >= current value among later indices (ties broken by smallest index). Even-numbered jumps (2nd, 4th, 6th...) go to the index of the largest value <= current value among later indices (ties broken by smallest index). A starting index is "good" if you can eventually reach the last index. Count how many good starting indices exist. This is a classic problem combining ordered data structures with dynamic programming, frequently asked at Google.

## 2. Key Insights / "Aha!" Moments

- **Work backwards from the end.** The last index is always good. For every other index, whether it is good depends on where its odd jump lands and whether that landing index is good (after an even jump). This naturally suggests a right-to-left DP.

- **Two boolean DP arrays.** Define `odd[i]` = "can we reach the end starting from index i if our next jump is an odd-numbered jump?" and `even[i]` = "can we reach the end starting from index i if our next jump is an even-numbered jump?" Then:
  - `odd[i] = even[j]` where `j` is the odd-jump target from `i`
  - `even[i] = odd[k]` where `k` is the even-jump target from `i`
  - The answer counts all `i` where `odd[i]` is true (since we always start with jump 1, which is odd).

- **Use an ordered map/set to find jump targets efficiently.** As we scan from right to left, we maintain a sorted structure (like `std::map<int,int>`) mapping values to their indices. For an odd jump from value `arr[i]`, we need the smallest key >= `arr[i]`, which is `lower_bound(arr[i])`. For an even jump, we need the largest key <= `arr[i]`, which is `upper_bound(arr[i])` then stepping back one position (or using `lower_bound` and checking).

- **The map handles duplicate values correctly.** When two indices have the same value, the later insertion (which is actually the earlier index since we scan right-to-left) overwrites the previous one. Since we want the smallest index among ties, and we process right-to-left, the map naturally stores the leftmost (smallest) index for each value -- but wait, we actually want the smallest index to the RIGHT, so we process right-to-left and the first time we see a value, its index is largest. We must overwrite with each new index because it is smaller. This works since `map[arr[i]] = i` always updates to the current (leftward) index.

## 3. Approach Categories

| Approach | Time | Space | Notes |
|----------|------|-------|-------|
| Brute force: for each index, simulate jumps | O(n^2) | O(1) | For each starting index, scan forward to find the jump target at each step. TLE for n=20000. |
| Monotonic stack to precompute jump targets | O(n log n) | O(n) | Sort indices by value, then use a monotonic stack to find next greater/smaller element. Two passes (one for odd jumps, one for even). |
| Ordered map (TreeMap) + DP | O(n log n) | O(n) | Process right-to-left. Use a balanced BST (map) to find odd/even jump targets in O(log n). Compute DP values on the fly. Cleanest approach. |

## 4. Step-by-Step Solution (Ordered Map + DP)

1. **Initialize.** Create two boolean arrays `odd` and `even` of size `n`, all `false`. Set `odd[n-1] = even[n-1] = true` because the last index is always reachable from itself.

2. **Create an ordered map** `map<int, int> valToIdx` that maps array values to their rightmost-so-far index. Insert `arr[n-1] -> n-1`.

3. **Iterate from `i = n-2` down to `0`:**
   a. **Find odd jump target:** Call `valToIdx.lower_bound(arr[i])`. This returns an iterator to the smallest key >= `arr[i]`. If found, let `j = it->second`. Set `odd[i] = even[j]`.
   b. **Find even jump target:** Call `valToIdx.upper_bound(arr[i])`. If the result is not `valToIdx.begin()`, decrement the iterator to get the largest key <= `arr[i]`. Let `k = it->second`. Set `even[i] = odd[k]`.
   c. **Insert** `valToIdx[arr[i]] = i`. This overwrites any previous entry for the same value, ensuring we track the smallest (leftmost) index for ties.

4. **Count** the number of indices where `odd[i]` is `true`. Return the count.

## 5. Complexity Analysis

- **Time Complexity:** O(n log n) -- We iterate through the array once (O(n)) and at each step perform a `lower_bound` / `upper_bound` lookup on a balanced BST containing at most n elements (O(log n) each). Total: O(n log n).

- **Space Complexity:** O(n) -- Two boolean arrays of size n, plus the ordered map containing at most n entries. All O(n).

## 6. Common Pitfalls

- **Confusing odd/even jump directions.** Odd jumps seek the smallest value >= current (ceiling), while even jumps seek the largest value <= current (floor). Mixing these up produces wrong jump targets.

- **Forgetting to handle the "no valid jump" case.** If `lower_bound` returns `end()` (odd jump) or `upper_bound` returns `begin()` (even jump), there is no valid jump. The DP value stays `false`.

- **Wrong tie-breaking with duplicate values.** When multiple indices share the same value, the problem requires jumping to the smallest index. Since we process right-to-left and overwrite `valToIdx[arr[i]] = i`, the map always stores the smallest index seen so far for each value, which is correct.

- **Off-by-one with `upper_bound` for even jumps.** `upper_bound(x)` returns the first key strictly greater than `x`. To get the largest key <= `x`, you must decrement the iterator (after checking it is not `begin()`). Using `lower_bound` instead of `upper_bound` here will skip equal values.

- **Starting with an even jump.** The problem says jump 1 is always odd. The answer only counts indices where `odd[i]` is true, not `even[i]`.

## 7. Pattern Recognition

- **Reverse-Direction DP:** Processing the array from right to left so that future states are already computed when needed. This pattern appears whenever the problem asks "can index i reach the end?"

- **Ordered Map / TreeMap for Next Greater/Smaller Element:** Using a balanced BST (`std::map` in C++) with `lower_bound` / `upper_bound` to efficiently find the nearest value satisfying a constraint. This is an alternative to monotonic stacks when you need ceiling/floor queries on values (not just positions).

- **Two-State DP:** Maintaining two separate DP arrays (odd-jump-reachable and even-jump-reachable) because the transition from one state depends on the other. Similar to problems with alternating moves or bipartite state transitions.

## 8. Related Problems

- **LeetCode 1340 - Jump Game V:** Another jump problem where you can jump to indices based on value constraints, solved with DP. The jump rules differ but the overall DP-on-indices structure is similar.

- **LeetCode 1696 - Jump Game VI:** Find the maximum score path using jumps of bounded length. Uses a monotonic deque instead of a map, but the right-to-left DP structure is analogous.

- **LeetCode 496 - Next Greater Element I / LeetCode 503 - Next Greater Element II:** Classic monotonic stack problems for finding the next greater element. The odd jump target is a variant of "next greater or equal element to the right, but among values not positions."

- **LeetCode 456 - 132 Pattern:** Uses an ordered set or monotonic stack to track value relationships among array elements, sharing the BST-based search pattern.
