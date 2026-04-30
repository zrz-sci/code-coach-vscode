// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 859: Buddy Strings
 * Difficulty: Easy
 * Tags: Hash Table, String
 *
 * Problem Description:
 * Given two strings `s` and `goal`, return `true` _if you can swap two letters
 * in_`s` _so the result is equal to_`goal` _, otherwise, return_`false` _._
 * 
 * Swapping letters is defined as taking two indices `i` and `j` (0-indexed)
 * such
 * that `i != j` and swapping the characters at `s[i]` and `s[j]`.
 * 
 * * For example, swapping at indices `0` and `2` in `"abcd"` results in
 * `"cbad"`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "ab", goal = "ba"
 *     **Output:** true
 * **Explanation:** You can swap s[0] = 'a' and s[1] = 'b' to get "ba", which
 * is equal to goal.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "ab", goal = "ab"
 *     **Output:** false
 * **Explanation:** The only letters you can swap are s[0] = 'a' and s[1] =
 * 'b', which results in "ba" != goal.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "aa", goal = "aa"
 *     **Output:** true
 * **Explanation:** You can swap s[0] = 'a' and s[1] = 'a' to get "aa", which
 * is equal to goal.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length, goal.length <= 2 * 104`
 *   * `s` and `goal` consist of lowercase letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/buddy-strings/
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
    bool buddyStrings(string s, string goal) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "ab"
    // Test case 2: "ba"
    // Test case 3: "ab"
    // Test case 4: "ab"
    // Test case 5: "aa"
    // Test case 6: "aa"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
