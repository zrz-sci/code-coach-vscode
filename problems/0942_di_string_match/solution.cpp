// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 942: DI String Match
 * Difficulty: Easy
 * Tags: Greedy, Array, Two Pointers, String
 *
 * Problem Description:
 * A permutation `perm` of `n + 1` integers of all the integers in the range
 * `[0,
 * n]` can be represented as a string `s` of length `n` where:
 * 
 *   * `s[i] == 'I'` if `perm[i] < perm[i + 1]`, and
 *   * `s[i] == 'D'` if `perm[i] > perm[i + 1]`.
 * 
 * Given a string `s`, reconstruct the permutation `perm` and return it. If
 * there
 * are multiple valid permutations perm, return **any of them**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "IDID"
 *     **Output:** [0,4,1,3,2]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "III"
 *     **Output:** [0,1,2,3]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "DDI"
 *     **Output:** [3,2,0,1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `s[i]` is either `'I'` or `'D'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/di-string-match/
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
    vector<int> diStringMatch(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "IDID"
    // Test case 2: "III"
    // Test case 3: "DDI"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
