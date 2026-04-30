[file://solution.cpp](solution.cpp)

# 991. Broken Calculator

## 1. Problem Summary
You have a broken calculator that initially displays `startValue`. In one operation, you can either multiply the display by 2 or subtract 1 from the display. Given `startValue` and `target`, find the minimum number of operations to reach `target`. For example, starting from 2 to reach 3 takes 2 operations: double (2->4), then subtract (4->3). Starting from 5 to reach 8 takes 2 operations: subtract (5->4), then double (4->8). Constraints allow values up to 10^9.

## 2. Key Insights / "Aha!" Moments
- **Reverse thinking is the key**: Instead of going from `startValue` to `target` (which requires exploring an exponentially branching search space), work **backwards** from `target` to `startValue`. The reverse operations are: divide by 2 (reverse of multiply by 2) and add 1 (reverse of subtract 1).
- **Greedy strategy on the reverse**: When working backwards from target, if target is even, dividing by 2 is always optimal (it reduces target faster than repeatedly adding 1). If target is odd, you must add 1 first to make it even before dividing. This greedy choice is provably optimal.
- **Why greedy works**: Dividing by 2 reduces the value much faster than subtracting 1 repeatedly. Any sequence that delays division unnecessarily will use more operations. Formally, if target is even, divide. If odd, (target+1)/2 is strictly more efficient than reaching target-1 first.
- **Base case**: When `target <= startValue`, you can only subtract 1 repeatedly (multiplying would overshoot). The answer adds `startValue - target` operations.
- **Forward BFS is too slow**: A BFS from startValue exploring multiply-by-2 and subtract-1 branches would have an enormous search space (up to 2 * 10^9 states). The greedy reverse approach runs in O(log(target)) time.

## 3. Approach Walkthrough (Step-by-Step)
**Approach: Greedy reverse (work backwards from target)**

1. **Initialize**: Set `ops = 0` (operation counter).
2. **Loop while `target > startValue`**:
   - If `target` is **odd**: We cannot divide an odd number to reach it via doubling. So increment `target` by 1 (reverse of subtract 1) and increment `ops`.
   - Now `target` is **even**: Divide `target` by 2 (reverse of multiply by 2) and increment `ops`.
3. **After the loop**: `target <= startValue`. The remaining gap must be closed by subtracting 1 from startValue, which in reverse means we added those 1s. Add `startValue - target` to `ops`.
4. **Return `ops`**.

**Walkthrough with Example**: startValue = 3, target = 10
- target = 10 (even) -> divide by 2 -> target = 5, ops = 1
- target = 5 (odd) -> add 1 -> target = 6, ops = 2
- target = 6 (even) -> divide by 2 -> target = 3, ops = 3
- target = 3 = startValue -> done. Total ops = 3.
- Verify forward: 3 -> 6 (double) -> 5 (subtract) -> 10 (double) = 3 operations. Correct!

**Why not just subtract until target < startValue?**
When target is odd, we could subtract 1 then divide: (target-1)/2. But this gives the same result as adding 1 then dividing: (target+1)/2 when we account for operations. Actually, adding 1 and dividing gives a larger value, which means fewer subsequent additions. The key is: one add + one divide = two operations to get (target+1)/2. But target-1 is even, and (target-1)/2 < (target+1)/2, meaning (target-1)/2 might be further from startValue. The greedy approach of dividing whenever possible (after making even) is provably optimal.

## 4. Complexity Analysis
- **Time Complexity**: O(log(target)). Each iteration of the main loop at least halves the target (after at most one add-1 operation). So the number of iterations is at most 2 * log2(target).
- **Space Complexity**: O(1). Only a constant number of variables are used.
- With target up to 10^9, log2(10^9) is about 30, so the algorithm runs in at most ~60 iterations. Extremely fast.

## 5. Edge Cases to Watch
- **startValue == target**: No operations needed. Return 0.
- **startValue > target**: Can only subtract. Return `startValue - target`. Multiplying would only increase the gap.
- **target == 1**: If startValue is 1, return 0. If startValue > 1, return startValue - 1 (subtract repeatedly).
- **target is a power of 2 and startValue == 1**: The answer is log2(target) -- just double repeatedly.
- **Very large target (10^9)**: The algorithm handles this in ~30 iterations. No overflow risk with int (max intermediate value is target + 1 which fits in int).
- **startValue == 1, target == 10^9**: Requires about 30 multiply operations plus some subtracts. The greedy reverse computes this efficiently.
- **target is odd and very large**: Requires one add to make it even, then proceeds normally. The extra operation is accounted for.

## 6. Similar / Related Problems
- **LeetCode 397 - Integer Replacement**: Similar concept of dividing/multiplying to reach a target with minimum operations. Also uses the insight that working backwards with even/odd analysis is effective.
- **LeetCode 780 - Reaching Points**: Another "reverse the operations" problem where working backwards from target is much simpler than forward search.
- **LeetCode 1342 - Number of Steps to Reduce a Number to Zero**: Simpler version involving dividing by 2 and subtracting 1.
- **LeetCode 1654 - Minimum Jumps to Reach Home**: BFS-based minimum operations problem on a number line.
- **LeetCode 2139 - Minimum Moves to Reach Target Score**: Very similar structure with doubling and incrementing.

## 7. Mnemonics / Memory Hooks
- **"Go backwards, it's greedy"**: Working from target to start makes the greedy choice obvious: always divide when even, add 1 when odd.
- **"Even? Halve it. Odd? Fix it."**: At each step, check parity. Even numbers get halved. Odd numbers get incremented to become even, then halved.
- **"Below start? Just subtract the gap."**: Once target drops to or below startValue, the remaining operations are just `startValue - target` subtractions.
- **"Division beats subtraction"**: Dividing by 2 reduces the number exponentially; subtracting 1 reduces linearly. Always prefer division in the reverse direction.

## 8. Code Template / Pseudocode
```
function brokenCalc(startValue, target):
    ops = 0
    
    while target > startValue:
        if target is odd:
            target += 1    // reverse of subtract 1
            ops += 1
        target /= 2        // reverse of multiply by 2 (target is now even)
        ops += 1
    
    // target <= startValue, need (startValue - target) subtract operations
    ops += (startValue - target)
    return ops
```

Alternative iterative approach (forward BFS - too slow for large inputs, included for understanding):
```
function brokenCalcBFS(startValue, target):
    // WARNING: This is O(target) and will TLE for large inputs
    visited = set()
    queue = [(startValue, 0)]  // (value, operations)
    
    while queue is not empty:
        (val, ops) = queue.dequeue()
        if val == target:
            return ops
        if val in visited:
            continue
        visited.add(val)
        
        // Try both operations
        if val * 2 not in visited and val * 2 <= 2 * target:
            queue.enqueue((val * 2, ops + 1))
        if val - 1 not in visited and val - 1 >= 0:
            queue.enqueue((val - 1, ops + 1))
    
    return -1  // should not reach here
```
