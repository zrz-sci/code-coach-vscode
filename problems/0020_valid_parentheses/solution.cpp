// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 20: Valid Parentheses
 * Difficulty: Easy
 * Tags: Stack, String
 *
 * Problem Description:
 * Given a string `s` containing just the characters `'('`, `')'`, `'{'`,
 * `'}'`,
 * `'['` and `']'`, determine if the input string is valid.
 * 
 * An input string is valid if:
 * 
 *   1. Open brackets must be closed by the same type of brackets.
 *   2. Open brackets must be closed in the correct order.
 *   3. Every close bracket has a corresponding open bracket of the same type.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** s = "()"
 * 
 * **Output:** true
 * 
 * **Example 2:**
 * 
 * **Input:** s = "()[]{}"
 * 
 * **Output:** true
 * 
 * **Example 3:**
 * 
 * **Input:** s = "(]"
 * 
 * **Output:** false
 * 
 * **Example 4:**
 * 
 * **Input:** s = "([])"
 * 
 * **Output:** true
 * 
 * **Example 5:**
 * 
 * **Input:** s = "([)]"
 * 
 * **Output:** false
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 104`
 *   * `s` consists of parentheses only `'()[]{}'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/valid-parentheses/
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
    bool isValid(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "()"
    // Test case 2: "()[]{}"
    // Test case 3: "(]"
    // Test case 4: "([])"
    // Test case 5: "([)]"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
