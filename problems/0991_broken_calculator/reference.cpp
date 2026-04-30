/*
 * LeetCode 991: Broken Calculator
 *
 * Approach 1: Greedy reverse (work backwards from target)
 * - Instead of going startValue -> target (exponential branching),
 *   work backwards: target -> startValue.
 * - Reverse operations: divide by 2 (reverse of *2), add 1 (reverse of -1).
 * - Greedy rule: if target is even, divide by 2. If odd, add 1 first.
 * - When target <= startValue, just add (startValue - target).
 *
 * Time:  O(log(target)) -- each loop iteration at least halves target
 * Space: O(1)
 */

class Solution {
public:
    int brokenCalc(int startValue, int target) {
        int ops = 0;

        while (target > startValue) {
            if (target % 2 == 1) {
                // Target is odd: must add 1 to make it even
                target += 1;
                ops++;
            }
            // Target is now even: divide by 2
            target /= 2;
            ops++;
        }

        // target <= startValue: need (startValue - target) subtract ops
        ops += (startValue - target);
        return ops;
    }
};


/*
 * Approach 2: Greedy reverse with bit manipulation insight
 * - Same logic as Approach 1 but uses bitwise operations.
 * - Checking if target is odd: target & 1.
 * - Dividing by 2: target >>= 1.
 * - Slightly more efficient at the machine level.
 *
 * Time:  O(log(target))
 * Space: O(1)
 */

class Solution2 {
public:
    int brokenCalc(int startValue, int target) {
        int ops = 0;

        while (target > startValue) {
            if (target & 1) {
                // Odd: increment to make even
                target++;
                ops++;
            }
            // Even: halve
            target >>= 1;
            ops++;
        }

        return ops + (startValue - target);
    }
};


/*
 * Approach 3: Recursive formulation
 * - Base case: target <= startValue -> return startValue - target.
 * - Recursive case: if target is even, solve for target/2 and add 1 op.
 *   If target is odd, solve for (target+1)/2 and add 2 ops (add 1, divide).
 *
 * Time:  O(log(target))
 * Space: O(log(target)) for recursion stack
 */

class Solution3 {
public:
    int brokenCalc(int startValue, int target) {
        // Base case: can only subtract
        if (target <= startValue) {
            return startValue - target;
        }

        // Recursive case
        if (target % 2 == 0) {
            // Target is even: one divide operation
            return 1 + brokenCalc(startValue, target / 2);
        } else {
            // Target is odd: one add + one divide = 2 operations
            // (target + 1) / 2 is the result after add-1-then-halve
            return 2 + brokenCalc(startValue, (target + 1) / 2);
        }
    }
};


/*
 * Approach 4: Mathematical closed-form intuition
 * - Count the number of times we need to divide (based on bit length
 *   difference) and the number of times we need to add 1 (based on
 *   the binary representation of target relative to startValue).
 * - This is essentially what the greedy algorithm does, but we can
 *   reason about it more formally.
 *
 * The key mathematical insight: after k doublings from startValue,
 * we get startValue * 2^k. We want startValue * 2^k - (sum of subtracts) = target.
 * Rearranging: sum of subtracts = startValue * 2^k - target.
 * We choose the smallest k such that startValue * 2^k >= target.
 * The subtracts can be distributed optimally among the doubling steps.
 *
 * The greedy reverse algorithm implicitly finds this optimal k and
 * distribution. Each "add 1" in reverse corresponds to a "subtract 1"
 * before a specific doubling step. Each "divide by 2" corresponds to
 * removing one doubling step.
 *
 * Time:  O(log(target))
 * Space: O(1)
 */

class Solution4 {
public:
    int brokenCalc(int startValue, int target) {
        // This is functionally identical to Approach 1 but with
        // explicit documentation of the mathematical reasoning.

        int ops = 0;

        // Phase 1: Reduce target to <= startValue using reverse operations
        // Each division roughly halves the problem size.
        // Each add-1 adjusts parity before division.
        while (target > startValue) {
            // If target is odd, we need one extra subtract (reverse: add 1)
            // before the next doubling (reverse: divide by 2).
            // This is because in the forward direction, subtracting 1 then
            // doubling gives 2*(x-1) = 2x - 2, while doubling then
            // subtracting gives 2x - 1. The parity of target tells us
            // which sequence was used.
            if (target % 2 != 0) {
                target++;
                ops++;
            }
            target /= 2;
            ops++;
        }

        // Phase 2: target <= startValue
        // In the forward direction, we simply subtract 1 repeatedly.
        ops += startValue - target;

        return ops;
    }
};


/*
 * Key insights and common pitfalls:
 *
 * 1. Forward search is infeasible: From startValue, you can double or
 *    subtract 1. The search tree branches exponentially, and with target
 *    up to 10^9, BFS/DFS would explore billions of states. The reverse
 *    greedy approach reduces this to ~30 iterations.
 *
 * 2. Why reverse works so cleanly: In the forward direction, the choice
 *    between "double" and "subtract" is non-obvious (you might need to
 *    subtract several times before doubling). In reverse, the choice is
 *    deterministic: even -> divide, odd -> add 1 then divide. There is
 *    no branching, so no search is needed.
 *
 * 3. Proof that greedy is optimal: If target is even, dividing by 2
 *    reduces target by half in one operation. Any alternative (adding 1s
 *    before dividing) would result in a larger value after division,
 *    requiring more operations later. If target is odd, we must add 1
 *    first (cannot divide odd numbers to reach an integer that doubles
 *    to an odd number). So the greedy choice is forced.
 *
 * 4. When target <= startValue: Only subtraction is useful. Multiplying
 *    would increase the value further away from target. So we need
 *    exactly (startValue - target) subtraction operations.
 *
 * 5. No overflow concerns: target starts at most 10^9. Adding 1 makes
 *    it at most 10^9 + 1, which fits in int. Then we divide, bringing
 *    it back down. So intermediate values never exceed 10^9 + 1.
 *
 * 6. Related pattern: "Reverse the problem" is a powerful technique.
 *    Whenever forward exploration has exponential branching but reverse
 *    operations are deterministic, consider working backwards.
 *    See also: LC 780 (Reaching Points), LC 397 (Integer Replacement).
 */
