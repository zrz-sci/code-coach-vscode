// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 849: Maximize Distance to Closest Person
 * Difficulty: Medium
 * Tags: Array
 *
 * Problem Description:
 * You are given an array representing a row of `seats` where `seats[i] = 1`
 * represents a person sitting in the `ith` seat, and `seats[i] = 0` represents
 * that the `ith` seat is empty **(0-indexed)**.
 * 
 * There is at least one empty seat, and at least one person sitting.
 * 
 * Alex wants to sit in the seat such that the distance between him and the
 * closest
 * person to him is maximized.
 * 
 * Return _that maximum distance to the closest person_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** seats = [1,0,0,0,1,0,1]
 *     **Output:** 2
 *     **Explanation:**
 * If Alex sits in the second open seat (i.e. seats[2]), then the closest
 * person has distance 2.
 *     If Alex sits in any other open seat, the closest person has distance 1.
 *     Thus, the maximum distance to the closest person is 2.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** seats = [1,0,0,0]
 *     **Output:** 3
 *     **Explanation:**
 * If Alex sits in the last seat (i.e. seats[3]), the closest person is 3
 * seats away.
 *     This is the maximum distance possible, so the answer is 3.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** seats = [0,1]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= seats.length <= 2 * 104`
 *   * `seats[i]` is `0` or `1`.
 *   * At least one seat is **empty**.
 *   * At least one seat is **occupied**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximize-distance-to-closest-person/
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
    int maxDistToClosest(vector<int>& seats) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,0,0,0,1,0,1]
    // Test case 2: [1,0,0,0]
    // Test case 3: [0,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
