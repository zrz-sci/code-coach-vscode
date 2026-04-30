// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1456: Maximum Number of Vowels in a Substring of Given Length
 * Difficulty: Medium
 * Tags: String, Sliding Window
 *
 * Problem Description:
 * Given a string `s` and an integer `k`, return _the maximum number of vowel
 * letters in any substring of_`s` _with length_`k`.
 * 
 * **Vowel letters** in English are `'a'`, `'e'`, `'i'`, `'o'`, and `'u'`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abciiidef", k = 3
 *     **Output:** 3
 *     **Explanation:** The substring "iii" contains 3 vowel letters.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "aeiou", k = 2
 *     **Output:** 2
 *     **Explanation:** Any substring of length 2 contains 2 vowels.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "leetcode", k = 3
 *     **Output:** 2
 *     **Explanation:** "lee", "eet" and "ode" contain 2 vowels.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `s` consists of lowercase English letters.
 *   * `1 <= k <= s.length`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-number-of-vowels-in-a-substring-of-given-length/
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
    int maxVowels(string s, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abciiidef"
    // Test case 2: 3
    // Test case 3: "aeiou"
    // Test case 4: 2
    // Test case 5: "leetcode"
    // Test case 6: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
