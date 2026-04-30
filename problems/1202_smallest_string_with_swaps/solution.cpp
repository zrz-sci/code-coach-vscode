// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1202: Smallest String With Swaps
 * Difficulty: Medium
 * Tags: Depth-First Search, Breadth-First Search, Union Find, Array, Hash Table, String, Sorting
 *
 * Problem Description:
 * You are given a string `s`, and an array of pairs of indices in the string
 * `pairs` where `pairs[i] = [a, b]` indicates 2 indices(0-indexed) of the
 * string.
 * 
 * You can swap the characters at any pair of indices in the given `pairs`
 * **any
 * number of times**.
 * 
 * Return the lexicographically smallest string that `s` can be changed to
 * after
 * using the swaps.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "dcab", pairs = [[0,3],[1,2]]
 *     **Output:** "bacd"
 *     **Explaination:** 
 *     Swap s[0] and s[3], s = "bcad"
 *     Swap s[1] and s[2], s = "bacd"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "dcab", pairs = [[0,3],[1,2],[0,2]]
 *     **Output:** "abcd"
 *     **Explaination:**
 *     Swap s[0] and s[3], s = "bcad"
 *     Swap s[0] and s[2], s = "acbd"
 *     Swap s[1] and s[2], s = "abcd"
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "cba", pairs = [[0,1],[1,2]]
 *     **Output:** "abc"
 *     **Explaination:**
 *     Swap s[0] and s[1], s = "bca"
 *     Swap s[1] and s[2], s = "bac"
 *     Swap s[0] and s[1], s = "abc"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 10^5`
 *   * `0 <= pairs.length <= 10^5`
 *   * `0 <= pairs[i][0], pairs[i][1] < s.length`
 *   * `s` only contains lower case English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/smallest-string-with-swaps/
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
    string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "dcab"
    // Test case 2: [[0,3],[1,2]]
    // Test case 3: "dcab"
    // Test case 4: [[0,3],[1,2],[0,2]]
    // Test case 5: "cba"
    // Test case 6: [[0,1],[1,2]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
