// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 76: Minimum Window Substring
 * Difficulty: Hard
 * Tags: Hash Table, String, Sliding Window
 *
 * Problem Description:
 * Given two strings `s` and `t` of lengths `m` and `n` respectively, return
 * _the**minimum window**_ **_substring_** _of_`s` _such that every character
 * in_`t` _(**including duplicates**) is included in the window_. If there is
 * no
 * such substring, return _the empty string_`""`.
 * 
 * The testcases will be generated such that the answer is **unique**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "ADOBECODEBANC", t = "ABC"
 *     **Output:** "BANC"
 * **Explanation:** The minimum window substring "BANC" includes 'A', 'B', and
 * 'C' from string t.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "a", t = "a"
 *     **Output:** "a"
 *     **Explanation:** The entire string s is the minimum window.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "a", t = "aa"
 *     **Output:** ""
 *     **Explanation:** Both 'a's from t must be included in the window.
 *     Since the largest window of s only has one 'a', return empty string.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `m == s.length`
 *   * `n == t.length`
 *   * `1 <= m, n <= 105`
 *   * `s` and `t` consist of uppercase and lowercase English letters.
 * 
 * 
 * 
 * **Follow up:** Could you find an algorithm that runs in `O(m + n)` time?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-window-substring/
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
    string minWindow(string s, string t) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "ADOBECODEBANC"
    // Test case 2: "ABC"
    // Test case 3: "a"
    // Test case 4: "a"
    // Test case 5: "a"
    // Test case 6: "aa"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
