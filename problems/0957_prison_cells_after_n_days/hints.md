# LeetCode 957: Prison Cells After N Days

[View Solution](./solution.cpp)

## Problem Understanding

There are 8 prison cells in a row. Each day, every cell changes according to a rule: a cell becomes occupied (1) if both its neighbors are the same (both occupied or both vacant), and becomes vacant (0) otherwise. The first and last cells have only one neighbor each, so they always become 0 after day 1 (they can never have "two adjacent neighbors that are both the same"). Given an initial state and a number `n` (up to 10^9), return the state after `n` days. The key challenge is that `n` can be extremely large (up to a billion), so simulating one day at a time is not feasible. You must find a shortcut.

## Key Insights

1. **The state space is tiny and must cycle.** With 8 cells where cells 0 and 7 are always 0 after day 1, only cells 1-6 matter. That gives at most 2^6 = 64 possible states. By the pigeonhole principle, the states MUST repeat within at most 64 steps, forming a cycle.

2. **Cycle detection eliminates the need to simulate all n days.** Once you detect that a state has repeated, you know the cycle length. You can then compute `n % cycle_length` to find the equivalent small number of days to simulate.

3. **The transformation rule is essentially XNOR on neighbors.** Cell `i` becomes 1 if `cells[i-1] == cells[i+1]`, which is the XNOR operation. This can be computed with bit manipulation for efficiency (though with only 8 cells, simple array operations are fine).

4. **Cells 0 and 7 always become 0 after the first step.** Since cell 0 has no left neighbor and cell 7 has no right neighbor, they cannot satisfy the "both neighbors same" condition. After day 1, they are permanently 0.

5. **The cycle length in practice is always 14 or a divisor of 14.** While you should write general cycle-detection code, it is an empirical fact (provable from the XNOR dynamics on 6 bits) that the cycle length divides 14. Some solutions hardcode this, but it is better to detect it programmatically.

## Approach Categories

| Approach | Strategy | Time | Space |
|----------|----------|------|-------|
| Simulation + Cycle Detection (Map) | Simulate day by day, store states in map, detect repeat | O(min(n, 2^6)) | O(2^6) |
| Simulation + Cycle Detection (Bitmask) | Encode state as 8-bit int, use set for detection | O(min(n, 64)) | O(64) |
| Floyd's Cycle Detection | Two-pointer slow/fast approach | O(cycle_length) | O(1) |
| Hardcoded Cycle Length | Use n % 14 directly (empirical shortcut) | O(1) | O(1) |

## Step-by-Step Approach (Simulation + Cycle Detection with Map)

1. **Simulate the transformation.** Write a helper function `nextDay(cells)` that computes the next state. For each cell `i` from 1 to 6, set `next[i] = (cells[i-1] == cells[i+1]) ? 1 : 0`. Set `next[0] = 0` and `next[7] = 0`.

2. **Track seen states.** Use a `map<vector<int>, int>` (or convert the state to a string/bitmask as a key) to store each state and the day number when it was first seen.

3. **Simulate day by day.** For each day from 1 to n:
   - Compute the next state.
   - Check if this state has been seen before.
   - If YES: The cycle length = `current_day - previously_seen_day`. Compute the remaining days = `(n - current_day) % cycle_length`. Simulate `remaining` more days from the current state and return the result.
   - If NO: Store the state with the current day number. Continue.

4. **If no cycle is detected within n steps** (only possible for very small n), return the current state directly.

## Complexity Analysis

**Time Complexity: O(min(n, 2^6)) = O(min(n, 64))**
- The state space has at most 2^6 = 64 possible states (cells 0 and 7 are fixed at 0 after day 1).
- Cycle detection must find a repeat within 64 steps.
- After detecting the cycle, we simulate at most `cycle_length - 1` additional steps (at most 63).
- Total: O(64) = O(1) effectively, regardless of how large n is.

**Space Complexity: O(2^6) = O(1)**
- We store at most 64 states in the map before a cycle is found.
- Each state is a vector of 8 integers (or a single byte if using bitmask).
- Constant space since the number of cells is fixed at 8.

## Common Mistakes

1. **Trying to simulate all n days.** With n up to 10^9, a naive day-by-day simulation times out. You MUST detect the cycle.

2. **Off-by-one errors in cycle calculation.** The most common bug is computing the remaining days incorrectly. If the cycle is detected on day `d` and the same state was first seen on day `d0`, the cycle length is `d - d0`. The remaining days from day `d` is `(n - d) % (d - d0)`, and you simulate that many more steps from the current state.

3. **Forgetting that cells 0 and 7 become 0 after day 1.** Some implementations compute the next state correctly for cells 1-6 but forget to explicitly set cells 0 and 7 to 0, relying on the initial state which may have them as 1.

4. **Using the wrong map key.** If you use `vector<int>` as a map key, use `map` not `unordered_map` (or provide a hash function). Alternatively, convert the state to a string or bitmask integer for hashing.

5. **Hardcoding cycle length = 14 without verification.** While the cycle length empirically divides 14, the starting point of the cycle varies based on the initial state. If you hardcode `n % 14`, you might get the phase wrong if the cycle doesn't start at day 0.

## Related Patterns

This problem belongs to the **Simulation with Cycle Detection** pattern. Related problems include:
- **LeetCode 202: Happy Number** -- detect cycles in a number transformation using Floyd's algorithm or a hash set.
- **LeetCode 457: Circular Array Loop** -- cycle detection in an array with modular movement.
- **LeetCode 289: Game of Life** -- cellular automaton state update (similar neighbor-based rules, but on a 2D grid without cycle detection).
- **LeetCode 1041: Robot Bounded In Circle** -- detecting periodic behavior in a simulation.
- The general technique of "state space is bounded -> pigeonhole principle guarantees cycle -> use modulo" is fundamental to many simulation problems with large iteration counts.

## Code Template

```
class Solution {
public:
    vector<int> prisonAfterNDays(vector<int>& cells, int n) {
        // Helper: compute next day's state
        auto nextDay = [](vector<int>& c) -> vector<int> {
            vector<int> next(8, 0);
            for (int i = 1; i < 7; i++) {
                next[i] = (c[i - 1] == c[i + 1]) ? 1 : 0;
            }
            return next;
        };

        // Cycle detection using a map
        map<vector<int>, int> seen;
        vector<int> cur = cells;

        for (int day = 0; day < n; day++) {
            if (seen.count(cur)) {
                int cycleLen = day - seen[cur];
                int remaining = (n - day) % cycleLen;
                for (int i = 0; i < remaining; i++) {
                    cur = nextDay(cur);
                }
                return cur;
            }
            seen[cur] = day;
            cur = nextDay(cur);
        }

        return cur;
    }
};
```
