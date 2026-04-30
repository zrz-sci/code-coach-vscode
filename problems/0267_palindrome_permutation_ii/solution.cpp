// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 267: Palindrome Permutation II
 * Difficulty: Medium
 * Tags: Hash Table, String, Backtracking
 *
 * Problem Description:
 * Given a string s, return _all the palindromic permutations (without
 * duplicates)
 * of it_.
 * 
 * You may return the answer in **any order**. If `s` has no palindromic
 * permutation, return an empty list.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "aabb"
 *     **Output:** ["abba","baab"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "abc"
 *     **Output:** []
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 16`
 *   * `s` consists of only lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/palindrome-permutation-ii/
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
    vector<string> generatePalindromes(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aabb"
    // Test case 2: "abc"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
