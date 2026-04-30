// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 140: Word Break II
 * Difficulty: Hard
 * Tags: Trie, Memoization, Array, Hash Table, String, Dynamic Programming, Backtracking
 *
 * Problem Description:
 * Given a string `s` and a dictionary of strings `wordDict`, add spaces in
 * `s` to
 * construct a sentence where each word is a valid dictionary word. Return all
 * such
 * possible sentences in **any order**.
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
 * **Input:** s = "catsanddog", wordDict = ["cat","cats","and","sand","dog"]
 *     **Output:** ["cats and dog","cat sand dog"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** s = "pineapplepenapple", wordDict =
 * ["apple","pen","applepen","pine","pineapple"]
 * **Output:** ["pine apple pen apple","pineapple pen apple","pine applepen
 * apple"]
 *     **Explanation:** Note that you are allowed to reuse a dictionary word.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "catsandog", wordDict = ["cats","dog","sand","and","cat"]
 *     **Output:** []
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 20`
 *   * `1 <= wordDict.length <= 1000`
 *   * `1 <= wordDict[i].length <= 10`
 *   * `s` and `wordDict[i]` consist of only lowercase English letters.
 *   * All the strings of `wordDict` are **unique**.
 * * Input is generated in a way that the length of the answer doesn't exceed
 * 105.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/word-break-ii/
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
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "catsanddog"
    // Test case 2: ["cat","cats","and","sand","dog"]
    // Test case 3: "pineapplepenapple"
    // Test case 4: ["apple","pen","applepen","pine","pineapple"]
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
