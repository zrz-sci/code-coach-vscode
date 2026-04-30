// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1963: Minimum Number of Swaps to Make the String Balanced
 * Difficulty: Medium
 * Tags: Stack, Greedy, Two Pointers, String
 *
 * Problem Description:
 * You are given a **0-indexed** string `s` of **even** length `n`. The string
 * consists of **exactly** `n / 2` opening brackets `'['` and `n / 2` closing
 * brackets `']'`.
 * 
 * A string is called **balanced** if and only if:
 * 
 *   * It is the empty string, or
 * * It can be written as `AB`, where both `A` and `B` are **balanced**
 * strings, or
 *   * It can be written as `[C]`, where `C` is a **balanced** string.
 * 
 * You may swap the brackets at **any** two indices **any** number of times.
 * 
 * Return _the**minimum** number of swaps to make _`s` _**balanced**_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "][]["
 *     **Output:** 1
 * **Explanation:** You can make the string balanced by swapping index 0 with
 * index 3.
 *     The resulting string is "[[]]".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "]]][[["
 *     **Output:** 2
 *     **Explanation:** You can do the following to make the string balanced:
 *     - Swap index 0 with index 4. s = "[]][][".
 *     - Swap index 1 with index 5. s = "[[][]]".
 *     The resulting string is "[[][]]".
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "[]"
 *     **Output:** 0
 *     **Explanation:** The string is already balanced.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == s.length`
 *   * `2 <= n <= 106`
 *   * `n` is even.
 *   * `s[i]` is either `'[' `or `']'`.
 * * The number of opening brackets `'['` equals `n / 2`, and the number of
 * closing brackets `']'` equals `n / 2`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-number-of-swaps-to-make-the-string-balanced/
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
    int minSwaps(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "][]["
    // Test case 2: "]]][[["
    // Test case 3: "[]"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
