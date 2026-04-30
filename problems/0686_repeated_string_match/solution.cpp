// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 686: Repeated String Match
 * Difficulty: Medium
 * Tags: String, String Matching
 *
 * Problem Description:
 * Given two strings `a` and `b`, return _the minimum number of times you
 * should
 * repeat string_`a` _so that string_ `b` _is a substring of it_. If it is
 * impossible for `b`​​​​​​ to be a substring of `a` after repeating it, return
 * `-1`.
 * 
 * **Notice:** string `"abc"` repeated 0 times is `""`, repeated 1 time is
 * `"abc"`
 * and repeated 2 times is `"abcabc"`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** a = "abcd", b = "cdabcdab"
 *     **Output:** 3
 * **Explanation:** We return 3 because by repeating a three times
 * "ab**cdabcdab** cd", b is a substring of it.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** a = "a", b = "aa"
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= a.length, b.length <= 104`
 *   * `a` and `b` consist of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/repeated-string-match/
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
    int repeatedStringMatch(string a, string b) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abcd"
    // Test case 2: "cdabcdab"
    // Test case 3: "a"
    // Test case 4: "aa"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
