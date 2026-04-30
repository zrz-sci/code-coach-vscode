// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 616: Add Bold Tag in String
 * Difficulty: Medium
 * Tags: Trie, Array, Hash Table, String, String Matching
 *
 * Problem Description:
 * You are given a string `s` and an array of strings `words`.
 * 
 * You should add a closed pair of bold tag `<b>` and `</b>` to wrap the
 * substrings
 * in `s` that exist in `words`.
 * 
 * * If two such substrings overlap, you should wrap them together with only
 * one pair of closed bold-tag.
 * * If two substrings wrapped by bold tags are consecutive, you should
 * combine them.
 * 
 * Return `s` _after adding the bold tags_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abcxyz123", words = ["abc","123"]
 *     **Output:** "<b>abc</b>xyz<b>123</b>"
 * **Explanation:** The two strings of words are substrings of s as following:
 * "_abc_ xyz _123_ ".
 *     We add <b> before each substring and </b> after each substring.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "aaabbb", words = ["aa","b"]
 *     **Output:** "<b>aaabbb</b>"
 *     **Explanation:** 
 *     "aa" appears as a substring two times: "_aa_ abbb" and "a _aa_ bbb".
 * "b" appears as a substring three times: "aaa _b_ bb", "aaab _b_ b", and
 * "aaabb _b_ ".
 * We add <b> before each substring and </b> after each substring:
 * "<b>a<b>a</b>a</b><b>b</b><b>b</b><b>b</b>".
 * Since the first two <b>'s overlap, we merge them:
 * "<b>aaa</b><b>b</b><b>b</b><b>b</b>".
 * Since now the four <b>'s are consecutive, we merge them: "<b>aaabbb</b>".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 1000`
 *   * `0 <= words.length <= 100`
 *   * `1 <= words[i].length <= 1000`
 *   * `s` and `words[i]` consist of English letters and digits.
 *   * All the values of `words` are **unique**.
 * 
 * 
 * 
 * **Note:** This question is the same as [758\. Bold Words in
 * String](https://leetcode.com/problems/bold-words-in-string/description/).
 * 
 * 
 *
 * Link: https://leetcode.com/problems/add-bold-tag-in-string/
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
    string addBoldTag(string s, vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abcxyz123"
    // Test case 2: ["abc","123"]
    // Test case 3: "aaabbb"
    // Test case 4: ["aa","b"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
