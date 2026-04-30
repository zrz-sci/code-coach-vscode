// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 524: Longest Word in Dictionary through Deleting
 * Difficulty: Medium
 * Tags: Array, Two Pointers, String, Sorting
 *
 * Problem Description:
 * Given a string `s` and a string array `dictionary`, return _the longest
 * string
 * in the dictionary that can be formed by deleting some of the given string
 * characters_. If there is more than one possible result, return the longest
 * word
 * with the smallest lexicographical order. If there is no possible result,
 * return
 * the empty string.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abpcplea", dictionary = ["ale","apple","monkey","plea"]
 *     **Output:** "apple"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "abpcplea", dictionary = ["a","b","c"]
 *     **Output:** "a"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 1000`
 *   * `1 <= dictionary.length <= 1000`
 *   * `1 <= dictionary[i].length <= 1000`
 *   * `s` and `dictionary[i]` consist of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/longest-word-in-dictionary-through-deleting/
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
    string findLongestWord(string s, vector<string>& dictionary) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abpcplea"
    // Test case 2: ["ale","apple","monkey","plea"]
    // Test case 3: "abpcplea"
    // Test case 4: ["a","b","c"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
