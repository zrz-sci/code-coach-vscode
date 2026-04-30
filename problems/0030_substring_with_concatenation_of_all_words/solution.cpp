// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 30: Substring with Concatenation of All Words
 * Difficulty: Hard
 * Tags: Hash Table, String, Sliding Window
 *
 * Problem Description:
 * You are given a string `s` and an array of strings `words`. All the strings
 * of
 * `words` are of **the same length**.
 * 
 * A **concatenated string** is a string that exactly contains all the strings
 * of
 * any permutation of `words` concatenated.
 * 
 * * For example, if `words = ["ab","cd","ef"]`, then `"abcdef"`, `"abefcd"`,
 * `"cdabef"`, `"cdefab"`, `"efabcd"`, and `"efcdab"` are all concatenated
 * strings. `"acdbef"` is not a concatenated string because it is not the
 * concatenation of any permutation of `words`.
 * 
 * Return an array of _the starting indices_ of all the concatenated
 * substrings in
 * `s`. You can return the answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** s = "barfoothefoobarman", words = ["foo","bar"]
 * 
 * **Output:** [0,9]
 * 
 * **Explanation:**
 * 
 * The substring starting at 0 is `"barfoo"`. It is the concatenation of
 * `["bar","foo"]` which is a permutation of `words`.  
 * The substring starting at 9 is `"foobar"`. It is the concatenation of
 * `["foo","bar"]` which is a permutation of `words`.
 * 
 * **Example 2:**
 * 
 * **Input:** s = "wordgoodgoodgoodbestword", words =
 * ["word","good","best","word"]
 * 
 * **Output:** []
 * 
 * **Explanation:**
 * 
 * There is no concatenated substring.
 * 
 * **Example 3:**
 * 
 * **Input:** s = "barfoofoobarthefoobarman", words = ["bar","foo","the"]
 * 
 * **Output:** [6,9,12]
 * 
 * **Explanation:**
 * 
 * The substring starting at 6 is `"foobarthe"`. It is the concatenation of
 * `["foo","bar","the"]`.  
 * The substring starting at 9 is `"barthefoo"`. It is the concatenation of
 * `["bar","the","foo"]`.  
 * The substring starting at 12 is `"thefoobar"`. It is the concatenation of
 * `["the","foo","bar"]`.
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 104`
 *   * `1 <= words.length <= 5000`
 *   * `1 <= words[i].length <= 30`
 *   * `s` and `words[i]` consist of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/substring-with-concatenation-of-all-words/
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
    vector<int> findSubstring(string s, vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "barfoothefoobarman"
    // Test case 2: ["foo","bar"]
    // Test case 3: "wordgoodgoodgoodbestword"
    // Test case 4: ["word","good","best","word"]
    // Test case 5: "barfoofoobarthefoobarman"
    // Test case 6: ["bar","foo","the"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
