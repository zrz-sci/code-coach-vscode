// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 125: Valid Palindrome
 * Difficulty: Easy
 * Tags: Two Pointers, String
 *
 * Problem Description:
 * A phrase is a **palindrome** if, after converting all uppercase letters into
 * lowercase letters and removing all non-alphanumeric characters, it reads the
 * same forward and backward. Alphanumeric characters include letters and
 * numbers.
 * 
 * Given a string `s`, return `true` _if it is a**palindrome** , or _`false`
 * _otherwise_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "A man, a plan, a canal: Panama"
 *     **Output:** true
 *     **Explanation:** "amanaplanacanalpanama" is a palindrome.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "race a car"
 *     **Output:** false
 *     **Explanation:** "raceacar" is not a palindrome.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = " "
 *     **Output:** true
 * **Explanation:** s is an empty string "" after removing non-alphanumeric
 * characters.
 * Since an empty string reads the same forward and backward, it is a
 * palindrome.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 2 * 105`
 *   * `s` consists only of printable ASCII characters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/valid-palindrome/
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
    bool isPalindrome(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "A man, a plan, a canal: Panama"
    // Test case 2: "race a car"
    // Test case 3: " "

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
