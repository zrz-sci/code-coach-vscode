// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 916: Word Subsets
 * Difficulty: Medium
 * Tags: Array, Hash Table, String
 *
 * Problem Description:
 * You are given two string arrays `words1` and `words2`.
 * 
 * A string `b` is a **subset** of string `a` if every letter in `b` occurs in
 * `a`
 * including multiplicity.
 * 
 * * For example, `"wrr"` is a subset of `"warrior"` but is not a subset of
 * `"world"`.
 * 
 * A string `a` from `words1` is **universal** if for every string `b` in
 * `words2`,
 * `b` is a subset of `a`.
 * 
 * Return an array of all the **universal** strings in `words1`. You may
 * return the
 * answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** words1 = ["amazon","apple","facebook","google","leetcode"],
 * words2 =
 * ["e","o"]
 * 
 * **Output:** ["facebook","google","leetcode"]
 * 
 * **Example 2:**
 * 
 * **Input:** words1 = ["amazon","apple","facebook","google","leetcode"],
 * words2 =
 * ["lc","eo"]
 * 
 * **Output:** ["leetcode"]
 * 
 * **Example 3:**
 * 
 * **Input:** words1 = ["acaac","cccbb","aacbb","caacc","bcbbb"], words2 =
 * ["c","cc","b"]
 * 
 * **Output:** ["cccbb"]
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words1.length, words2.length <= 104`
 *   * `1 <= words1[i].length, words2[i].length <= 10`
 *   * `words1[i]` and `words2[i]` consist only of lowercase English letters.
 *   * All the strings of `words1` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/word-subsets/
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
    vector<string> wordSubsets(vector<string>& words1, vector<string>& words2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["amazon","apple","facebook","google","leetcode"]
    // Test case 2: ["e","o"]
    // Test case 3: ["amazon","apple","facebook","google","leetcode"]
    // Test case 4: ["lc","eo"]
    // Test case 5: ["acaac","cccbb","aacbb","caacc","bcbbb"]
    // Test case 6: ["c","cc","b"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
