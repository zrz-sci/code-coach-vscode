// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 118: Pascal's Triangle
 * Difficulty: Easy
 * Tags: Array, Dynamic Programming
 *
 * Problem Description:
 * Given an integer `numRows`, return the first numRows of **Pascal 's
 * triangle**.
 * 
 * In **Pascal 's triangle**, each number is the sum of the two numbers
 * directly
 * above it as shown:
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** numRows = 5
 *     **Output:** [[1],[1,1],[1,2,1],[1,3,3,1],[1,4,6,4,1]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** numRows = 1
 *     **Output:** [[1]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= numRows <= 30`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/pascals-triangle/
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
    vector<vector<int>> generate(int numRows) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 5
    // Test case 2: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
