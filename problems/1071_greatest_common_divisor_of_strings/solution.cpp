// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1071: Greatest Common Divisor of Strings
 * Difficulty: Easy
 * Tags: Math, String
 *
 * Problem Description:
 * For two strings `s` and `t`, we say "`t` divides `s`" if and only if `s = t
 * + t
 * + t + ... + t + t` (i.e., `t` is concatenated with itself one or more
 * times).
 * 
 * Given two strings `str1` and `str2`, return _the largest string_`x` _such
 * that_`x` _divides both_`str1` _and_`str2`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** str1 = "ABCABC", str2 = "ABC"
 * 
 * **Output:** "ABC"
 * 
 * **Example 2:**
 * 
 * **Input:** str1 = "ABABAB", str2 = "ABAB"
 * 
 * **Output:** "AB"
 * 
 * **Example 3:**
 * 
 * **Input:** str1 = "LEET", str2 = "CODE"
 * 
 * **Output:** ""
 * 
 * **Example 4:**
 * 
 * **Input:** str1 = "AAAAAB", str2 = "AAA"
 * 
 * **Output:** ""​​​​​​​
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= str1.length, str2.length <= 1000`
 *   * `str1` and `str2` consist of English uppercase letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/greatest-common-divisor-of-strings/
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
    string gcdOfStrings(string str1, string str2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "ABCABC"
    // Test case 2: "ABC"
    // Test case 3: "ABABAB"
    // Test case 4: "ABAB"
    // Test case 5: "LEET"
    // Test case 6: "CODE"
    // Test case 7: "AAAAAB"
    // Test case 8: "AAA"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
