[file://solution.cpp](./solution.cpp)

# 911. Online Election

## 1. Problem Understanding (What is being asked?)

Design a class `TopVotedCandidate` that processes an election timeline and answers queries about who was leading at any given time.

**Constructor**: Given `persons[i]` (who received the i-th vote) and `times[i]` (when the i-th vote was cast), preprocess the election data. The `times` array is strictly increasing.

**Query `q(t)`**: Return the person who was leading at time `t`. Votes cast at time `t` count. In case of a tie, the candidate who received the most recent vote (among those tied) wins.

Key observations:
- The `times` array is sorted in strictly increasing order, which enables binary search for queries.
- We need to handle ties: the most recently voted candidate wins ties.
- Queries can be for any time `t >= times[0]`, not just times in the `times` array.
- Up to 10^4 queries, so each query must be efficient.
- The leader can only change at vote times (times in the `times` array), so we only need to track the leader at each vote event.

## 2. Key Insights & Patterns

**Pattern: Precomputation + Binary Search**

The key insight is to **precompute the leader at every vote event** during construction, then use **binary search** to answer queries.

1. **Precomputation**: Walk through the votes in order. Maintain a count map for each person. At each vote, update the count and determine the current leader. Store the leader at each time point.

2. **Query**: Given time `t`, binary search in the `times` array to find the latest vote event at or before time `t`. Return the precomputed leader at that event.

The tie-breaking rule ("most recent vote wins") is naturally handled during precomputation: when we process vote `i` and the person ties with the current leader, the newly voted person becomes the new leader.

## 3. Approach Discussion

### Approach A: Precompute Leaders + Binary Search (Optimal)
- In the constructor, iterate through all votes and compute the running leader at each time point.
- For queries, use `upper_bound` on the times array to find the relevant time point.
- Constructor: O(n), Query: O(log n). Space: O(n).

### Approach B: Brute Force per Query
- For each query `q(t)`, iterate through all votes with `times[i] <= t`, count votes, and find the leader.
- Constructor: O(1), Query: O(n). Total: O(n * Q) which is too slow for 5000 votes and 10^4 queries.

### Approach C: Balanced BST / TreeMap
- Store (time -> leader) in a TreeMap/ordered map. Use `lower_bound` for queries.
- Functionally equivalent to Approach A but uses a map instead of vectors + binary search.
- Slightly more overhead due to map operations.

**Best approach: Precompute Leaders + Binary Search (Approach A)** -- clean, efficient, and leverages the sorted `times` array.

## 4. Step-by-Step Solution Walk-through

**Constructor**:
1. Store the `times` array for binary search later.
2. Create a `leaders` vector of the same length as `times`.
3. Maintain a hash map `voteCount` mapping person -> count.
4. Track the current leader and their count.
5. For each vote `i`:
   - Increment `voteCount[persons[i]]`.
   - If `voteCount[persons[i]] >= currentLeaderCount`:
     - Update leader to `persons[i]` and `currentLeaderCount` to `voteCount[persons[i]]`.
     - Note: we use `>=` (not `>`) to implement the tie-breaking rule where the most recent vote wins.
   - Set `leaders[i] = currentLeader`.

**Query q(t)**:
1. Use `upper_bound(times.begin(), times.end(), t)` to find the first time strictly greater than `t`.
2. Decrement the iterator by 1 (or use the index) to get the last time `<= t`.
3. Return `leaders[index]`.

**Example**: persons = [0,1,1,0,0,1,0], times = [0,5,10,15,20,25,30]
- Vote 0 (t=0): person 0, counts={0:1}. Leader=0. leaders=[0]
- Vote 1 (t=5): person 1, counts={0:1,1:1}. Tie, recent vote wins -> Leader=1. leaders=[0,1]
- Vote 2 (t=10): person 1, counts={0:1,1:2}. Leader=1. leaders=[0,1,1]
- Vote 3 (t=15): person 0, counts={0:2,1:2}. Tie, recent vote wins -> Leader=0. leaders=[0,1,1,0]
- Vote 4 (t=20): person 0, counts={0:3,1:2}. Leader=0. leaders=[0,1,1,0,0]
- Vote 5 (t=25): person 1, counts={0:3,1:3}. Tie, recent vote wins -> Leader=1. leaders=[0,1,1,0,0,1]
- Vote 6 (t=30): person 0, counts={0:4,1:3}. Leader=0. leaders=[0,1,1,0,0,1,0]

Query q(3): upper_bound finds t=5 (index 1), so index=0. leaders[0]=0. Correct.
Query q(12): upper_bound finds t=15 (index 3), so index=2. leaders[2]=1. Correct.
Query q(25): upper_bound finds t=30 (index 6), so index=5. leaders[5]=1. Correct.

## 5. Complexity Analysis

| Aspect | Complexity |
|--------|-----------|
| Constructor Time | O(n) where n = number of votes |
| Query Time | O(log n) per query via binary search |
| Space | O(n) for storing times and leaders arrays |
| Total for Q queries | O(n + Q log n) |

## 6. Common Pitfalls & Edge Cases

1. **Tie-breaking with `>=` vs `>`**: The problem says "in case of a tie, the most recent vote wins." This means when updating the leader, use `>=` for the count comparison, not `>`. Using `>` would keep the older leader on ties, which is wrong.
2. **Binary search off-by-one**: Use `upper_bound` and then subtract 1. Using `lower_bound` directly would find the first time `>= t`, but if `t` is between two vote times, you want the one before it.
3. **Query at exact vote time**: Votes at time `t` should count. `upper_bound(t) - 1` correctly includes `t` if it is in the times array.
4. **Query before first vote**: The constraint says `times[0] <= t`, so we are guaranteed at least one vote has been cast. No need to handle "no votes yet."
5. **Large time values**: Times can be up to 10^9, but we only search within the `times` array, so this does not cause issues.
6. **Single candidate**: If only one person receives all votes, the answer is always that person. Handled naturally.

## 7. Related Problems & Patterns

| Problem | Relationship |
|---------|-------------|
| 981. Time Based Key-Value Store | Same pattern: precompute + binary search for time-based queries |
| 732. My Calendar III | Precomputation with sweep line for event-based queries |
| 1146. Snapshot Array | Versioned data structure with binary search on snapshots |
| 352. Data Stream as Disjoint Intervals | Design problem with ordered data and range queries |
| 362. Design Hit Counter | Time-based counter design problem |

## 8. Interview Tips & Communication Guide

**Opening statement**: "I will precompute the election leader at every vote event, then use binary search to answer each query in O(log n) time. The key insight is that the leader can only change at vote times, so we just need to look up the most recent vote event."

**Key points to communicate**:
- Explain the tie-breaking rule and how `>=` implements it naturally.
- Clarify why binary search works (times are sorted, leader only changes at vote events).
- Mention the `upper_bound` technique and why it correctly handles queries at exact vote times and between vote times.

**Follow-up questions to prepare for**:
- "What if votes could be added dynamically (not all known upfront)?" -- You would need to maintain a balanced BST or segment tree with dynamic updates. Each new vote updates the leader list and inserts into the searchable structure.
- "What if there are millions of candidates?" -- The hash map approach still works, as we only store candidates who have received at least one vote.
- "Can you make the constructor O(n) and queries O(1)?" -- If time values are bounded and small, you could use a direct lookup array. But with times up to 10^9, binary search is more practical.
- "What if we need the top K candidates instead of just the leader?" -- Use a more sophisticated data structure like a max-heap or balanced BST during precomputation to track top K.

**Time management**: Explain the design in 3 minutes, implement in 8-10 minutes. The implementation is straightforward once the approach is clear.
