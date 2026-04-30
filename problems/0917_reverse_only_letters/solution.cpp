// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 917: Reverse Only Letters
 * Difficulty: Easy
 * Tags: Two Pointers, String
 *
 * Problem Description:
 * Given a string `s`, reverse the string according to the following rules:
 * 
 * * All the characters that are not English letters remain in the same
 * position.
 *   * All the English letters (lowercase or uppercase) should be reversed.
 * 
 * Return `s` _after reversing it_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "ab-cd"
 *     **Output:** "dc-ba"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "a-bC-dEf-ghIj"
 *     **Output:** "j-Ih-gfE-dCba"
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "Test1ng-Leet=code-Q!"
 *     **Output:** "Qedo1ct-eeLg=ntse-T!"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 100`
 *   * `s` consists of characters with ASCII values in the range `[33, 122]`.
 *   * `s` does not contain `'\"'` or `'\\'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reverse-only-letters/
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
    string reverseOnlyLetters(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "ab-cd"
    // Test case 2: "a-bC-dEf-ghIj"
    // Test case 3: "Test1ng-Leet=code-Q!"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
