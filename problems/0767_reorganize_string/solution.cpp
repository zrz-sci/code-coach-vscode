// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 767: Reorganize String
 * Difficulty: Medium
 * Tags: Greedy, Hash Table, String, Counting, Sorting, Heap (Priority Queue)
 *
 * Problem Description:
 * Given a string `s`, rearrange the characters of `s` so that any two adjacent
 * characters are not the same.
 * 
 * Return _any possible rearrangement of_ `s` _or return_ `""` _if not
 * possible_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "aab"
 *     **Output:** "aba"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "aaab"
 *     **Output:** ""
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 500`
 *   * `s` consists of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reorganize-string/
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
    string reorganizeString(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aab"
    // Test case 2: "aaab"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
