// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 44: Wildcard Matching
 * Difficulty: Hard
 * Tags: Greedy, Recursion, String, Dynamic Programming
 *
 * Problem Description:
 * Given an input string (`s`) and a pattern (`p`), implement wildcard pattern
 * matching with support for `'?'` and `'*'` where:
 * 
 *   * `'?'` Matches any single character.
 *   * `'*'` Matches any sequence of characters (including the empty sequence).
 * 
 * The matching should cover the **entire** input string (not partial).
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "aa", p = "a"
 *     **Output:** false
 *     **Explanation:** "a" does not match the entire string "aa".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "aa", p = "*"
 *     **Output:** true
 *     **Explanation:**  '*' matches any sequence.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "cb", p = "?a"
 *     **Output:** false
 * **Explanation:** '?' matches 'c', but the second letter is 'a', which does
 * not match 'b'.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= s.length, p.length <= 2000`
 *   * `s` contains only lowercase English letters.
 *   * `p` contains only lowercase English letters, `'?'` or `'*'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/wildcard-matching/
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
    bool isMatch(string s, string p) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aa"
    // Test case 2: "a"
    // Test case 3: "aa"
    // Test case 4: "*"
    // Test case 5: "cb"
    // Test case 6: "?a"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
