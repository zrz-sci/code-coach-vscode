// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 583: Delete Operation for Two Strings
 * Difficulty: Medium
 * Tags: String, Dynamic Programming
 *
 * Problem Description:
 * Given two strings `word1` and `word2`, return _the minimum number
 * of**steps**
 * required to make_ `word1` _and_ `word2` _the same_.
 * 
 * In one **step** , you can delete exactly one character in either string.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** word1 = "sea", word2 = "eat"
 *     **Output:** 2
 * **Explanation:** You need one step to make "sea" to "ea" and another step
 * to make "eat" to "ea".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** word1 = "leetcode", word2 = "etco"
 *     **Output:** 4
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= word1.length, word2.length <= 500`
 *   * `word1` and `word2` consist of only lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/delete-operation-for-two-strings/
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
    // Test case 1: "sea"
    // Test case 2: "eat"
    // Test case 3: "leetcode"
    // Test case 4: "etco"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
