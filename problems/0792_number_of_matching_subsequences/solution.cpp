// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 792: Number of Matching Subsequences
 * Difficulty: Medium
 * Tags: Trie, Array, Hash Table, String, Binary Search, Dynamic Programming, Sorting
 *
 * Problem Description:
 * Given a string `s` and an array of strings `words`, return _the number of_
 * `words[i]` _that is a subsequence of_ `s`.
 * 
 * A **subsequence** of a string is a new string generated from the original
 * string
 * with some characters (can be none) deleted without changing the relative
 * order
 * of the remaining characters.
 * 
 *   * For example, `"ace"` is a subsequence of `"abcde"`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abcde", words = ["a","bb","acd","ace"]
 *     **Output:** 3
 * **Explanation:** There are three strings in words that are a subsequence of
 * s: "a", "acd", "ace".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** s = "dsahjpjauf", words =
 * ["ahjpjau","ja","ahbwzgqnuk","tnmlanowax"]
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 5 * 104`
 *   * `1 <= words.length <= 5000`
 *   * `1 <= words[i].length <= 50`
 *   * `s` and `words[i]` consist of only lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/number-of-matching-subsequences/
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
    int numMatchingSubseq(string s, vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abcde"
    // Test case 2: ["a","bb","acd","ace"]
    // Test case 3: "dsahjpjauf"
    // Test case 4: ["ahjpjau","ja","ahbwzgqnuk","tnmlanowax"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
