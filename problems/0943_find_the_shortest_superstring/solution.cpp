// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 943: Find the Shortest Superstring
 * Difficulty: Hard
 * Tags: Bit Manipulation, Array, String, Dynamic Programming, Bitmask
 *
 * Problem Description:
 * Given an array of strings `words`, return _the smallest string that contains
 * each string in_ `words` _as a substring_. If there are multiple valid
 * strings of
 * the smallest length, return **any of them**.
 * 
 * You may assume that no string in `words` is a substring of another string in
 * `words`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** words = ["alex","loves","leetcode"]
 *     **Output:** "alexlovesleetcode"
 * **Explanation:** All permutations of "alex","loves","leetcode" would also
 * be accepted.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** words = ["catg","ctaagt","gcta","ttca","atgcatc"]
 *     **Output:** "gctaagttcatgcatc"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words.length <= 12`
 *   * `1 <= words[i].length <= 20`
 *   * `words[i]` consists of lowercase English letters.
 *   * All the strings of `words` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-the-shortest-superstring/
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
    string shortestSuperstring(vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["alex","loves","leetcode"]
    // Test case 2: ["catg","ctaagt","gcta","ttca","atgcatc"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
