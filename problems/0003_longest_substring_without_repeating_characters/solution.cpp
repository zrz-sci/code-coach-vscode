// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 3: Longest Substring Without Repeating Characters
 * Difficulty: Medium
 * Tags: Hash Table, String, Sliding Window
 *
 * Problem Description:
 * Given a string `s`, find the length of the **longest** **substring** without
 * duplicate characters.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abcabcbb"
 *     **Output:** 3
 * **Explanation:** The answer is "abc", with the length of 3. Note that "bca"
 * and "cab" are also correct answers.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "bbbbb"
 *     **Output:** 1
 *     **Explanation:** The answer is "b", with the length of 1.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "pwwkew"
 *     **Output:** 3
 *     **Explanation:** The answer is "wke", with the length of 3.
 * Notice that the answer must be a substring, "pwke" is a subsequence and not
 * a substring.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= s.length <= 5 * 104`
 *   * `s` consists of English letters, digits, symbols and spaces.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-substring-without-repeating-characters/
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
    int lengthOfLongestSubstring(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abcabcbb"
    // Test case 2: "bbbbb"
    // Test case 3: "pwwkew"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
