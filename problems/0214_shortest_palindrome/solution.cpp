// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 214: Shortest Palindrome
 * Difficulty: Hard
 * Tags: String, String Matching, Hash Function, Rolling Hash
 *
 * Problem Description:
 * You are given a string `s`. You can convert `s` to a palindrome by adding
 * characters in front of it.
 * 
 * Return _the shortest palindrome you can find by performing this
 * transformation_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "aacecaaa"
 *     **Output:** "aaacecaaa"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "abcd"
 *     **Output:** "dcbabcd"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= s.length <= 5 * 104`
 *   * `s` consists of lowercase English letters only.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shortest-palindrome/
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
    string shortestPalindrome(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aacecaaa"
    // Test case 2: "abcd"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
