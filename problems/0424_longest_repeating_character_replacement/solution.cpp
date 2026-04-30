// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 424: Longest Repeating Character Replacement
 * Difficulty: Medium
 * Tags: Hash Table, String, Sliding Window
 *
 * Problem Description:
 * You are given a string `s` and an integer `k`. You can choose any character
 * of
 * the string and change it to any other uppercase English character. You can
 * perform this operation at most `k` times.
 * 
 * Return _the length of the longest substring containing the same letter you
 * can
 * get after performing the above operations_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "ABAB", k = 2
 *     **Output:** 4
 *     **Explanation:** Replace the two 'A's with two 'B's or vice versa.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "AABABBA", k = 1
 *     **Output:** 4
 * **Explanation:** Replace the one 'A' in the middle with 'B' and form
 * "AABBBBA".
 *     The substring "BBBB" has the longest repeating letters, which is 4.
 *     There may exists other ways to achieve this answer too.
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `s` consists of only uppercase English letters.
 *   * `0 <= k <= s.length`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-repeating-character-replacement/
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
    int characterReplacement(string s, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "ABAB"
    // Test case 2: 2
    // Test case 3: "AABABBA"
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
