// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 557: Reverse Words in a String III
 * Difficulty: Easy
 * Tags: Two Pointers, String
 *
 * Problem Description:
 * Given a string `s`, reverse the order of characters in each word within a
 * sentence while still preserving whitespace and initial word order.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "Let's take LeetCode contest"
 *     **Output:** "s'teL ekat edoCteeL tsetnoc"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "Mr Ding"
 *     **Output:** "rM gniD"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 5 * 104`
 *   * `s` contains printable **ASCII** characters.
 *   * `s` does not contain any leading or trailing spaces.
 *   * There is **at least one** word in `s`.
 *   * All the words in `s` are separated by a single space.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reverse-words-in-a-string-iii/
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
    string reverseWords(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "Let's take LeetCode contest"
    // Test case 2: "Mr Ding"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
