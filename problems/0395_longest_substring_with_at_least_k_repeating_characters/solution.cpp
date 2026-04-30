// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 395: Longest Substring with At Least K Repeating Characters
 * Difficulty: Medium
 * Tags: Hash Table, String, Divide and Conquer, Sliding Window
 *
 * Problem Description:
 * Given a string `s` and an integer `k`, return _the length of the longest
 * substring of_ `s` _such that the frequency of each character in this
 * substring
 * is greater than or equal to_ `k`.
 * 
 * if no such substring exists, return 0.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "aaabb", k = 3
 *     **Output:** 3
 * **Explanation:** The longest substring is "aaa", as 'a' is repeated 3
 * times.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "ababbc", k = 2
 *     **Output:** 5
 * **Explanation:** The longest substring is "ababb", as 'a' is repeated 2
 * times and 'b' is repeated 3 times.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 104`
 *   * `s` consists of only lowercase English letters.
 *   * `1 <= k <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-substring-with-at-least-k-repeating-characters/
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
    int longestSubstring(string s, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aaabb"
    // Test case 2: 3
    // Test case 3: "ababbc"
    // Test case 4: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
