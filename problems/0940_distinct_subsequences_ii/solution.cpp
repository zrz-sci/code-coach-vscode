// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 940: Distinct Subsequences II
 * Difficulty: Hard
 * Tags: String, Dynamic Programming
 *
 * Problem Description:
 * Given a string s, return _the number of**distinct non-empty subsequences**
 * of_
 * `s`. Since the answer may be very large, return it **modulo** `109 + 7`.
 * 
 * A **subsequence** of a string is a new string that is formed from the
 * original
 * string by deleting some (can be none) of the characters without disturbing
 * the
 * relative positions of the remaining characters. (i.e., `"ace"` is a
 * subsequence
 * of `"_a_ b _c_ d _e_ "` while `"aec"` is not.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abc"
 *     **Output:** 7
 * **Explanation:** The 7 distinct subsequences are "a", "b", "c", "ab", "ac",
 * "bc", and "abc".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "aba"
 *     **Output:** 6
 * **Explanation:** The 6 distinct subsequences are "a", "b", "ab", "aa",
 * "ba", and "aba".
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "aaa"
 *     **Output:** 3
 *     **Explanation:** The 3 distinct subsequences are "a", "aa" and "aaa".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 2000`
 *   * `s` consists of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/distinct-subsequences-ii/
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
    int distinctSubseqII(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abc"
    // Test case 2: "aba"
    // Test case 3: "aaa"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
