// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 151: Reverse Words in a String
 * Difficulty: Medium
 * Tags: Two Pointers, String
 *
 * Problem Description:
 * Given an input string `s`, reverse the order of the **words**.
 * 
 * A **word** is defined as a sequence of non-space characters. The **words**
 * in
 * `s` will be separated by at least one space.
 * 
 * Return _a string of the words in reverse order concatenated by a single
 * space._
 * 
 * **Note** that `s` may contain leading or trailing spaces or multiple spaces
 * between two words. The returned string should only have a single space
 * separating the words. Do not include any extra spaces.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "the sky is blue"
 *     **Output:** "blue is sky the"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "  hello world  "
 *     **Output:** "world hello"
 * **Explanation:** Your reversed string should not contain leading or
 * trailing spaces.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "a good   example"
 *     **Output:** "example good a"
 * **Explanation:** You need to reduce multiple spaces between two words to a
 * single space in the reversed string.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 104`
 * * `s` contains English letters (upper-case and lower-case), digits, and
 * spaces `' '`.
 *   * There is **at least one** word in `s`.
 * 
 * 
 * 
 * **Follow-up:  **If the string data type is mutable in your language, can you
 * solve it **in-place**  with `O(1)` extra space?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reverse-words-in-a-string/
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
    // Test case 1: "the sky is blue"
    // Test case 2: "  hello world  "
    // Test case 3: "a good   example"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
