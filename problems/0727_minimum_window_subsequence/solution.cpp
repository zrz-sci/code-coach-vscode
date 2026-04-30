// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 727: Minimum Window Subsequence
 * Difficulty: Hard
 * Tags: String, Dynamic Programming, Sliding Window
 *
 * Problem Description:
 * Given strings `s1` and `s2`, return _the minimum contiguous substring part
 * of
 * _`s1` _, so that_`s2` _is a subsequence of the part_.
 * 
 * If there is no such window in `s1` that covers all characters in `s2`,
 * return
 * the empty string `""`. If there are multiple such minimum-length windows,
 * return
 * the one with the **left-most starting index**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s1 = "abcdebdde", s2 = "bde"
 *     **Output:** "bcde"
 *     **Explanation:** 
 * "bcde" is the answer because it occurs before "bdde" which has the same
 * length.
 * "deb" is not a smaller window because the elements of s2 in the window must
 * occur in order.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s1 = "jmeqksfrsdcmsiwvaovztaqenprpvnbstl", s2 = "u"
 *     **Output:** ""
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s1.length <= 2 * 104`
 *   * `1 <= s2.length <= 100`
 *   * `s1` and `s2` consist of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-window-subsequence/
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
    string minWindow(string s1, string s2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abcdebdde"
    // Test case 2: "bde"
    // Test case 3: "jmeqksfrsdcmsiwvaovztaqenprpvnbstl"
    // Test case 4: "u"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
