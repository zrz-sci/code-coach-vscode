# LeetCode 942 - DI String Match

[View Solution](./solution.cpp)

## 1. Problem Understanding (Read & Rephrase)

You are given a string `s` consisting only of the characters `'I'` (increase) and `'D'` (decrease). The string has length `n`, and you need to construct a permutation of integers `[0, 1, 2, ..., n]` (which has `n + 1` elements) such that:

- If `s[i] == 'I'`, then `perm[i] < perm[i + 1]` (the value increases at position `i`).
- If `s[i] == 'D'`, then `perm[i] > perm[i + 1]` (the value decreases at position `i`).

Any valid permutation that satisfies these constraints is acceptable. The key insight is that you must use each integer in `[0, n]` exactly once, and the relative ordering between consecutive elements must match the pattern given by `s`.

**Restatement:** Given a pattern of increases and decreases, assign the numbers `0` through `n` to positions `0` through `n` so that consecutive pairs respect the pattern.

## 2. Key Observations & Insights

1. **Greedy choice is always safe:** When you see an `'I'`, you want the current position to hold a small value so that the next position can hold something larger. Conversely, when you see a `'D'`, you want the current position to hold a large value so the next position can hold something smaller.

2. **Two-pointer range:** Maintain a range `[lo, hi]` where `lo` starts at `0` and `hi` starts at `n`. For each character in `s`, pick from the appropriate end of the range. This guarantees no collisions because each pick shrinks the available range by one.

3. **No conflicts possible:** Since `lo` and `hi` converge toward each other and you always pick from the boundary, every chosen number is unique and within `[0, n]`.

4. **After processing all characters of `s`, exactly one number remains** in the range `[lo, hi]` (where `lo == hi`). Assign this final number to the last position.

5. **Why greedy works formally:** Picking the smallest available for `'I'` ensures you leave as many large numbers available as possible, giving maximum flexibility for future decreases. Similarly, picking the largest available for `'D'` leaves small numbers for future increases.

## 3. Approach Categories

| Approach | Time | Space | Notes |
|---|---|---|---|
| Greedy two-pointer | O(n) | O(n) | Optimal, single pass, construct result directly |
| Backtracking / brute force | O(n! * n) | O(n) | Generate all permutations and check, way too slow |
| Stack-based simulation | O(n) | O(n) | Process groups of consecutive I/D, more complex |

**Recommended:** Greedy two-pointer -- simple, optimal, and easy to implement.

## 4. Step-by-Step Solution (Greedy Two-Pointer)

1. Initialize `lo = 0` and `hi = n` where `n = s.length()`.
2. Create a result array of size `n + 1`.
3. Iterate `i` from `0` to `n - 1`:
   - If `s[i] == 'I'`, set `result[i] = lo` and increment `lo`.
   - If `s[i] == 'D'`, set `result[i] = hi` and decrement `hi`.
4. After the loop, `lo == hi`. Set `result[n] = lo` (or `hi`, they are equal).
5. Return the result array.

**Why this works:** When the pattern says "increase," placing the current smallest guarantees the next number (whatever it is from the remaining pool) will be larger. When the pattern says "decrease," placing the current largest guarantees the next number will be smaller.

## 5. Complexity Analysis

**Time Complexity:** O(n) -- a single pass through the string of length `n`, with constant work per character.

**Space Complexity:** O(n) -- the output array of size `n + 1`. If we exclude the output from our space accounting, auxiliary space is O(1) since we only use two integer variables `lo` and `hi`.

## 6. Common Mistakes & Edge Cases

- **Off-by-one on the last element:** After processing all `n` characters of `s`, do not forget to assign `result[n]`. There are `n + 1` positions but only `n` characters, so the final position needs the remaining number.
- **Misunderstanding the index relationship:** `s[i]` describes the relationship between `perm[i]` and `perm[i+1]`, not between `perm[i-1]` and `perm[i]`.
- **Edge case -- single character:** `s = "I"` should give `[0, 1]` and `s = "D"` should give `[1, 0]`. The two-pointer approach handles this naturally.
- **Edge case -- all same characters:** `s = "IIII"` gives `[0, 1, 2, 3, 4]` and `s = "DDDD"` gives `[4, 3, 2, 1, 0]`. Both handled correctly by always picking from the same side.
- **Returning the wrong type:** The problem asks for a vector of integers, not a string.

## 7. Related LeetCode Problems

| Problem | Relationship |
|---|---|
| **484. Find Permutation** | Very similar -- reconstruct a permutation from D/I pattern (Medium difficulty, similar greedy approach) |
| **31. Next Permutation** | Permutation manipulation with ordering constraints |
| **946. Validate Stack Sequences** | Greedy simulation with two-pointer/stack approach |
| **1920. Build Array from Permutation** | Array construction from a permutation rule |
| **667. Beautiful Arrangement II** | Constructing an array with a specific number of distinct differences |

## 8. Interview Tips & Follow-ups

- **Start with examples:** Walk through `s = "IDID"` manually. Show the interviewer you understand what the problem asks before coding.
- **Explain the greedy intuition clearly:** "If I need the next element to be larger, I should use the smallest remaining number now. If I need it to be smaller, I should use the largest remaining now."
- **Prove correctness informally:** Each pick is from a boundary of the remaining range, guaranteeing the constraint is met and no duplicates occur.
- **Follow-up question -- multiple valid answers:** The interviewer might ask how many valid permutations exist. This is a harder combinatorial question. For a string with `k` consecutive I's, there are multiple ways to arrange that segment, so the total count is the product of factorials of consecutive same-character run lengths.
- **Follow-up question -- lexicographically smallest:** If asked for the lexicographically smallest valid permutation, the greedy two-pointer approach already gives it when you use `lo` for `'I'` and `hi` for `'D'`.
- **Optimization discussion:** This is already O(n) time and O(1) auxiliary space, so there is nothing to optimize further. Mention that this is the theoretical lower bound since you must read the entire input.
