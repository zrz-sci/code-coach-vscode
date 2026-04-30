// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 407: Trapping Rain Water II
 * Difficulty: Hard
 * Tags: Breadth-First Search, Array, Matrix, Heap (Priority Queue)
 *
 * Problem Description:
 * Given an `m x n` integer matrix `heightMap` representing the height of each
 * unit
 * cell in a 2D elevation map, return _the volume of water it can trap after
 * raining_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** heightMap = [[1,4,3,1,3,2],[3,2,1,3,2,4],[2,3,3,2,3,1]]
 *     **Output:** 4
 *     **Explanation:** After the rain, water is trapped between the blocks.
 *     We have two small ponds 1 and 3 units trapped.
 *     The total volume of water trapped is 4.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** heightMap =
 * [[3,3,3,3,3],[3,2,2,2,3],[3,2,1,2,3],[3,2,2,2,3],[3,3,3,3,3]]
 *     **Output:** 10
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == heightMap.length`
 *   * `n == heightMap[i].length`
 *   * `1 <= m, n <= 200`
 *   * `0 <= heightMap[i][j] <= 2 * 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/trapping-rain-water-ii/
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
    int trapRainWater(vector<vector<int>>& heightMap) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,4,3,1,3,2],[3,2,1,3,2,4],[2,3,3,2,3,1]]
    // Test case 2: [[3,3,3,3,3],[3,2,2,2,3],[3,2,1,2,3],[3,2,2,2,3],[3,3,3,3,3]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
