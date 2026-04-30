[file://solution.cpp](./solution.cpp)

# 914. X of a Kind in a Deck of Cards

## 1. Problem Understanding (What is being asked?)

You are given an integer array `deck` where `deck[i]` represents the number on the i-th card. You need to determine if you can partition ALL cards into one or more groups such that:
1. Each group has exactly `x` cards (the same `x` for all groups), where `x > 1`.
2. All cards in the same group have the same number.

Return `true` if such a partition exists, `false` otherwise.

Key observations:
- We need a single value of `x >= 2` that works for ALL card types.
- If a number appears `count` times, then `count` must be divisible by `x` (so we can split those `count` cards into groups of exactly `x`).
- So `x` must divide the count of every distinct card number.
- We need to find some `x >= 2` that divides ALL counts simultaneously.
- This means `x` must divide the GCD of all counts. If the GCD is >= 2, the answer is `true`.

## 2. Key Insights & Patterns

**Pattern: GCD (Greatest Common Divisor) / Number Theory**

The problem reduces to a pure number theory question:

1. Count the frequency of each card number.
2. Compute the GCD of all frequencies.
3. If GCD >= 2, return `true`; otherwise return `false`.

Why does this work?
- If `g = GCD(count1, count2, ..., countK)`, then `g` divides every `countI`.
- We can set `x = g`, and partition each group of `countI` cards into `countI / g` groups of `g` cards each.
- If `g = 1`, no `x >= 2` can divide all counts, so it is impossible.

This is a beautiful example of how a seemingly combinatorial partition problem reduces to a simple GCD computation.

## 3. Approach Discussion

### Approach A: Frequency Count + GCD (Optimal)
- Count frequencies using a hash map.
- Compute the GCD of all frequencies.
- Return `GCD >= 2`.
- Time: O(n + k * log(maxCount)), Space: O(k) where k = number of distinct values.

### Approach B: Try All Possible X Values
- Count frequencies.
- For each possible `x` from 2 to max_count:
  - Check if `x` divides all counts.
  - If yes, return `true`.
- If no valid `x` found, return `false`.
- Time: O(n + maxCount * k), potentially slower than GCD approach.

### Approach C: GCD from Divisors
- Similar to Approach A but explicitly check divisors of the first count and test them against all other counts.
- No significant advantage over computing the running GCD.

**Best approach: Frequency Count + GCD (Approach A)** -- mathematically elegant, efficient, and minimal code.

## 4. Step-by-Step Solution Walk-through

1. **Count frequencies**: Use a hash map to count how many times each number appears.
   - Example: deck = [1,2,3,4,4,3,2,1] -> counts = {1:2, 2:2, 3:2, 4:2}

2. **Compute running GCD**: Initialize `g` with the first count, then iteratively compute `g = gcd(g, count)` for each subsequent count.
   - counts = {1:2, 2:2, 3:2, 4:2}
   - g = gcd(2, 2) = 2, gcd(2, 2) = 2, gcd(2, 2) = 2
   - Final g = 2

3. **Check result**: If `g >= 2`, return `true`. Otherwise return `false`.
   - g = 2 >= 2, so return `true`.

**Example 2**: deck = [1,1,1,2,2,2,3,3]
- counts = {1:3, 2:3, 3:2}
- g = gcd(3, 3) = 3, gcd(3, 2) = 1
- g = 1 < 2, return `false`.

**Example 3**: deck = [1,1,2,2,2,2]
- counts = {1:2, 2:4}
- g = gcd(2, 4) = 2
- g = 2 >= 2, return `true`. (Group into pairs: [1,1], [2,2], [2,2])

## 5. Complexity Analysis

| Aspect | Complexity |
|--------|-----------|
| Time | O(n) for counting frequencies + O(k * log(M)) for GCD computation, where k = distinct values and M = max frequency. Since k <= n and log(M) <= log(n), total is O(n log n) in the worst case, but typically O(n) |
| Space | O(k) for the frequency map, where k = number of distinct card values |

The GCD computation is extremely fast in practice due to the logarithmic convergence of the Euclidean algorithm.

## 6. Common Pitfalls & Edge Cases

1. **GCD of 0**: If the deck is empty, the answer is technically undefined by the constraints (deck.length >= 1). But deck.length = 1 means one card with count 1, GCD = 1 < 2, return `false`. This is correct because you need x >= 2 but only have 1 card of that type.
2. **All same cards**: deck = [1,1,1,1]. Count = {1:4}. GCD = 4 >= 2. Return `true`. Correct (one group of 4, or two groups of 2).
3. **Single distinct value with count 1**: deck = [5]. Count = {5:1}. GCD = 1 < 2. Return `false`. You cannot form a group of x >= 2 with only 1 card.
4. **Large counts with GCD = 1**: deck has values with counts [7, 11]. GCD = 1. Return `false`. Even though individual counts are large, no common x >= 2 divides both.
5. **Confusing x with group count**: x is the SIZE of each group (must be >= 2), NOT the number of groups. Each card type can have multiple groups of size x.
6. **Forgetting that all cards must be used**: Every card must be in some group. You cannot discard cards. This is why x must divide EVERY count.

## 7. Related Problems & Patterns

| Problem | Relationship |
|---------|-------------|
| 1071. Greatest Common Divisor of Strings | GCD applied to string lengths to determine common pattern |
| 365. Water and Jug Problem | GCD determines if a target volume is achievable |
| 858. Mirror Reflection | GCD for determining laser reflections |
| 2344. Minimum Deletions to Make Array Divisible | GCD of an array to determine divisibility |
| 1979. Find Greatest Common Divisor of Array | Direct GCD computation on array elements |

## 8. Interview Tips & Communication Guide

**Opening statement**: "This is a number theory problem disguised as a partition problem. After counting frequencies, the question becomes: does there exist an x >= 2 that divides all frequencies? This is equivalent to checking if the GCD of all frequencies is >= 2."

**Key points to communicate**:
- Explain the reduction from partition to GCD clearly. The interviewer wants to see the mathematical insight.
- Mention the Euclidean algorithm for GCD: `gcd(a, b) = gcd(b, a % b)`, which runs in O(log(min(a,b))) time.
- Explain why GCD >= 2 is both necessary (any valid x must divide all counts, so it must divide their GCD) and sufficient (we can use x = GCD itself).

**Follow-up questions to prepare for**:
- "What if you want to find the largest possible x?" -- The largest x is the GCD itself. Any divisor of the GCD is also a valid x.
- "What if x can be different for different groups?" -- Then you only need each count to be representable as a sum of integers >= 2, which is true for any count >= 2. So the answer is just: are all counts >= 2? But the original problem requires the SAME x for all groups.
- "What if the deck is very large (10^7 elements)?" -- The algorithm is already O(n), which handles this easily. The hash map might have memory pressure, but it is bounded by the number of distinct values.
- "Can you do it without a hash map?" -- Sort the array first, then count consecutive runs. Time: O(n log n), Space: O(1) extra (counts computed on the fly).

**Time management**: This is an easy problem. Spend 2 minutes on insight, 3 minutes on implementation, 2 minutes on edge cases. If you finish early, discuss the mathematical proof of correctness.
