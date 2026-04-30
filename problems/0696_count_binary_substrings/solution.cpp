// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 696: Count Binary Substrings
 * Difficulty: Easy
 * Tags: Two Pointers, String
 *
 * Problem Description:
 * Given a binary string `s`, return the number of non-empty substrings that
 * have
 * the same number of `0`'s and `1`'s, and all the `0`'s and all the `1`'s in
 * these
 * substrings are grouped consecutively.
 * 
 * Substrings that occur multiple times are counted the number of times they
 * occur.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "00110011"
 *     **Output:** 6
 * **Explanation:** There are 6 substrings that have equal number of
 * consecutive 1's and 0's: "0011", "01", "1100", "10", "0011", and "01".
 * Notice that some of these substrings repeat and are counted the number of
 * times they occur.
 * Also, "00110011" is not a valid substring because all the 0's (and 1's) are
 * not grouped together.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "10101"
 *     **Output:** 4
 * **Explanation:** There are 4 substrings: "10", "01", "10", "01" that have
 * equal number of consecutive 1's and 0's.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `s[i]` is either `'0'` or `'1'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-binary-substrings/
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
    int countBinarySubstrings(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "00110011"
    // Test case 2: "10101"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
