// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 837: New 21 Game
 * Difficulty: Medium
 * Tags: Math, Dynamic Programming, Sliding Window, Probability and Statistics
 *
 * Problem Description:
 * Alice plays the following game, loosely based on the card game **" 21"**.
 * 
 * Alice starts with `0` points and draws numbers while she has less than `k`
 * points. During each draw, she gains an integer number of points randomly
 * from
 * the range `[1, maxPts]`, where `maxPts` is an integer. Each draw is
 * independent
 * and the outcomes have equal probabilities.
 * 
 * Alice stops drawing numbers when she gets `k` **or more points**.
 * 
 * Return the probability that Alice has `n` or fewer points.
 * 
 * Answers within `10-5` of the actual answer are considered accepted.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 10, k = 1, maxPts = 10
 *     **Output:** 1.00000
 *     **Explanation:** Alice gets a single card, then stops.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 6, k = 1, maxPts = 10
 *     **Output:** 0.60000
 *     **Explanation:** Alice gets a single card, then stops.
 *     In 6 out of 10 possibilities, she is at or below 6 points.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** n = 21, k = 17, maxPts = 10
 *     **Output:** 0.73278
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= k <= n <= 104`
 *   * `1 <= maxPts <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/new-21-game/
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

class Solution {
public:
    double new21Game(int n, int k, int maxPts) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 10
    // Test case 2: 1
    // Test case 3: 10
    // Test case 4: 6
    // Test case 5: 1
    // Test case 6: 10
    // Test case 7: 21
    // Test case 8: 17
    // Test case 9: 10

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
