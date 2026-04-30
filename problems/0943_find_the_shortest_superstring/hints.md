# LeetCode 943 - Find the Shortest Superstring

[View Solution](./solution.cpp)

## 1. Problem Understanding (Read & Rephrase)

You are given an array of strings `words` where no string is a substring of another. You must find the shortest possible string that contains every string in `words` as a substring. If multiple shortest strings exist, return any one of them.

This is a classic optimization problem related to the **Shortest Superstring Problem**, which is NP-hard in the general case. However, the constraint `words.length <= 12` makes it tractable with bitmask dynamic programming (2^12 = 4096 states).

**Restatement:** Find an ordering (permutation) of the words and merge them by overlapping shared prefixes/suffixes to produce the shortest combined string. The key is maximizing the total overlap between consecutive words in the chosen ordering.

## 2. Key Observations & Insights

1. **Overlap precomputation:** For every pair `(i, j)`, precompute how many characters of the suffix of `words[i]` match the prefix of `words[j]`. This overlap tells you how many characters you can save when placing `j` immediately after `i`.

2. **TSP reduction:** This problem is equivalent to finding the shortest Hamiltonian path in a directed graph where the edge weight from node `i` to node `j` is `len(words[j]) - overlap(i, j)`. Minimizing the total path length is equivalent to maximizing total overlap.

3. **Bitmask DP for TSP:** With `n <= 12` words, we can use a bitmask of size `2^n` to represent which words have been included. The DP state is `(mask, last)` where `mask` is the set of included words and `last` is the index of the last word appended.

4. **Reconstruction:** To build the actual string, we need to track which word was placed before `last` in the optimal path. This requires a parent/predecessor table built during DP.

5. **No substring condition:** The problem guarantees no string is a substring of another. This simplifies things because every word contributes at least one unique character to the superstring.

## 3. Approach Categories

| Approach | Time | Space | Notes |
|---|---|---|---|
| Bitmask DP (TSP) | O(n^2 * 2^n) | O(n * 2^n) | Optimal for n <= 12. Standard approach. |
| Brute force permutation | O(n! * n * L) | O(n * L) | Try all orderings. Feasible for n <= 10 but slow for 12. |
| Greedy overlap merging | O(n^2 * L) | O(n * L) | Not guaranteed optimal. Produces a good approximation. |

**Recommended:** Bitmask DP -- it is the standard, correct solution for the given constraints.

## 4. Step-by-Step Solution (Bitmask DP)

**Phase 1: Precompute overlaps**
1. For every pair `(i, j)` where `i != j`, compute `overlap[i][j]` = the maximum length `k` such that the last `k` characters of `words[i]` equal the first `k` characters of `words[j]`.
2. To compute this efficiently, for each `k` from `min(len(words[i]), len(words[j]))` down to `0`, check if `words[i].substr(len(words[i]) - k)` equals `words[j].substr(0, k)`. Use the first match found.

**Phase 2: Bitmask DP**
1. Let `n = words.size()` and define `dp[mask][i]` = the maximum total overlap achievable using the set of words indicated by `mask`, where word `i` is the last word in the sequence.
2. Base case: For each word `i`, `dp[1 << i][i] = 0` (a single word has no overlap with predecessors).
3. Transition: For each state `(mask, last)`, try extending by adding word `j` not yet in `mask`:
   - `new_mask = mask | (1 << j)`
   - `dp[new_mask][j] = max(dp[new_mask][j], dp[mask][last] + overlap[last][j])`
4. Also maintain `parent[mask][j]` to record which `last` yielded the best transition, for reconstruction.

**Phase 3: Reconstruct the answer**
1. Among all `dp[(1 << n) - 1][i]` for `i` in `[0, n)`, find the `i` that maximizes total overlap.
2. Trace back through `parent` to recover the full ordering of words.
3. Build the superstring by appending only the non-overlapping suffix of each successive word.

## 5. Complexity Analysis

**Time Complexity:** O(n^2 * 2^n + n^2 * L)
- Overlap precomputation: O(n^2 * L) where L is the max word length (up to 20).
- DP transitions: O(n^2 * 2^n). For n = 12, this is 12^2 * 4096 = ~590,000 operations.
- Reconstruction: O(n) to trace back, plus O(n * L) to build the string.

**Space Complexity:** O(n * 2^n)
- The DP table and parent table each have `n * 2^n` entries. For n = 12, that is ~49,000 entries per table.

## 6. Common Mistakes & Edge Cases

- **Overlap direction matters:** `overlap[i][j]` (suffix of `i` matching prefix of `j`) is NOT the same as `overlap[j][i]`. The overlap matrix is generally asymmetric.
- **Forgetting reconstruction:** Many people can compute the maximum overlap but struggle to rebuild the actual string. Always maintain a parent table during DP.
- **Off-by-one in overlap computation:** When checking if `words[i]` ends with a prefix of `words[j]`, make sure to check all lengths from `min(len_i, len_j)` down to `0`, not just `len_j`.
- **Edge case -- single word:** If `words` has one element, return it directly.
- **Edge case -- no overlap at all:** If no words share any prefix/suffix overlap (e.g., `["abc", "def", "ghi"]`), the answer is just the concatenation of all words in any order.
- **Greedy is wrong:** A common mistake is to use a greedy approach (always merge the pair with maximum overlap). This does NOT guarantee the optimal solution. Example: `["ab", "bc", "ca"]` -- greedy might pick overlap `"ab"->"bc"` first, yielding `"abc" + "ca"` = `"abca"` (length 4), but the optimal is `"cabc"` (also length 4 in this case). For other inputs the greedy can be strictly worse.

## 7. Related LeetCode Problems

| Problem | Relationship |
|---|---|
| **1125. Smallest Sufficient Team** | Bitmask DP with similar state structure (set cover via bitmask) |
| **847. Shortest Path Visiting All Nodes** | BFS + bitmask on a graph, similar TSP-like structure |
| **1494. Parallel Courses II** | Bitmask DP with subset enumeration |
| **526. Beautiful Arrangement** | Bitmask DP for counting permutations with constraints |
| **691. Stickers to Spell Word** | Bitmask DP where each bit represents a needed character |

## 8. Interview Tips & Follow-ups

- **Clarify the constraint on `n`:** The first thing to notice is `n <= 12`, which screams bitmask DP. Mention TSP immediately -- interviewers love seeing that connection.
- **Start with the overlap precomputation:** This is a natural first step that shows good problem decomposition. Explain it clearly before diving into DP.
- **Draw the state transition diagram:** Show `(mask, last)` states and how adding a new word transitions between them. This helps the interviewer follow your logic.
- **Discuss the NP-hard nature:** Mention that the general Shortest Superstring problem is NP-hard, but the small `n` allows an exact exponential-time solution.
- **Follow-up -- approximation:** If `n` were much larger (say 1000), you would need a greedy approximation. The greedy "merge max overlap pair" gives a 4-approximation, and more sophisticated algorithms achieve 2.5-approximation ratios.
- **Follow-up -- just the length:** If only the length (not the actual string) is needed, the DP is simpler because you do not need reconstruction. This is a good simplification to mention.
- **Time management:** This is a Hard problem. In a 45-minute interview, spend 10 minutes on design (overlap + DP formulation), 20 minutes coding, and 10 minutes testing. The reconstruction part is the trickiest to code correctly.
