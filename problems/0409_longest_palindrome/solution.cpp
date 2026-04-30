// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 409: Longest Palindrome
 * Difficulty: Easy
 * Tags: Greedy, Hash Table, String
 *
 * Problem Description:
 * Given a string `s` which consists of lowercase or uppercase letters, return
 * the
 * length of the **longest palindrome** that can be built with those letters.
 * 
 * Letters are **case sensitive** , for example, `"Aa"` is not considered a
 * palindrome.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abccccdd"
 *     **Output:** 7
 * **Explanation:** One longest palindrome that can be built is "dccaccd",
 * whose length is 7.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "a"
 *     **Output:** 1
 * **Explanation:** The longest palindrome that can be built is "a", whose
 * length is 1.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 2000`
 *   * `s` consists of lowercase **and/or** uppercase English letters only.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-palindrome/
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
    int longestPalindrome(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abccccdd"
    // Test case 2: "a"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
