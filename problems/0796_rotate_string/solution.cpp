// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 796: Rotate String
 * Difficulty: Easy
 * Tags: String, String Matching
 *
 * Problem Description:
 * Given two strings `s` and `goal`, return `true` _if and only if_ `s` _can
 * become_ `goal` _after some number of**shifts** on_ `s`.
 * 
 * A **shift** on `s` consists of moving the leftmost character of `s` to the
 * rightmost position.
 * 
 * * For example, if `s = "abcde"`, then it will be `"bcdea"` after one shift.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abcde", goal = "cdeab"
 *     **Output:** true
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "abcde", goal = "abced"
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length, goal.length <= 100`
 *   * `s` and `goal` consist of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/rotate-string/
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
    bool rotateString(string s, string goal) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abcde"
    // Test case 2: "cdeab"
    // Test case 3: "abcde"
    // Test case 4: "abced"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
