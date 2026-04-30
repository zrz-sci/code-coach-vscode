[file://solution.cpp](solution.cpp)

# LeetCode 936 - Stamping The Sequence

## 1. Problem Understanding (Restate & Clarify)

We have a `stamp` string and a `target` string. We start with a string `s` of the same length as `target` filled entirely with '?' characters. In each turn, we can choose a starting index and "stamp" -- that is, we overwrite a contiguous window of `s` (of length `stamp.length`) with the characters of `stamp`. Later stamps can overwrite earlier ones. We need to find a sequence of stamping indices such that after all stamps are applied, `s` equals `target`. We must use at most `10 * target.length` turns. If impossible, return an empty array.

Key observations:
- Later stamps overwrite earlier ones, so the ORDER of stamping matters.
- The last stamp applied must match the corresponding portion of `target` exactly.
- Earlier stamps may be partially or fully overwritten by later stamps.
- The problem guarantees stamp.length <= target.length <= 1000.

## 2. Approach Brainstorming (At Least 2 Approaches)

**Approach A: Forward Simulation (Brute Force)**
Try all possible orderings of stamp placements and check if the final result matches target. This is combinatorially explosive and completely infeasible.

**Approach B: Reverse Thinking (Greedy, work backwards)**
Instead of building target from '?', think in reverse: start with `target` and try to "undo" stamps by replacing matched windows with '?'. If we can reduce the entire string to all '?', then the reverse of our undo sequence is a valid stamping order. At each step, find a position where the stamp matches (treating '?' as wildcards that match anything), replace those characters with '?', and record the index. Repeat until the entire string is '?'.

**Approach C: BFS/Graph Search**
Model each state of the string as a node and transitions as stamp applications. This quickly becomes infeasible due to the exponential state space.

## 3. Optimal Approach Walkthrough (Step by Step)

We choose **Approach B: Reverse Greedy** as the optimal solution.

Step 1 -- Start with the full `target` string. We will iteratively "unstamp" it.

Step 2 -- Define a matching function: at position `i`, check if stamp can match target[i..i+len-1] where '?' characters in target are treated as wildcards. Crucially, at least one non-'?' character must match (otherwise we are stamping an already-erased region, which is wasteful and leads to infinite loops).

Step 3 -- Scan through all possible positions 0 to target.length - stamp.length. For each position where the stamp matches (with at least one concrete character match), replace those positions in target with '?', and record this index.

Step 4 -- Repeat Step 3 until either:
  (a) The entire target becomes all '?', meaning success. OR
  (b) A full pass through all positions produces no matches, meaning it is impossible.

Step 5 -- The recorded indices, reversed, give the stamping order (since we undid stamps in reverse chronological order).

Step 6 -- Verify the total number of stamps does not exceed 10 * target.length.

## 4. Implementation Notes

- Use a character array (or modify string in place) for target to allow replacing with '?'.
- Track a counter `totalReplaced` of how many non-'?' characters have been turned to '?'. When totalReplaced equals target.length, we are done.
- In the matching function, iterate through each position of the stamp. If target[i+k] is '?' skip it; if it matches stamp[k] mark it; if it does not match, the stamp does not fit here. Ensure at least one non-'?' was matched.
- The result vector is built in reverse order of operations, so reverse it before returning.
- Each pass can stamp multiple positions. Continue passes until no more stamping is possible or all characters are erased.

## 5. Complexity Analysis

**Time Complexity:** O(n * m * (n/m)) where n = target.length and m = stamp.length. In the worst case, each pass through the string is O(n * m) and we might need O(n/m) passes since each pass must turn at least one character to '?'. More precisely, the total work is bounded by O(n * (n - m + 1)) since each character can only be "erased" once and contributes at most O(m) work. With n, m <= 1000, this is at most about O(n^2) = O(10^6), well within limits.

**Space Complexity:** O(n) for the modified target array and the result vector storing stamping indices.

## 6. Edge Cases & Pitfalls

- **stamp equals target:** One stamp at index 0 suffices. The algorithm should find this in one pass.
- **Impossible case:** If the first or last characters of target cannot be produced by the stamp, it may be impossible. For example, stamp = "abc", target = "xbc" -- the 'x' can never appear. Return empty array.
- **Overlapping stamps:** Stamps may overlap. The reverse approach handles this naturally because '?' acts as a wildcard.
- **Single character stamp:** stamp = "a", target = "aaaa". Each position is stamped independently. The reverse approach erases one character per stamp operation.
- **Must match at least one concrete character:** When checking a stamp position, if ALL characters in the window are already '?', skip it. Otherwise you would loop infinitely recording useless stamps.
- **Result length limit:** The problem guarantees at most 10 * target.length stamps. The greedy reverse approach naturally stays within this bound.
- **Repeated passes needed:** A single left-to-right scan may not find all valid positions because erasing characters in one spot may enable a match at a previously invalid spot. Keep looping until no progress is made.

## 7. Mnemonics / Memory Hooks

**"Peel the onion backwards"** -- Think of each stamp as a layer. The last stamp is the outermost layer (most visible). By peeling layers from the outside in (working backwards), you can identify each stamp's placement by what is still visible.

**"Wildcard erasure"** -- The '?' character is the key insight. In the reverse direction, already-erased characters are wildcards that match anything, letting us find stamps that were partially overwritten.

**"Greedy unstamp until all gone"** -- The algorithm is simple: keep finding places the stamp could have been the last one applied, erase them, repeat. If you get stuck, it is impossible.

## 8. Related Problems & Patterns

- **LeetCode 1048 - Longest String Chain:** Greedy/reverse thinking pattern where building forward is hard but working backwards simplifies the problem.
- **LeetCode 316 - Remove Duplicate Letters (Greedy + Stack):** Similar greedy approach where you process from a desired end state backward.
- **LeetCode 1209 - Remove All Adjacent Duplicates in String II:** Stack-based greedy removal pattern with repeated passes.
- **Pattern: "Reverse Simulation"** -- When forward simulation is complex due to overwriting/ordering constraints, try reversing the process. Undoing operations is often simpler because you can observe the final state and work backwards.
- **Pattern: "Greedy with Progress Guarantee"** -- Each iteration must make measurable progress (here, at least one character erased). If a full pass makes no progress, stop. This guarantees termination and bounds the total work.
