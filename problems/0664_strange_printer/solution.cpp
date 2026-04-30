// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 664: Strange Printer
 * Difficulty: Hard
 * Tags: String, Dynamic Programming
 *
 * Problem Description:
 * There is a strange printer with the following two special properties:
 * 
 * * The printer can only print a sequence of **the same character** each
 * time.
 * * At each turn, the printer can print new characters starting from and
 * ending at any place and will cover the original existing characters.
 * 
 * Given a string `s`, return _the minimum number of turns the printer needed
 * to
 * print it_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "aaabbb"
 *     **Output:** 2
 *     **Explanation:** Print "aaa" first and then print "bbb".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "aba"
 *     **Output:** 2
 * **Explanation:** Print "aaa" first and then print "b" from the second place
 * of the string, which will cover the existing character 'a'.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 100`
 *   * `s` consists of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/strange-printer/
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
    int strangePrinter(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aaabbb"
    // Test case 2: "aba"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
