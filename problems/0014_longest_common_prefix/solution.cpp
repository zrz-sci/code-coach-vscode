// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 14: Longest Common Prefix
 * Difficulty: Easy
 * Tags: Trie, Array, String
 *
 * Problem Description:
 * Write a function to find the longest common prefix string amongst an array
 * of
 * strings.
 * 
 * If there is no common prefix, return an empty string `""`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** strs = ["flower","flow","flight"]
 *     **Output:** "fl"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** strs = ["dog","racecar","car"]
 *     **Output:** ""
 *     **Explanation:** There is no common prefix among the input strings.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= strs.length <= 200`
 *   * `0 <= strs[i].length <= 200`
 *   * `strs[i]` consists of only lowercase English letters if it is non-empty.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-common-prefix/
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
    string longestCommonPrefix(vector<string>& strs) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["flower","flow","flight"]
    // Test case 2: ["dog","racecar","car"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
