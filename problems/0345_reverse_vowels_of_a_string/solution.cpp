// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 345: Reverse Vowels of a String
 * Difficulty: Easy
 * Tags: Two Pointers, String
 *
 * Problem Description:
 * Given a string `s`, reverse only all the vowels in the string and return it.
 * 
 * The vowels are `'a'`, `'e'`, `'i'`, `'o'`, and `'u'`, and they can appear in
 * both lower and upper cases, more than once.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** s = "IceCreAm"
 * 
 * **Output:** "AceCreIm"
 * 
 * **Explanation:**
 * 
 * The vowels in `s` are `['I', 'e', 'e', 'A']`. On reversing the vowels, s
 * becomes
 * `"AceCreIm"`.
 * 
 * **Example 2:**
 * 
 * **Input:** s = "leetcode"
 * 
 * **Output:** "leotcede"
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 3 * 105`
 *   * `s` consist of **printable ASCII** characters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reverse-vowels-of-a-string/
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
    string reverseVowels(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "IceCreAm"
    // Test case 2: "leetcode"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
