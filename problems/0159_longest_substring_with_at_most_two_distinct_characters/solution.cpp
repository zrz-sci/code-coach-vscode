// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 159: Longest Substring with At Most Two Distinct Characters
 * Difficulty: Medium
 * Tags: Hash Table, String, Sliding Window
 *
 * Problem Description:
 * Given a string `s`, return _the length of the longest_ _substring_ _that
 * contains at most**two distinct characters**_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "eceba"
 *     **Output:** 3
 *     **Explanation:** The substring is "ece" which its length is 3.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "ccaabbb"
 *     **Output:** 5
 *     **Explanation:** The substring is "aabbb" which its length is 5.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `s` consists of English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-substring-with-at-most-two-distinct-characters/
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
    int lengthOfLongestSubstringTwoDistinct(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "eceba"
    // Test case 2: "ccaabbb"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
