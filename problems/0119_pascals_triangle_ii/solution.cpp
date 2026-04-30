// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 119: Pascal's Triangle II
 * Difficulty: Easy
 * Tags: Array, Dynamic Programming
 *
 * Problem Description:
 * Given an integer `rowIndex`, return the `rowIndexth` (**0-indexed**) row of
 * the
 * **Pascal 's triangle**.
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
 *     **Input:** rowIndex = 3
 *     **Output:** [1,3,3,1]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** rowIndex = 0
 *     **Output:** [1]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** rowIndex = 1
 *     **Output:** [1,1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= rowIndex <= 33`
 * 
 * 
 * 
 * **Follow up:** Could you optimize your algorithm to use only `O(rowIndex)`
 * extra
 * space?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/pascals-triangle-ii/
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
    vector<int> getRow(int rowIndex) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 0
    // Test case 3: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
