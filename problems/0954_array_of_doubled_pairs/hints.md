# LeetCode 954: Array of Doubled Pairs

[View Solution](./solution.cpp)

## Problem Understanding

You are given an integer array `arr` of even length. The task is to determine whether it is possible to reorder the array such that for every index `i` in the range `[0, len(arr)/2)`, the element at position `2*i+1` equals twice the element at position `2*i`. In simpler terms, you need to pair up every element with its double. Each element must be used exactly once, and every element must successfully find a partner (either it is someone's double, or someone is its double). The array can contain negative numbers, which adds a twist because the "double" relationship reverses direction for negatives (e.g., -2 paired with -4). Zero is also a special case since 0 * 2 = 0, meaning zeros must pair with other zeros.

## Key Insights

1. **Sorting by absolute value is the critical enabler.** When you process elements from the smallest absolute value to the largest, you guarantee that when you encounter a number `x`, its partner `2x` has not yet been consumed by a different pairing. This greedy ordering eliminates ambiguity about which direction to match.

2. **A frequency map tracks available elements.** By counting occurrences of each value in a hash map, you can efficiently check whether the required double exists and has remaining count. This avoids expensive linear scans.

3. **Negative numbers require no special branching.** If you sort by absolute value, a negative number like -3 naturally looks for -6 (its double), and this works seamlessly because -3 * 2 = -6. The absolute value sorting ensures -3 is processed before -6.

4. **Zero must pair with zero.** Since 0 * 2 = 0, zeros can only match with other zeros. If the count of zeros is odd, the answer is immediately false.

5. **Greedy consumption is sufficient.** Once sorted by absolute value, processing each element greedily (consume it and its double) is provably optimal. There is no need for backtracking because the smallest-first ordering prevents conflicts.

## Approach Categories

| Approach | Strategy | Time | Space |
|----------|----------|------|-------|
| Sort + Greedy + Hash Map | Sort by abs value, greedily match x with 2x | O(n log n) | O(n) |
| Multiset with Greedy | Use a multiset, extract smallest abs, erase its double | O(n log n) | O(n) |
| Counting Sort (limited range) | Bucket count, iterate from 0 outward for positives/negatives | O(n + R) | O(R) |

## Step-by-Step Approach (Sort + Greedy + Hash Map)

1. **Build a frequency map** by iterating through the array and counting occurrences of each value using an `unordered_map<int, int>`.

2. **Sort the array by absolute value.** Use `sort` with a custom comparator `abs(a) < abs(b)`. This ensures that for every pair (x, 2x), `x` is visited before `2x`.

3. **Iterate through the sorted array.** For each element `x`:
   - If `count[x] == 0`, this element has already been consumed as someone else's double. Skip it.
   - If `count[2*x] == 0`, there is no available double for `x`. Return `false`.
   - Decrement both `count[x]` and `count[2*x]` by 1.

4. **If the loop completes without returning false**, all elements have been successfully paired. Return `true`.

## Complexity Analysis

**Time Complexity: O(n log n)**
- Building the frequency map: O(n).
- Sorting the array by absolute value: O(n log n), which dominates.
- Single pass through sorted array for greedy matching: O(n).
- Total: O(n log n).

**Space Complexity: O(n)**
- The frequency map stores at most n entries (one per unique value).
- Sorting may use O(log n) stack space depending on implementation.
- Total auxiliary space: O(n).

## Common Mistakes

1. **Sorting by actual value instead of absolute value.** If you sort normally, negative numbers like -4 appear before -2, and you would try to match -4 with -8 instead of matching -2 with -4. Absolute value sorting is essential.

2. **Not handling zero correctly.** Forgetting that zeros must pair with zeros, and that an odd count of zeros makes the answer false. Some implementations skip zero or treat it as a special edge case incorrectly.

3. **Modifying the map while iterating in wrong order.** If you iterate through map keys instead of the sorted array, the order is unpredictable (hash maps are unordered). Always iterate through the sorted array to maintain the greedy invariant.

4. **Double-counting elements.** Failing to check `count[x] > 0` before processing, which can lead to negative counts and incorrect results when an element has already been consumed.

5. **Integer overflow with large doubles.** For values near the constraint boundary (10^5), doubling gives 2*10^5, which fits in int. But if constraints were larger, you would need to check bounds before computing 2*x.

## Related Patterns

This problem belongs to the **Greedy Matching with Frequency** pattern. Similar problems include:
- **LeetCode 2007: Find Original Array From Doubled Array** -- nearly identical logic but returns the original array instead of a boolean.
- **LeetCode 1296: Divide Array in Sets of K Consecutive Numbers** -- same greedy + frequency approach but matching consecutive values instead of doubles.
- **LeetCode 846: Hand of Straights** -- grouping elements into consecutive sequences using sorted greedy with a frequency map.
- The core technique of "sort by some criterion, then greedily consume with a counter" appears frequently in array partitioning and matching problems.

## Code Template

```
class Solution {
public:
    bool canReorderDoubled(vector<int>& arr) {
        // Step 1: Build frequency map
        unordered_map<int, int> count;
        for (int x : arr) {
            count[x]++;
        }

        // Step 2: Sort by absolute value
        sort(arr.begin(), arr.end(), [](int a, int b) {
            return abs(a) < abs(b);
        });

        // Step 3: Greedy matching
        for (int x : arr) {
            if (count[x] == 0) continue;  // already consumed
            if (count[2 * x] == 0) return false;  // no partner
            count[x]--;
            count[2 * x]--;
        }

        return true;
    }
};
```
