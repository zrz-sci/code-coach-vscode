// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 340: Longest Substring with At Most K Distinct Characters
 * Difficulty: Medium
 * Tags: Hash Table, String, Sliding Window
 *
 * Problem Description:
 * Given a string `s` and an integer `k`, return _the length of the longest_
 * _substring_ _of_ `s` _that contains at most_ `k` _**distinct** characters_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "eceba", k = 2
 *     **Output:** 3
 *     **Explanation:** The substring is "ece" with length 3.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "aa", k = 1
 *     **Output:** 2
 *     **Explanation:** The substring is "aa" with length 2.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 5 * 104`
 *   * `0 <= k <= 50`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-substring-with-at-most-k-distinct-characters/
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
    int lengthOfLongestSubstringKDistinct(string s, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "eceba"
    // Test case 2: 2
    // Test case 3: "aa"
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
