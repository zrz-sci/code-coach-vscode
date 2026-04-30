// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 781: Rabbits in Forest
 * Difficulty: Medium
 * Tags: Greedy, Array, Hash Table, Math
 *
 * Problem Description:
 * There is a forest with an unknown number of rabbits. We asked n rabbits **"
 * How
 * many other rabbits have the same color as you?"** and collected the answers
 * in
 * an integer array `answers` where `answers[i]` is the answer of the `ith`
 * rabbit.
 * 
 * Given the array `answers`, return _the minimum number of rabbits that could
 * be
 * in the forest_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** answers = [1,1,2]
 *     **Output:** 5
 *     **Explanation:**
 *     The two rabbits that answered "1" could both be the same color, say red.
 * The rabbit that answered "2" can't be red or the answers would be
 * inconsistent.
 *     Say the rabbit that answered "2" was blue.
 * Then there should be 2 other blue rabbits in the forest that didn't answer
 * into the array.
 * The smallest possible number of rabbits in the forest is therefore 5: 3
 * that answered plus 2 that didn't.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** answers = [10,10,10]
 *     **Output:** 11
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= answers.length <= 1000`
 *   * `0 <= answers[i] < 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/rabbits-in-forest/
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
    int numRabbits(vector<int>& answers) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,1,2]
    // Test case 2: [10,10,10]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
