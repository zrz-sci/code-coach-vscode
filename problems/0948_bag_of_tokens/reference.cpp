/*
 * LeetCode 948 - Bag of Tokens
 *
 * Approach: Sort + Two Pointers (Greedy)
 *
 * Key Idea:
 *   Sort tokens. Use the smallest tokens to gain score (face-up, spend power),
 *   and the largest tokens to gain power (face-down, spend score).
 *   Two pointers converge from both ends. Track the maximum score achieved.
 *
 * Time Complexity:  O(n log n) for sorting; O(n) for the two-pointer pass
 * Space Complexity: O(1) auxiliary (O(log n) for sort stack)
 *
 * Reference: ./solution.cpp
 */

// ============================================================
// Solution 1: Sort + Two Pointers (Optimal)
// ============================================================
class Solution {
public:
    int bagOfTokensScore(vector<int>& tokens, int power) {
        // Sort tokens so we can use the cheapest for score and the most
        // expensive for power
        sort(tokens.begin(), tokens.end());

        int left = 0;
        int right = (int)tokens.size() - 1;
        int score = 0;
        int maxScore = 0;

        while (left <= right) {
            if (power >= tokens[left]) {
                // Play the cheapest remaining token face-up:
                // Spend power, gain 1 score
                power -= tokens[left];
                ++score;
                ++left;
                // Update the running maximum score
                maxScore = max(maxScore, score);
            } else if (score >= 1) {
                // Not enough power for the cheapest token, but we have score.
                // Play the most expensive remaining token face-down:
                // Spend 1 score, gain power equal to the token value.
                power += tokens[right];
                --score;
                --right;
            } else {
                // Cannot play face-up (not enough power) and cannot play
                // face-down (no score). No further moves possible.
                break;
            }
        }

        return maxScore;
    }
};

// ============================================================
// Solution 2: Explicit State Tracking (More Verbose)
// ============================================================
//
// Same logic as Solution 1 but with more explicit state tracking
// and comments for clarity. Useful for interview whiteboarding.
class Solution2 {
public:
    int bagOfTokensScore(vector<int>& tokens, int power) {
        if (tokens.empty()) return 0;

        sort(tokens.begin(), tokens.end());

        int n = tokens.size();
        int left = 0;        // next cheapest token to consider for face-up
        int right = n - 1;   // next most expensive token to consider for face-down
        int score = 0;
        int maxScore = 0;

        // Strategy:
        // 1. Always try to play face-up first (gain score, which is our goal)
        // 2. If we can't afford the cheapest token, play the most expensive
        //    face-down to recharge power (sacrificing 1 score for maximum power)
        // 3. If we can do neither, we're done

        while (left <= right) {
            // Attempt face-up: spend power, gain score
            if (power >= tokens[left]) {
                power -= tokens[left];
                score++;
                left++;

                // This is the key: track the peak score at any point
                if (score > maxScore) {
                    maxScore = score;
                }
                continue; // try to play more face-up if possible
            }

            // Attempt face-down: spend score, gain power
            if (score > 0) {
                power += tokens[right];
                score--;
                right--;
                continue; // now try face-up again with more power
            }

            // Dead end: no power and no score
            break;
        }

        return maxScore;
    }
};

// ============================================================
// Detailed Walkthrough with Example 3
// ============================================================
//
// tokens = [100, 200, 300, 400], power = 200
// After sort: [100, 200, 300, 400] (already sorted)
//
// Initial state:
//   left=0, right=3, power=200, score=0, maxScore=0
//
// Step 1: power(200) >= tokens[0](100)
//   Play tokens[0]=100 face-up
//   power = 200 - 100 = 100, score = 1, left = 1
//   maxScore = max(0, 1) = 1
//
// Step 2: power(100) < tokens[1](200), but score(1) >= 1
//   Play tokens[3]=400 face-down
//   power = 100 + 400 = 500, score = 0, right = 2
//
// Step 3: power(500) >= tokens[1](200)
//   Play tokens[1]=200 face-up
//   power = 500 - 200 = 300, score = 1, left = 2
//   maxScore = max(1, 1) = 1
//
// Step 4: power(300) >= tokens[2](300)
//   Play tokens[2]=300 face-up
//   power = 300 - 300 = 0, score = 2, left = 3
//   maxScore = max(1, 2) = 2
//
// Step 5: left(3) > right(2), loop ends
//
// Return maxScore = 2
//
// ============================================================
// Walkthrough: Why maxScore Tracking Matters
// ============================================================
//
// Consider: tokens = [100, 200], power = 100
// After sort: [100, 200]
//
// Step 1: power(100) >= tokens[0](100)
//   Face-up: power=0, score=1, left=1
//   maxScore = 1
//
// Step 2: power(0) < tokens[1](200), score(1) >= 1
//   Face-down: power=200, score=0, right=0
//
// Step 3: left(1) > right(0), loop ends
//
// Final score = 0, but maxScore = 1. Without tracking maxScore,
// we would incorrectly return 0.
//
// ============================================================
// Edge Cases
// ============================================================
//
// 1. Empty tokens: tokens = [], power = 100
//    Loop never executes. Return 0.
//
// 2. Single token, enough power: tokens = [100], power = 100
//    Play face-up: score = 1. Return 1.
//
// 3. Single token, not enough power: tokens = [100], power = 50
//    Cannot play face-up (50 < 100). Cannot play face-down (score=0).
//    Return 0.
//
// 4. All tokens affordable: tokens = [10, 20, 30], power = 100
//    Play all face-up: score = 3, power = 100-10-20-30 = 40.
//    Return 3.
//
// 5. No token affordable, score=0: tokens = [1000, 2000], power = 1
//    Cannot do anything. Return 0.
//
// ============================================================
// Greedy Proof Sketch
// ============================================================
//
// Claim: The two-pointer greedy achieves the optimal score.
//
// Proof intuition:
// - If we decide to play k tokens face-up, we should choose the k
//   cheapest ones (minimizes total power spent).
// - If we decide to play m tokens face-down, we should choose the m
//   most expensive ones (maximizes total power gained).
// - The two-pointer naturally enforces both of these properties.
// - The only question is the interleaving order of face-up and face-down
//   plays. The greedy processes face-up first (always prefer score gain),
//   and resorts to face-down only when necessary for power. This ensures
//   we do not "waste" face-down plays unnecessarily.
//
// A formal proof can be done by exchange argument: swapping any
// non-greedy move with the greedy choice leads to equal or worse
// outcome.
