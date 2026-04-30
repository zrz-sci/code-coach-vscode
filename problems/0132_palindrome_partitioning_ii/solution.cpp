// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 132: Palindrome Partitioning II
 * Difficulty: Hard
 * Tags: String, Dynamic Programming
 *
 * Problem Description:
 * Given a string `s`, partition `s` such that every substring of the
 * partition is
 * a palindrome.
 * 
 * Return _the**minimum** cuts needed for a palindrome partitioning of_ `s`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "aab"
 *     **Output:** 1
 * **Explanation:** The palindrome partitioning ["aa","b"] could be produced
 * using 1 cut.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "a"
 *     **Output:** 0
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "ab"
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 2000`
 *   * `s` consists of lowercase English letters only.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/palindrome-partitioning-ii/
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
    int minCut(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aab"
    // Test case 2: "a"
    // Test case 3: "ab"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
