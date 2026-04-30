// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 316: Remove Duplicate Letters
 * Difficulty: Medium
 * Tags: Stack, Greedy, String, Monotonic Stack
 *
 * Problem Description:
 * Given a string `s`, remove duplicate letters so that every letter appears
 * once
 * and only once. You must make sure your result is **the smallest in
 * lexicographical order** among all possible results.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "bcabc"
 *     **Output:** "abc"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "cbacdcbc"
 *     **Output:** "acdb"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 104`
 *   * `s` consists of lowercase English letters.
 * 
 * 
 * 
 * **Note:** This question is the same as 1081:
 * <https://leetcode.com/problems/smallest-subsequence-of-distinct-characters/>
 * 
 * 
 *
 * Link: https://leetcode.com/problems/remove-duplicate-letters/
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
    string removeDuplicateLetters(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "bcabc"
    // Test case 2: "cbacdcbc"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
