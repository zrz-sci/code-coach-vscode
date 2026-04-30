// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 624: Maximum Distance in Arrays
 * Difficulty: Medium
 * Tags: Greedy, Array
 *
 * Problem Description:
 * You are given `m` `arrays`, where each array is sorted in **ascending
 * order**.
 * 
 * You can pick up two integers from two different arrays (each array picks
 * one)
 * and calculate the distance. We define the distance between two integers `a`
 * and
 * `b` to be their absolute difference `|a - b|`.
 * 
 * Return _the maximum distance_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** arrays = [[1,2,3],[4,5],[1,2,3]]
 *     **Output:** 4
 * **Explanation:** One way to reach the maximum distance 4 is to pick 1 in
 * the first or third array and pick 5 in the second array.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** arrays = [[1],[1]]
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == arrays.length`
 *   * `2 <= m <= 105`
 *   * `1 <= arrays[i].length <= 500`
 *   * `-104 <= arrays[i][j] <= 104`
 *   * `arrays[i]` is sorted in **ascending order**.
 *   * There will be at most `105` integers in all the arrays.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-distance-in-arrays/
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
    int maxDistance(vector<vector<int>>& arrays) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2,3],[4,5],[1,2,3]]
    // Test case 2: [[1],[1]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
