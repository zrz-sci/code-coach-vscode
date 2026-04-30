// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 2062: Count Vowel Substrings of a String
 * Difficulty: Easy
 * Tags: Hash Table, String
 *
 * Problem Description:
 * A **substring** is a contiguous (non-empty) sequence of characters within a
 * string.
 * 
 * A **vowel substring** is a substring that **only** consists of vowels
 * (`'a'`,
 * `'e'`, `'i'`, `'o'`, and `'u'`) and has **all five** vowels present in it.
 * 
 * Given a string `word`, return _the number of**vowel substrings** in_ `word`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** word = "aeiouu"
 *     **Output:** 2
 * **Explanation:** The vowel substrings of word are as follows (underlined):
 *     - "**_aeiou_** u"
 *     - "**_aeiouu_** "
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** word = "unicornarihan"
 *     **Output:** 0
 * **Explanation:** Not all 5 vowels are present, so there are no vowel
 * substrings.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** word = "cuaieuouac"
 *     **Output:** 7
 * **Explanation:** The vowel substrings of word are as follows (underlined):
 *     - "c** _uaieuo_** uac"
 *     - "c** _uaieuou_** ac"
 *     - "c** _uaieuoua_** c"
 *     - "cu** _aieuo_** uac"
 *     - "cu** _aieuou_** ac"
 *     - "cu** _aieuoua_** c"
 *     - "cua** _ieuoua_** c"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= word.length <= 100`
 *   * `word` consists of lowercase English letters only.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-vowel-substrings-of-a-string/
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
    int countVowelSubstrings(string word) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aeiouu"
    // Test case 2: "unicornarihan"
    // Test case 3: "cuaieuouac"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
