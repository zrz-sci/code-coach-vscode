[file://solution.cpp](./solution.cpp)

# 910. Smallest Range II

## 1. Problem Understanding (What is being asked?)

You are given an integer array `nums` and an integer `k`. For each element `nums[i]`, you must change it to either `nums[i] + k` or `nums[i] - k` (you MUST apply one of the two operations to every element -- you cannot leave it unchanged). The **score** is `max(modified_nums) - min(modified_nums)`. Return the minimum possible score.

Key observations:
- Every element must be modified by exactly +k or -k. There is no "do nothing" option.
- We want to minimize the spread (max - min) of the resulting array.
- If k = 0, the answer is simply the original range `max(nums) - min(nums)`.
- Intuitively, we want to "compress" the range: increase small values (+k) and decrease large values (-k).

This is fundamentally different from LeetCode 908 (Smallest Range I) where you can add any value in [-k, k]. Here the choice is binary: +k or -k.

## 2. Key Insights & Patterns

**Pattern: Greedy + Sorting + Partitioning**

The crucial insight is that after sorting, the optimal strategy partitions the array into two groups:
- A **left group** (smaller elements) that gets +k
- A **right group** (larger elements) that gets -k

This partition point can be at any index i (meaning elements 0..i get +k, elements i+1..n-1 get -k). We try all n-1 possible partition points and take the minimum score.

Why does sorting + partitioning work?
1. If you sort the array, you would never want a larger element to get +k while a smaller element gets -k (that would only increase the spread).
2. So the optimal assignment must have some prefix getting +k and the rest getting -k.
3. After the partition at index i:
   - The candidates for the new maximum are: `nums[i] + k` (largest in left group) or `nums[n-1] - k` (largest in right group).
   - The candidates for the new minimum are: `nums[0] + k` (smallest in left group) or `nums[i+1] - k` (smallest in right group).
   - The score = `max(nums[i]+k, nums[n-1]-k) - min(nums[0]+k, nums[i+1]-k)`.

## 3. Approach Discussion

### Approach A: Sorting + Greedy Partition (Optimal)
- Sort the array.
- Initialize the answer as `nums[n-1] - nums[0]` (the case where all elements get the same operation, which cancels out).
- For each partition point i from 0 to n-2:
  - Compute `high = max(nums[i] + k, nums[n-1] - k)`
  - Compute `low = min(nums[0] + k, nums[i+1] - k)`
  - Update answer with `min(answer, high - low)`
- Time: O(n log n) for sorting, O(n) for the scan.

### Approach B: Brute Force (2^n)
- Try all 2^n combinations of +k/-k for each element.
- Compute the score for each combination.
- Time: O(2^n * n), completely infeasible for n up to 10^4.

### Approach C: Binary Search on Answer
- Binary search on the score value and check if it is achievable.
- The check would still require understanding the partition structure, making this approach more complex without improving time complexity.

**Best approach: Sorting + Greedy Partition (Approach A)** -- elegant, efficient, and straightforward once the insight is understood.

## 4. Step-by-Step Solution Walk-through

1. **Sort** the array in non-decreasing order.

2. **Base case**: Initialize `result = nums[n-1] - nums[0]`. This represents the scenario where we apply the same operation to all elements (the k offsets cancel out).

3. **Iterate** over each possible partition point `i` from 0 to n-2:
   - Elements `nums[0..i]` receive +k.
   - Elements `nums[i+1..n-1]` receive -k.
   - The new maximum is `max(nums[i] + k, nums[n-1] - k)`:
     - `nums[i] + k` is the largest element in the left group after +k.
     - `nums[n-1] - k` is the largest element in the right group after -k.
   - The new minimum is `min(nums[0] + k, nums[i+1] - k)`:
     - `nums[0] + k` is the smallest element in the left group after +k.
     - `nums[i+1] - k` is the smallest element in the right group after -k.
   - Update `result = min(result, high - low)`.

4. **Return** `result`.

**Example: nums = [1, 3, 6], k = 3**
- Sorted: [1, 3, 6]. Initial result = 6 - 1 = 5.
- i=0: left=[1+3=4], right=[3-3=0, 6-3=3]. high=max(4,3)=4, low=min(4,0)=0. score=4. result=4.
- i=1: left=[1+3=4, 3+3=6], right=[6-3=3]. high=max(6,3)=6, low=min(4,3)=3. score=3. result=3.
- Answer: 3. (Array becomes [4, 6, 3], range = 6-3 = 3.)

## 5. Complexity Analysis

| Aspect | Complexity |
|--------|-----------|
| Time | O(n log n) for sorting + O(n) for the linear scan = O(n log n) overall |
| Space | O(1) extra space (or O(log n) for the sort's stack space) |

The algorithm is optimal because we must at least read all elements, and sorting is the bottleneck.

## 6. Common Pitfalls & Edge Cases

1. **Forgetting the initial answer**: The initial answer `nums[n-1] - nums[0]` covers the case where all elements get the same operation. Without this, you might miss the optimal solution when k is very large.
2. **Off-by-one in the loop**: The loop runs from i=0 to i=n-2 (inclusive). At i=n-1, there would be no right group, which is already covered by the initial answer.
3. **Confusing with Smallest Range I (LC 908)**: In LC 908, you can add any value in [-k, k]. Here, you MUST add exactly +k or -k. The greedy is completely different.
4. **Negative values after subtraction**: `nums[i+1] - k` can be negative. This is fine; the formula still works correctly.
5. **Single element**: If n=1, the answer is always 0 (the element becomes either nums[0]+k or nums[0]-k, and max-min of a single element is 0). The initial answer `nums[0]-nums[0] = 0` handles this.
6. **k = 0**: Every element stays the same, answer = nums[n-1] - nums[0]. Handled by the initial answer.
7. **All elements equal**: Answer is 0 regardless of k (add +k to all or -k to all). Also handled correctly.

## 7. Related Problems & Patterns

| Problem | Relationship |
|---------|-------------|
| 908. Smallest Range I | Simpler variant: can add any value in [-k, k] instead of exactly +k or -k |
| 561. Array Partition | Sort-based greedy to optimize a sum involving pairs |
| 1509. Minimum Difference Between Largest and Smallest Value in Three Moves | Sorting + greedy to minimize range by modifying at most 3 elements |
| 2616. Minimize the Maximum Difference of Pairs | Sorting + binary search + greedy for minimizing maximum pair differences |
| 1423. Maximum Points You Can Obtain from Cards | Greedy prefix/suffix optimization after sorting |

## 8. Interview Tips & Communication Guide

**Opening statement**: "After sorting, the optimal strategy partitions the array into a left group that gets +k and a right group that gets -k. We enumerate all partition points and compute the resulting range for each."

**Key points to communicate**:
- Explain why sorting enables the partition approach (no sorted element would benefit from the opposite operation of a smaller neighbor).
- Walk through the formula for new max and new min at each partition point.
- Mention the edge case of the initial answer where all elements get the same operation.

**Follow-up questions to prepare for**:
- "What if you could also leave elements unchanged (three choices: +k, -k, or 0)?" -- You would need to consider more partition points but the sorted structure still helps. The problem becomes more complex but the same greedy intuition applies with two partition boundaries.
- "What if k is different for each element?" -- This becomes a much harder optimization problem, potentially requiring DP or other techniques.
- "Can you prove that the optimal solution must be a contiguous partition of the sorted array?" -- Yes: if sorted element a < b, and a gets -k while b gets +k, swapping their operations reduces the range.

**Time management**: The key insight (sort + partition) should take 2-3 minutes to explain. Implementation is only 10-15 lines. Spend time on the proof of correctness if asked.
