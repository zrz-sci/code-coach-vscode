// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 171: Excel Sheet Column Number
 * Difficulty: Easy
 * Tags: Math, String
 *
 * Problem Description:
 * Given a string `columnTitle` that represents the column title as appears in
 * an
 * Excel sheet, return _its corresponding column number_.
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
 *     **Input:** columnTitle = "A"
 *     **Output:** 1
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** columnTitle = "AB"
 *     **Output:** 28
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** columnTitle = "ZY"
 *     **Output:** 701
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= columnTitle.length <= 7`
 *   * `columnTitle` consists only of uppercase English letters.
 *   * `columnTitle` is in the range `["A", "FXSHRXW"]`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/excel-sheet-column-number/
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
    int titleToNumber(string columnTitle) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "A"
    // Test case 2: "AB"
    // Test case 3: "ZY"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
