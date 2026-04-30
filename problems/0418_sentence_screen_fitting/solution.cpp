// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 418: Sentence Screen Fitting
 * Difficulty: Medium
 * Tags: Array, String, Dynamic Programming
 *
 * Problem Description:
 * Given a `rows x cols` screen and a `sentence` represented as a list of
 * strings,
 * return _the number of times the given sentence can be fitted on the
 * screen_.
 * 
 * The order of words in the sentence must remain unchanged, and a word cannot
 * be
 * split into two lines. A single space must separate two consecutive words in
 * a
 * line.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** sentence = ["hello","world"], rows = 2, cols = 8
 *     **Output:** 1
 *     **Explanation:**
 *     hello---
 *     world---
 *     The character '-' signifies an empty space on the screen.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** sentence = ["a", "bcd", "e"], rows = 3, cols = 6
 *     **Output:** 2
 *     **Explanation:**
 *     a-bcd- 
 *     e-a---
 *     bcd-e-
 *     The character '-' signifies an empty space on the screen.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** sentence = ["i","had","apple","pie"], rows = 4, cols = 5
 *     **Output:** 1
 *     **Explanation:**
 *     i-had
 *     apple
 *     pie-i
 *     had--
 *     The character '-' signifies an empty space on the screen.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= sentence.length <= 100`
 *   * `1 <= sentence[i].length <= 10`
 *   * `sentence[i]` consists of lowercase English letters.
 *   * `1 <= rows, cols <= 2 * 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sentence-screen-fitting/
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
    int wordsTyping(vector<string>& sentence, int rows, int cols) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["hello","world"]
    // Test case 2: 2
    // Test case 3: 8
    // Test case 4: ["a", "bcd", "e"]
    // Test case 5: 3
    // Test case 6: 6
    // Test case 7: ["i","had","apple","pie"]
    // Test case 8: 4
    // Test case 9: 5

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
