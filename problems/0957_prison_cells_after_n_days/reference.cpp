// LeetCode 957: Prison Cells After N Days
// Difficulty: Medium
// Tags: Bit Manipulation, Array, Hash Table, Math
// Link: https://leetcode.com/problems/prison-cells-after-n-days/
//
// Approach: Simulate day-by-day with cycle detection.
// The state space is at most 2^6 = 64 (cells 0 and 7 are always 0
// after day 1). Detect the cycle, then use modulo arithmetic.
//
// Time:  O(1) -- at most 64 steps to find cycle, plus <=63 to finish
// Space: O(1) -- at most 64 states stored

// ============================================================
// Solution 1: Cycle Detection with Map (Clean & General)
// ============================================================
// Simulate day by day. Before each transition, store the current
// state in a map keyed by the state itself and valued by the day
// number. When a state repeats, compute cycle length and use
// modulo to skip ahead.

class Solution {
public:
    vector<int> prisonAfterNDays(vector<int>& cells, int n) {
        // Helper: compute the next day's cell configuration
        // Rule: cell[i] = 1 iff cell[i-1] == cell[i+1] (XNOR)
        // Cells 0 and 7 always become 0 (no two neighbors)
        auto nextDay = [](vector<int>& c) -> vector<int> {
            vector<int> result(8, 0);
            for (int i = 1; i <= 6; i++) {
                result[i] = (c[i - 1] == c[i + 1]) ? 1 : 0;
            }
            // result[0] and result[7] are already 0
            return result;
        };

        // Map from state -> day number when first seen
        map<vector<int>, int> seen;
        vector<int> current = cells;

        for (int day = 0; day < n; day++) {
            // Check if we've seen this state before
            if (seen.find(current) != seen.end()) {
                // Cycle detected!
                int cycleStart = seen[current];
                int cycleLength = day - cycleStart;

                // How many more days do we need after this point?
                int remaining = (n - day) % cycleLength;

                // Simulate the remaining days
                for (int r = 0; r < remaining; r++) {
                    current = nextDay(current);
                }
                return current;
            }

            // Record this state and advance
            seen[current] = day;
            current = nextDay(current);
        }

        // If n is small enough that we never hit a cycle
        return current;
    }
};


// ============================================================
// Solution 2: Bitmask + Cycle Detection (Memory Efficient)
// ============================================================
// Encode the 8 cells as bits in a single byte. Use an unordered_map
// keyed by the bitmask for O(1) lookup. The transformation becomes
// a bitwise XNOR operation on adjacent bits.

class SolutionBitmask {
public:
    vector<int> prisonAfterNDays(vector<int>& cells, int n) {
        // Convert vector to bitmask (bit 0 = cells[0], ..., bit 7 = cells[7])
        auto toBitmask = [](vector<int>& c) -> int {
            int mask = 0;
            for (int i = 0; i < 8; i++) {
                if (c[i]) mask |= (1 << i);
            }
            return mask;
        };

        // Convert bitmask back to vector
        auto toVector = [](int mask) -> vector<int> {
            vector<int> c(8);
            for (int i = 0; i < 8; i++) {
                c[i] = (mask >> i) & 1;
            }
            return c;
        };

        // Compute next state from bitmask
        // cell[i] = 1 iff cell[i-1] == cell[i+1] = ~(cell[i-1] XOR cell[i+1])
        // Only bits 1-6 matter; bits 0 and 7 are always 0
        auto nextMask = [](int mask) -> int {
            int shifted_left = mask >> 1;   // cell[i-1] at position i
            int shifted_right = mask << 1;  // cell[i+1] at position i
            int xnor = ~(shifted_left ^ shifted_right);
            // Mask to keep only bits 1-6
            return xnor & 0x7E;  // 0b01111110
        };

        int state = toBitmask(cells);
        unordered_map<int, int> seen;

        for (int day = 0; day < n; day++) {
            if (seen.count(state)) {
                int cycleLen = day - seen[state];
                int remaining = (n - day) % cycleLen;
                for (int r = 0; r < remaining; r++) {
                    state = nextMask(state);
                }
                return toVector(state);
            }
            seen[state] = day;
            state = nextMask(state);
        }

        return toVector(state);
    }
};


// ============================================================
// Solution 3: Store States in Array (Cycle via Indexing)
// ============================================================
// Instead of a hash map, store all states in an array indexed by
// day number. When a cycle is detected, directly index into the
// array to find the final state without re-simulating.

class SolutionArrayStore {
public:
    vector<int> prisonAfterNDays(vector<int>& cells, int n) {
        auto nextDay = [](vector<int>& c) -> vector<int> {
            vector<int> res(8, 0);
            for (int i = 1; i <= 6; i++) {
                res[i] = (c[i - 1] == c[i + 1]) ? 1 : 0;
            }
            return res;
        };

        // Store all states encountered, indexed by day
        vector<vector<int>> history;
        map<vector<int>, int> seen;

        vector<int> current = cells;

        for (int day = 0; day <= n; day++) {
            if (day == n) return current;

            if (seen.count(current)) {
                // Cycle: from seen[current] to day-1
                int cycleStart = seen[current];
                int cycleLen = day - cycleStart;
                // The answer is at position cycleStart + (n - cycleStart) % cycleLen
                int targetDay = cycleStart + (n - cycleStart) % cycleLen;
                return history[targetDay];
            }

            seen[current] = day;
            history.push_back(current);
            current = nextDay(current);
        }

        return current;  // unreachable for valid input
    }
};


// ============================================================
// Walkthrough with Example: cells = [0,1,0,1,1,0,0,1], n = 7
// ============================================================
//
// Day 0 (initial): [0, 1, 0, 1, 1, 0, 0, 1]
//   seen = {[0,1,0,1,1,0,0,1]: 0}
//
// Compute next day:
//   cell[0] = 0 (always)
//   cell[1] = (cell[0]==cell[2]) = (0==0) = 1
//   cell[2] = (cell[1]==cell[3]) = (1==1) = 1
//   cell[3] = (cell[2]==cell[4]) = (0==1) = 0
//   cell[4] = (cell[3]==cell[5]) = (1==0) = 0
//   cell[5] = (cell[4]==cell[6]) = (1==0) = 0
//   cell[6] = (cell[5]==cell[7]) = (0==1) = 0
//   cell[7] = 0 (always)
//
// Day 1: [0, 1, 1, 0, 0, 0, 0, 0]
// Day 2: [0, 0, 0, 0, 1, 1, 1, 0]
// Day 3: [0, 1, 1, 0, 0, 1, 0, 0]
// Day 4: [0, 0, 0, 0, 0, 1, 0, 0]
// Day 5: [0, 1, 1, 1, 0, 1, 0, 0]
// Day 6: [0, 0, 1, 0, 1, 1, 0, 0]
// Day 7: [0, 0, 1, 1, 0, 0, 0, 0]  <-- Answer
//
// For n=7, no cycle is needed (7 < 64). Simulation completes directly.
//
//
// Example 2: cells = [1,0,0,1,0,0,1,0], n = 1000000000
//
// Day 0: [1, 0, 0, 1, 0, 0, 1, 0]
// Day 1: [0, 0, 0, 1, 0, 0, 0, 0]  (cells 0 and 7 become 0)
// ...
// Suppose cycle detected at day 15 matching day 1:
//   cycleLen = 15 - 1 = 14
//   remaining = (1000000000 - 15) % 14 = ...
//   Simulate 'remaining' more steps.
//
// Output: [0, 0, 1, 1, 1, 1, 1, 0]
