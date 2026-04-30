// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1249: Minimum Remove to Make Valid Parentheses
 * Difficulty: Medium
 * Tags: Stack, String
 *
 * Problem Description:
 * Given a string s of `'('` , `')'` and lowercase English characters.
 * 
 * Your task is to remove the minimum number of parentheses ( `'('` or `')'`,
 * in
 * any positions ) so that the resulting _parentheses string_ is valid and
 * return
 * **any** valid string.
 * 
 * Formally, a _parentheses string_ is valid if and only if:
 * 
 *   * It is the empty string, contains only lowercase characters, or
 * * It can be written as `AB` (`A` concatenated with `B`), where `A` and `B`
 * are valid strings, or
 *   * It can be written as `(A)`, where `A` is a valid string.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "lee(t(c)o)de)"
 *     **Output:** "lee(t(c)o)de"
 *     **Explanation:** "lee(t(co)de)" , "lee(t(c)ode)" would also be accepted.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "a)b(c)d"
 *     **Output:** "ab(c)d"
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "))(("
 *     **Output:** ""
 *     **Explanation:** An empty string is also valid.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `s[i]` is either `'('` , `')'`, or lowercase English letter.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-remove-to-make-valid-parentheses/
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
    string minRemoveToMakeValid(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "lee(t(c)o)de)"
    // Test case 2: "a)b(c)d"
    // Test case 3: "))(("

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
