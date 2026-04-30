// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 647: Palindromic Substrings
 * Difficulty: Medium
 * Tags: Two Pointers, String, Dynamic Programming
 *
 * Problem Description:
 * Given a string `s`, return _the number of**palindromic substrings** in it_.
 * 
 * A string is a **palindrome** when it reads the same backward as forward.
 * 
 * A **substring** is a contiguous sequence of characters within the string.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abc"
 *     **Output:** 3
 *     **Explanation:** Three palindromic strings: "a", "b", "c".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "aaa"
 *     **Output:** 6
 * **Explanation:** Six palindromic strings: "a", "a", "a", "aa", "aa", "aaa".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 1000`
 *   * `s` consists of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/palindromic-substrings/
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
    int countSubstrings(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abc"
    // Test case 2: "aaa"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
