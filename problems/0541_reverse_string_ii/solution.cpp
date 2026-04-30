// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 541: Reverse String II
 * Difficulty: Easy
 * Tags: Two Pointers, String
 *
 * Problem Description:
 * Given a string `s` and an integer `k`, reverse the first `k` characters for
 * every `2k` characters counting from the start of the string.
 * 
 * If there are fewer than `k` characters left, reverse all of them. If there
 * are
 * less than `2k` but greater than or equal to `k` characters, then reverse the
 * first `k` characters and leave the other as original.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abcdefg", k = 2
 *     **Output:** "bacdfeg"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "abcd", k = 2
 *     **Output:** "bacd"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 104`
 *   * `s` consists of only lowercase English letters.
 *   * `1 <= k <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reverse-string-ii/
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
    string reverseStr(string s, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abcdefg"
    // Test case 2: 2
    // Test case 3: "abcd"
    // Test case 4: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
