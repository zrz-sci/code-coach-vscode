// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 168: Excel Sheet Column Title
 * Difficulty: Easy
 * Tags: Math, String
 *
 * Problem Description:
 * Given an integer `columnNumber`, return _its corresponding column title as
 * it
 * appears in an Excel sheet_.
 * 
 * For example:
 * 
 *     
 *     
 *     A -> 1
 *     B -> 2
 *     C -> 3
 *     ...
 *     Z -> 26
 *     AA -> 27
 *     AB -> 28 
 *     ...
 *     
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** columnNumber = 1
 *     **Output:** "A"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** columnNumber = 28
 *     **Output:** "AB"
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** columnNumber = 701
 *     **Output:** "ZY"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= columnNumber <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/excel-sheet-column-title/
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
    string convertToTitle(int columnNumber) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 1
    // Test case 2: 28
    // Test case 3: 701

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
