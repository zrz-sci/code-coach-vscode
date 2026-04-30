// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 335: Self Crossing
 * Difficulty: Hard
 * Tags: Geometry, Array, Math
 *
 * Problem Description:
 * You are given an array of integers `distance`.
 * 
 * You start at the point `(0, 0)` on an **X-Y plane,** and you move
 * `distance[0]`
 * meters to the north, then `distance[1]` meters to the west, `distance[2]`
 * meters
 * to the south, `distance[3]` meters to the east, and so on. In other words,
 * after
 * each move, your direction changes counter-clockwise.
 * 
 * Return `true` _if your path crosses itself or_`false` _if it does not_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** distance = [2,1,1,2]
 *     **Output:** true
 *     **Explanation:** The path crosses itself at the point (0, 1).
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** distance = [1,2,3,4]
 *     **Output:** false
 *     **Explanation:** The path does not cross itself at any point.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** distance = [1,1,1,2,1]
 *     **Output:** true
 *     **Explanation:** The path crosses itself at the point (0, 0).
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= distance.length <= 105`
 *   * `1 <= distance[i] <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/self-crossing/
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
    bool isSelfCrossing(vector<int>& distance) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,1,1,2]
    // Test case 2: [1,2,3,4]
    // Test case 3: [1,1,1,2,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
