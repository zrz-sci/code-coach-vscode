// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 824: Goat Latin
 * Difficulty: Easy
 * Tags: String
 *
 * Problem Description:
 * You are given a string `sentence` that consist of words separated by spaces.
 * Each word consists of lowercase and uppercase letters only.
 * 
 * We would like to convert the sentence to "Goat Latin" (a made-up language
 * similar to Pig Latin.) The rules of Goat Latin are as follows:
 * 
 * * If a word begins with a vowel (`'a'`, `'e'`, `'i'`, `'o'`, or `'u'`),
 * append `"ma"` to the end of the word.
 *     * For example, the word `"apple"` becomes `"applema"`.
 * * If a word begins with a consonant (i.e., not a vowel), remove the first
 * letter and append it to the end, then add `"ma"`.
 *     * For example, the word `"goat"` becomes `"oatgma"`.
 * * Add one letter `'a'` to the end of each word per its word index in the
 * sentence, starting with `1`.
 * * For example, the first word gets `"a"` added to the end, the second word
 * gets `"aa"` added to the end, and so on.
 * 
 * Return _the final sentence representing the conversion from sentence to Goat
 * Latin_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** sentence = "I speak Goat Latin"
 *     **Output:** "Imaa peaksmaaa oatGmaaaa atinLmaaaaa"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** sentence = "The quick brown fox jumped over the lazy dog"
 * **Output:** "heTmaa uickqmaaa rownbmaaaa oxfmaaaaa umpedjmaaaaaa
 * overmaaaaaaa hetmaaaaaaaa azylmaaaaaaaaa ogdmaaaaaaaaaa"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= sentence.length <= 150`
 *   * `sentence` consists of English letters and spaces.
 *   * `sentence` has no leading or trailing spaces.
 *   * All the words in `sentence` are separated by a single space.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/goat-latin/
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
    string toGoatLatin(string sentence) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "I speak Goat Latin"
    // Test case 2: "The quick brown fox jumped over the lazy dog"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
