# LeetCode 948 - Bag of Tokens

[View Solution](./solution.cpp)

## 1. Problem Understanding (Read & Rephrase)

You start with a given amount of `power` and a `score` of 0. You have a bag of tokens, each with an integer value. On each move, you pick an unplayed token and play it in one of two ways:

- **Face-up:** If your current power >= token value, you lose that much power and gain 1 score.
- **Face-down:** If your current score >= 1, you gain that much power and lose 1 score.

Each token can be played at most once, and you choose which way to play it. The goal is to maximize your final score.

**Restatement:** You have a "currency exchange" system -- power can buy score (at the cost of token value) and score can buy power (gaining token value). You want to maximize score by strategically choosing which tokens to spend power on and which to cash in for more power.

**Key intuition:** To maximize score, play cheap tokens face-up (spend the least power to gain score) and expensive tokens face-down (gain the most power when you need more). This naturally leads to a two-pointer approach on a sorted array.

## 2. Key Observations & Insights

1. **Sort the tokens:** After sorting, the smallest tokens are the cheapest to play face-up (gaining score with minimum power cost) and the largest tokens yield the most power when played face-down (gaining maximum power for the score spent).

2. **Two-pointer greedy:** Use a left pointer starting at the smallest token and a right pointer starting at the largest. Greedily play small tokens face-up for score, and when you cannot afford the next small token, play the largest remaining token face-down for power.

3. **Score as a resource:** You need at least 1 score to play face-down. So you can only trade score for power if you have already accumulated some score.

4. **Greedy correctness:** If you are going to spend 1 score to gain power, you should gain as much power as possible (take the largest remaining token). If you are going to spend power to gain 1 score, you should spend as little as possible (take the smallest remaining token).

5. **Stopping condition:** Stop when the left pointer crosses the right pointer, or when you cannot play any more tokens (not enough power for the cheapest remaining token AND not enough score to play face-down).

6. **Track the maximum score:** Your score might temporarily decrease when you play face-down. Always track the maximum score achieved at any point during the process.

## 3. Approach Categories

| Approach | Time | Space | Notes |
|---|---|---|---|
| Sort + Two Pointers (Greedy) | O(n log n) | O(1) | Optimal. Sort dominates. Greedy simulation. |
| Brute force (all subsets) | O(3^n * n) | O(n) | Each token: face-up, face-down, or skip. Way too slow. |
| DP on sorted array | O(n^2) | O(n^2) | Possible but unnecessary given greedy works |

**Recommended:** Sort + Two Pointers -- simple, optimal, and elegant.

## 4. Step-by-Step Solution (Sort + Two Pointers)

1. Sort the `tokens` array in non-decreasing order.
2. Initialize `left = 0`, `right = tokens.size() - 1`, `score = 0`, `maxScore = 0`.
3. While `left <= right`:
   a. If `power >= tokens[left]`:
      - Play token at `left` face-up: `power -= tokens[left]`, `score += 1`, `left += 1`.
      - Update `maxScore = max(maxScore, score)`.
   b. Else if `score >= 1`:
      - Play token at `right` face-down: `power += tokens[right]`, `score -= 1`, `right -= 1`.
   c. Else:
      - Cannot play any token. Break.
4. Return `maxScore`.

**Why this works:**
- We always spend power on the cheapest available token (maximizing the number of face-up plays).
- When we need more power, we sacrifice 1 score for the maximum possible power gain (the largest remaining token).
- We track `maxScore` separately because the final score might be lower than the peak (due to face-down plays at the end).

## 5. Complexity Analysis

**Time Complexity:** O(n log n) where `n` is the number of tokens.
- Sorting: O(n log n).
- Two-pointer traversal: O(n) -- each token is visited at most once (either by left moving right or right moving left).
- Overall dominated by sorting.

**Space Complexity:** O(1) auxiliary (or O(log n) for the sort stack, depending on the sort implementation). We modify no additional data structures beyond a few integer variables.

## 6. Common Mistakes & Edge Cases

- **Forgetting to track maxScore separately:** The score can decrease when you play face-down. If you just return `score` at the end, you might miss a higher score achieved earlier. Always maintain a running maximum.
- **Infinite loop when score = 0 and not enough power:** If you cannot play face-up (not enough power) and cannot play face-down (score is 0), you must break out of the loop. Forgetting this leads to an infinite loop.
- **Playing face-down when left == right:** If there is only one token left and you play it face-down, you gain power but lose score, which is never beneficial (you gain power but have nothing left to spend it on). The condition `left <= right` combined with the greedy priorities handles this correctly: you would only play face-down if there are still cheaper tokens to the left that you could subsequently play face-up.
- **Edge case -- empty tokens:** If `tokens` is empty, return 0 immediately.
- **Edge case -- single token:** If `tokens = [100]` and `power = 50`, you cannot play it face-up (50 < 100) and cannot play it face-down (score = 0). Answer is 0. If `power = 100`, play it face-up for score = 1.
- **Edge case -- all tokens too expensive:** If power is less than the smallest token and score is 0, answer is 0.
- **Edge case -- abundant power:** If power is very large, you can play all tokens face-up. Answer is `n`.

## 7. Related LeetCode Problems

| Problem | Relationship |
|---|---|
| **11. Container With Most Water** | Two-pointer technique on a sorted/ordered structure |
| **881. Boats to Save People** | Sort + two pointers, greedy pairing of small and large |
| **826. Most Profit Assigning Work** | Sort + greedy to maximize a resource metric |
| **1029. Two City Scheduling** | Greedy sorting by cost differential |
| **621. Task Scheduler** | Greedy resource management with scoring |
| **134. Gas Station** | Greedy approach to resource (fuel) management around a cycle |

## 8. Interview Tips & Follow-ups

- **Explain the greedy intuition before coding:** "Sort tokens. Use small tokens to buy score cheaply, use large tokens to buy power when needed. Two pointers naturally manage this."
- **Justify why greedy is optimal:** If we are going to spend 1 score, we should maximize the power gained (largest token). If we are going to spend power, we should minimize the cost (smallest token). Any other choice is dominated by this greedy strategy.
- **Mention the maxScore tracking:** This is a subtle but critical detail. Explain that the score trajectory can go up and down, so the final score is not necessarily the maximum.
- **Walk through Example 3 in detail:** `tokens = [100, 200, 300, 400], power = 200`. This example beautifully illustrates the "buy score, sell for power, buy more score" pattern. Step through it to show the two-pointer movement.
- **Follow-up -- what if tokens can be replayed?** If tokens can be used multiple times, the problem becomes fundamentally different. You would want to find a cycle that generates net score. This leads to a more complex analysis.
- **Follow-up -- minimize power to achieve score `k`:** Given a target score, what is the minimum starting power needed? This is a binary search on the initial power combined with the greedy simulation.
- **Follow-up -- multiple token types with different face-up/face-down values:** A generalization where face-up and face-down yields differ. This might require DP instead of greedy.
- **Code cleanliness:** The two-pointer loop fits in about 10 lines. Keep it clean. Name variables clearly (`left`, `right`, `score`, `maxScore`, `power`).
