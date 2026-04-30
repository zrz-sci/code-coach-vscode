// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 466: Count The Repetitions
 * Difficulty: Hard
 * Tags: Two Pointers, String, Dynamic Programming
 *
 * Problem Description:
 * We define `str = [s, n]` as the string `str` which consists of the string
 * `s`
 * concatenated `n` times.
 * 
 *   * For example, `str == ["abc", 3] =="abcabcabc"`.
 * 
 * We define that string `s1` can be obtained from string `s2` if we can remove
 * some characters from `s2` such that it becomes `s1`.
 * 
 * * For example, `s1 = "abc"` can be obtained from `s2 = "ab** _dbe_** c"`
 * based on our definition by removing the bolded underlined characters.
 * 
 * You are given two strings `s1` and `s2` and two integers `n1` and `n2`. You
 * have
 * the two strings `str1 = [s1, n1]` and `str2 = [s2, n2]`.
 * 
 * Return _the maximum integer_`m` _such that_`str = [str2, m]`_can be obtained
 * from_`str1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s1 = "acb", n1 = 4, s2 = "ab", n2 = 2
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s1 = "acb", n1 = 1, s2 = "acb", n2 = 1
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s1.length, s2.length <= 100`
 *   * `s1` and `s2` consist of lowercase English letters.
 *   * `1 <= n1, n2 <= 106`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-the-repetitions/
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
    int getMaxRepetitions(string s1, int n1, string s2, int n2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "acb"
    // Test case 2: 4
    // Test case 3: "ab"
    // Test case 4: 2
    // Test case 5: "acb"
    // Test case 6: 1
    // Test case 7: "acb"
    // Test case 8: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
