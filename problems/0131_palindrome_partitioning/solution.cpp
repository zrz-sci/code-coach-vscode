// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 131: Palindrome Partitioning
 * Difficulty: Medium
 * Tags: String, Dynamic Programming, Backtracking
 *
 * Problem Description:
 * Given a string `s`, partition `s` such that every substring of the
 * partition is
 * a **palindrome**. Return _all possible palindrome partitioning of_`s`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "aab"
 *     **Output:** [["a","a","b"],["aa","b"]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "a"
 *     **Output:** [["a"]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 16`
 *   * `s` contains only lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/palindrome-partitioning/
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
    vector<vector<string>> partition(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aab"
    // Test case 2: "a"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
