// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 186: Reverse Words in a String II
 * Difficulty: Medium
 * Tags: Two Pointers, String
 *
 * Problem Description:
 * Given a character array `s`, reverse the order of the **words**.
 * 
 * A **word** is defined as a sequence of non-space characters. The **words**
 * in
 * `s` will be separated by a single space.
 * 
 * Your code must solve the problem **in-place,** i.e. without allocating extra
 * space.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** s = ["t","h","e"," ","s","k","y"," ","i","s","
 * ","b","l","u","e"]
 * **Output:** ["b","l","u","e"," ","i","s"," ","s","k","y"," ","t","h","e"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = ["a"]
 *     **Output:** ["a"]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 * * `s[i]` is an English letter (uppercase or lowercase), digit, or space `'
 * '`.
 *   * There is **at least one** word in `s`.
 *   * `s` does not contain leading or trailing spaces.
 *   * All the words in `s` are guaranteed to be separated by a single space.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reverse-words-in-a-string-ii/
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
    void reverseWords(vector<char>& s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["t","h","e"," ","s","k","y"," ","i","s"," ","b","l","u","e"]
    // Test case 2: ["a"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
