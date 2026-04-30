// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 484: Find Permutation
 * Difficulty: Medium
 * Tags: Stack, Greedy, Array, String
 *
 * Problem Description:
 * A permutation `perm` of `n` integers of all the integers in the range `[1,
 * n]`
 * can be represented as a string `s` of length `n - 1` where:
 * 
 *   * `s[i] == 'I'` if `perm[i] < perm[i + 1]`, and
 *   * `s[i] == 'D'` if `perm[i] > perm[i + 1]`.
 * 
 * Given a string `s`, reconstruct the lexicographically smallest permutation
 * `perm` and return it.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "I"
 *     **Output:** [1,2]
 * **Explanation:** [1,2] is the only legal permutation that can represented
 * by s, where the number 1 and 2 construct an increasing relationship.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "DI"
 *     **Output:** [2,1,3]
 * **Explanation:** Both [2,1,3] and [3,1,2] can be represented as "DI", but
 * since we want to find the smallest lexicographical permutation, you should
 * return [2,1,3]
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
 * Link: https://leetcode.com/problems/find-permutation/
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
    vector<int> findPermutation(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "I"
    // Test case 2: "DI"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
