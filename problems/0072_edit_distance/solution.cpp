// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 72: Edit Distance
 * Difficulty: Medium
 * Tags: String, Dynamic Programming
 *
 * Problem Description:
 * Given two strings `word1` and `word2`, return _the minimum number of
 * operations
 * required to convert`word1` to `word2`_.
 * 
 * You have the following three operations permitted on a word:
 * 
 *   * Insert a character
 *   * Delete a character
 *   * Replace a character
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** word1 = "horse", word2 = "ros"
 *     **Output:** 3
 *     **Explanation:** 
 *     horse -> rorse (replace 'h' with 'r')
 *     rorse -> rose (remove 'r')
 *     rose -> ros (remove 'e')
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** word1 = "intention", word2 = "execution"
 *     **Output:** 5
 *     **Explanation:** 
 *     intention -> inention (remove 't')
 *     inention -> enention (replace 'i' with 'e')
 *     enention -> exention (replace 'n' with 'x')
 *     exention -> exection (replace 'n' with 'c')
 *     exection -> execution (insert 'u')
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= word1.length, word2.length <= 500`
 *   * `word1` and `word2` consist of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/edit-distance/
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
    int minDistance(string word1, string word2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "horse"
    // Test case 2: "ros"
    // Test case 3: "intention"
    // Test case 4: "execution"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
