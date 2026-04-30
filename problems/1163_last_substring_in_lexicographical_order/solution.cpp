// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1163: Last Substring in Lexicographical Order
 * Difficulty: Hard
 * Tags: Two Pointers, String
 *
 * Problem Description:
 * Given a string `s`, return _the last substring of_ `s` _in lexicographical
 * order_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abab"
 *     **Output:** "bab"
 * **Explanation:** The substrings are ["a", "ab", "aba", "abab", "b", "ba",
 * "bab"]. The lexicographically maximum substring is "bab".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "leetcode"
 *     **Output:** "tcode"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 4 * 105`
 *   * `s` contains only lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/last-substring-in-lexicographical-order/
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
    string lastSubstring(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abab"
    // Test case 2: "leetcode"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
