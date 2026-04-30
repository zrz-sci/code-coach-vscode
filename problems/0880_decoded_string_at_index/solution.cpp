// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 880: Decoded String at Index
 * Difficulty: Medium
 * Tags: Stack, String
 *
 * Problem Description:
 * You are given an encoded string `s`. To decode the string to a tape, the
 * encoded
 * string is read one character at a time and the following steps are taken:
 * 
 *   * If the character read is a letter, that letter is written onto the tape.
 * * If the character read is a digit `d`, the entire current tape is
 * repeatedly written `d - 1` more times in total.
 * 
 * Given an integer `k`, return _the_`kth` _letter (**1-indexed)** in the
 * decoded
 * string_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "leet2code3", k = 10
 *     **Output:** "o"
 * **Explanation:** The decoded string is
 * "leetleetcodeleetleetcodeleetleetcode".
 *     The 10th letter in the string is "o".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "ha22", k = 5
 *     **Output:** "h"
 *     **Explanation:** The decoded string is "hahahaha".
 *     The 5th letter is "h".
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "a2345678999999999999999", k = 1
 *     **Output:** "a"
 * **Explanation:** The decoded string is "a" repeated 8301530446056247680
 * times.
 *     The 1st letter is "a".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= s.length <= 100`
 *   * `s` consists of lowercase English letters and digits `2` through `9`.
 *   * `s` starts with a letter.
 *   * `1 <= k <= 109`
 * * It is guaranteed that `k` is less than or equal to the length of the
 * decoded string.
 *   * The decoded string is guaranteed to have less than `263` letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/decoded-string-at-index/
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
    string decodeAtIndex(string s, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "leet2code3"
    // Test case 2: 10
    // Test case 3: "ha22"
    // Test case 4: 5
    // Test case 5: "a2345678999999999999999"
    // Test case 6: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
