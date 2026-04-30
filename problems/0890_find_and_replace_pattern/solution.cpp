// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 890: Find and Replace Pattern
 * Difficulty: Medium
 * Tags: Array, Hash Table, String
 *
 * Problem Description:
 * Given a list of strings `words` and a string `pattern`, return _a list of_
 * `words[i]` _that match_ `pattern`. You may return the answer in **any
 * order**.
 * 
 * A word matches the pattern if there exists a permutation of letters `p` so
 * that
 * after replacing every letter `x` in the pattern with `p(x)`, we get the
 * desired
 * word.
 * 
 * Recall that a permutation of letters is a bijection from letters to letters:
 * every letter maps to another letter, and no two letters map to the same
 * letter.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** words = ["abc","deq","mee","aqq","dkd","ccc"], pattern = "abb"
 *     **Output:** ["mee","aqq"]
 * **Explanation:** "mee" matches the pattern because there is a permutation
 * {a -> m, b -> e, ...}.
 * "ccc" does not match the pattern because {a -> c, b -> c, ...} is not a
 * permutation, since a and b map to the same letter.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** words = ["a","b","c"], pattern = "a"
 *     **Output:** ["a","b","c"]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= pattern.length <= 20`
 *   * `1 <= words.length <= 50`
 *   * `words[i].length == pattern.length`
 *   * `pattern` and `words[i]` are lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-and-replace-pattern/
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
    vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["abc","deq","mee","aqq","dkd","ccc"]
    // Test case 2: "abb"
    // Test case 3: ["a","b","c"]
    // Test case 4: "a"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
