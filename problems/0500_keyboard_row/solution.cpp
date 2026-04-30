// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 500: Keyboard Row
 * Difficulty: Easy
 * Tags: Array, Hash Table, String
 *
 * Problem Description:
 * Given an array of strings `words`, return _the words that can be typed using
 * letters of the alphabet on only one row of American keyboard like the image
 * below_.
 * 
 * **Note** that the strings are **case-insensitive** , both lowercased and
 * uppercased of the same letter are treated as if they are at the same row.
 * 
 * In the **American keyboard** :
 * 
 *   * the first row consists of the characters `"qwertyuiop"`,
 *   * the second row consists of the characters `"asdfghjkl"`, and
 *   * the third row consists of the characters `"zxcvbnm"`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** words = ["Hello","Alaska","Dad","Peace"]
 * 
 * **Output:** ["Alaska","Dad"]
 * 
 * **Explanation:**
 * 
 * Both `"a"` and `"A"` are in the 2nd row of the American keyboard due to case
 * insensitivity.
 * 
 * **Example 2:**
 * 
 * **Input:** words = ["omk"]
 * 
 * **Output:** []
 * 
 * **Example 3:**
 * 
 * **Input:** words = ["adsdf","sfd"]
 * 
 * **Output:** ["adsdf","sfd"]
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words.length <= 20`
 *   * `1 <= words[i].length <= 100`
 *   * `words[i]` consists of English letters (both lowercase and uppercase). 
 * 
 * 
 *
 * Link: https://leetcode.com/problems/keyboard-row/
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
    vector<string> findWords(vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["Hello","Alaska","Dad","Peace"]
    // Test case 2: ["omk"]
    // Test case 3: ["adsdf","sfd"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
