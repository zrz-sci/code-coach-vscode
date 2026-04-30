// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 139: Word Break
 * Difficulty: Medium
 * Tags: Trie, Memoization, Array, Hash Table, String, Dynamic Programming
 *
 * Problem Description:
 * Given a string `s` and a dictionary of strings `wordDict`, return `true` if
 * `s`
 * can be segmented into a space-separated sequence of one or more dictionary
 * words.
 * 
 * **Note** that the same word in the dictionary may be reused multiple times
 * in
 * the segmentation.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "leetcode", wordDict = ["leet","code"]
 *     **Output:** true
 * **Explanation:** Return true because "leetcode" can be segmented as "leet
 * code".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "applepenapple", wordDict = ["apple","pen"]
 *     **Output:** true
 * **Explanation:** Return true because "applepenapple" can be segmented as
 * "apple pen apple".
 *     Note that you are allowed to reuse a dictionary word.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "catsandog", wordDict = ["cats","dog","sand","and","cat"]
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 300`
 *   * `1 <= wordDict.length <= 1000`
 *   * `1 <= wordDict[i].length <= 20`
 *   * `s` and `wordDict[i]` consist of only lowercase English letters.
 *   * All the strings of `wordDict` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/word-break/
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
    bool wordBreak(string s, vector<string>& wordDict) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "leetcode"
    // Test case 2: ["leet","code"]
    // Test case 3: "applepenapple"
    // Test case 4: ["apple","pen"]
    // Test case 5: "catsandog"
    // Test case 6: ["cats","dog","sand","and","cat"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
