// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 648: Replace Words
 * Difficulty: Medium
 * Tags: Trie, Array, Hash Table, String
 *
 * Problem Description:
 * In English, we have a concept called **root** , which can be followed by
 * some
 * other word to form another longer word - let's call this word
 * **derivative**.
 * For example, when the **root** `"help"` is followed by the word `"ful"`, we
 * can
 * form a derivative `"helpful"`.
 * 
 * Given a `dictionary` consisting of many **roots** and a `sentence`
 * consisting of
 * words separated by spaces, replace all the derivatives in the sentence with
 * the
 * **root** forming it. If a derivative can be replaced by more than one
 * **root** ,
 * replace it with the **root** that has **the shortest length**.
 * 
 * Return _the`sentence`_ after the replacement.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** dictionary = ["cat","bat","rat"], sentence = "the cattle was
 * rattled by the battery"
 *     **Output:** "the cat was rat by the bat"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** dictionary = ["a","b","c"], sentence = "aadsfasf absbs bbab
 * cadsfafs"
 *     **Output:** "a a b c"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= dictionary.length <= 1000`
 *   * `1 <= dictionary[i].length <= 100`
 *   * `dictionary[i]` consists of only lower-case letters.
 *   * `1 <= sentence.length <= 106`
 *   * `sentence` consists of only lower-case letters and spaces.
 *   * The number of words in `sentence` is in the range `[1, 1000]`
 *   * The length of each word in `sentence` is in the range `[1, 1000]`
 * * Every two consecutive words in `sentence` will be separated by exactly
 * one space.
 *   * `sentence` does not have leading or trailing spaces.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/replace-words/
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
    string replaceWords(vector<string>& dictionary, string sentence) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["cat","bat","rat"]
    // Test case 2: "the cattle was rattled by the battery"
    // Test case 3: ["a","b","c"]
    // Test case 4: "aadsfasf absbs bbab cadsfafs"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
