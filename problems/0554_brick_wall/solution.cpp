// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 554: Brick Wall
 * Difficulty: Medium
 * Tags: Array, Hash Table
 *
 * Problem Description:
 * There is a rectangular brick wall in front of you with `n` rows of bricks.
 * The
 * `ith` row has some number of bricks each of the same height (i.e., one
 * unit) but
 * they can be of different widths. The total width of each row is the same.
 * 
 * Draw a vertical line from the top to the bottom and cross the least bricks.
 * If
 * your line goes through the edge of a brick, then the brick is not
 * considered as
 * crossed. You cannot draw a line just along one of the two vertical edges of
 * the
 * wall, in which case the line will obviously cross no bricks.
 * 
 * Given the 2D array `wall` that contains the information about the wall,
 * return
 * _the minimum number of crossed bricks after drawing such a vertical line_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** wall = [[1,2,2,1],[3,1,2],[1,3,2],[2,4],[3,1,2],[1,3,1,1]]
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** wall = [[1],[1],[1]]
 *     **Output:** 3
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == wall.length`
 *   * `1 <= n <= 104`
 *   * `1 <= wall[i].length <= 104`
 *   * `1 <= sum(wall[i].length) <= 2 * 104`
 *   * `sum(wall[i])` is the same for each row `i`.
 *   * `1 <= wall[i][j] <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/brick-wall/
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
    int leastBricks(vector<vector<int>>& wall) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2,2,1],[3,1,2],[1,3,2],[2,4],[3,1,2],[1,3,1,1]]
    // Test case 2: [[1],[1],[1]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
