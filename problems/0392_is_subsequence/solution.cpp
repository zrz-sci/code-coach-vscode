// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 392: Is Subsequence
 * Difficulty: Easy
 * Tags: Two Pointers, String, Dynamic Programming
 *
 * Problem Description:
 * Given two strings `s` and `t`, return `true` _if_`s` _is a**subsequence** of
 * _`t` _, or_`false` _otherwise_.
 * 
 * A **subsequence** of a string is a new string that is formed from the
 * original
 * string by deleting some (can be none) of the characters without disturbing
 * the
 * relative positions of the remaining characters. (i.e., `"ace"` is a
 * subsequence
 * of `"_a_ b _c_ d _e_ "` while `"aec"` is not).
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abc", t = "ahbgdc"
 *     **Output:** true
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "axc", t = "ahbgdc"
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= s.length <= 100`
 *   * `0 <= t.length <= 104`
 *   * `s` and `t` consist only of lowercase English letters.
 * 
 * 
 * 
 * **Follow up:** Suppose there are lots of incoming `s`, say `s1, s2, ..., sk`
 * where `k >= 109`, and you want to check one by one to see if `t` has its
 * subsequence. In this scenario, how would you change your code?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/is-subsequence/
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
    bool isSubsequence(string s, string t) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abc"
    // Test case 2: "ahbgdc"
    // Test case 3: "axc"
    // Test case 4: "ahbgdc"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
