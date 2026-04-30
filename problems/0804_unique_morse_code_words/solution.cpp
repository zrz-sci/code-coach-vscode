// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 804: Unique Morse Code Words
 * Difficulty: Easy
 * Tags: Array, Hash Table, String
 *
 * Problem Description:
 * International Morse Code defines a standard encoding where each letter is
 * mapped
 * to a series of dots and dashes, as follows:
 * 
 *   * `'a'` maps to `".-"`,
 *   * `'b'` maps to `"-..."`,
 *   * `'c'` maps to `"-.-."`, and so on.
 * 
 * For convenience, the full table for the `26` letters of the English
 * alphabet is
 * given below:
 * 
 *     
 *     
 * [".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."]
 * 
 * Given an array of strings `words` where each word can be written as a
 * concatenation of the Morse code of each letter.
 * 
 * * For example, `"cab"` can be written as `"-.-..--..."`, which is the
 * concatenation of `"-.-."`, `".-"`, and `"-..."`. We will call such a
 * concatenation the **transformation** of a word.
 * 
 * Return _the number of different**transformations** among all words we have_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** words = ["gin","zen","gig","msg"]
 *     **Output:** 2
 *     **Explanation:** The transformation of each word is:
 *     "gin" -> "--...-."
 *     "zen" -> "--...-."
 *     "gig" -> "--...--."
 *     "msg" -> "--...--."
 *     There are 2 different transformations: "--...-." and "--...--.".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** words = ["a"]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words.length <= 100`
 *   * `1 <= words[i].length <= 12`
 *   * `words[i]` consists of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/unique-morse-code-words/
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
    int uniqueMorseRepresentations(vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["gin","zen","gig","msg"]
    // Test case 2: ["a"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
