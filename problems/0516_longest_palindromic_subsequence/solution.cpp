// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 516: Longest Palindromic Subsequence
 * Difficulty: Medium
 * Tags: String, Dynamic Programming
 *
 * Problem Description:
 * Given a string `s`, find _the longest palindromic**subsequence** 's length
 * in_
 * `s`.
 * 
 * A **subsequence** is a sequence that can be derived from another sequence by
 * deleting some or no elements without changing the order of the remaining
 * elements.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "bbbab"
 *     **Output:** 4
 *     **Explanation:** One possible longest palindromic subsequence is "bbbb".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "cbbd"
 *     **Output:** 2
 *     **Explanation:** One possible longest palindromic subsequence is "bb".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 1000`
 *   * `s` consists only of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-palindromic-subsequence/
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
    int longestPalindromeSubseq(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "bbbab"
    // Test case 2: "cbbd"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
