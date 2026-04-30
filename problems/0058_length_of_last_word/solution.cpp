// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 58: Length of Last Word
 * Difficulty: Easy
 * Tags: String
 *
 * Problem Description:
 * Given a string `s` consisting of words and spaces, return _the length of
 * the**last** word in the string._
 * 
 * A **word** is a maximal substring consisting of non-space characters only.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "Hello World"
 *     **Output:** 5
 *     **Explanation:** The last word is "World" with length 5.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "   fly me   to   the moon  "
 *     **Output:** 4
 *     **Explanation:** The last word is "moon" with length 4.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "luffy is still joyboy"
 *     **Output:** 6
 *     **Explanation:** The last word is "joyboy" with length 6.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 104`
 *   * `s` consists of only English letters and spaces `' '`.
 *   * There will be at least one word in `s`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/length-of-last-word/
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
    int lengthOfLastWord(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "Hello World"
    // Test case 2: "   fly me   to   the moon  "
    // Test case 3: "luffy is still joyboy"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
