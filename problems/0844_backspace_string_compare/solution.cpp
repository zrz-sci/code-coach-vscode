// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 844: Backspace String Compare
 * Difficulty: Easy
 * Tags: Stack, Two Pointers, String, Simulation
 *
 * Problem Description:
 * Given two strings `s` and `t`, return `true` _if they are equal when both
 * are
 * typed into empty text editors_. `'#'` means a backspace character.
 * 
 * Note that after backspacing an empty text, the text will continue empty.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "ab#c", t = "ad#c"
 *     **Output:** true
 *     **Explanation:** Both s and t become "ac".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "ab##", t = "c#d#"
 *     **Output:** true
 *     **Explanation:** Both s and t become "".
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "a#c", t = "b"
 *     **Output:** false
 *     **Explanation:** s becomes "c" while t becomes "b".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length, t.length <= 200`
 *   * `s` and `t` only contain lowercase letters and `'#'` characters.
 * 
 * 
 * 
 * **Follow up:** Can you solve it in `O(n)` time and `O(1)` space?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/backspace-string-compare/
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
    bool backspaceCompare(string s, string t) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "ab#c"
    // Test case 2: "ad#c"
    // Test case 3: "ab##"
    // Test case 4: "c#d#"
    // Test case 5: "a#c"
    // Test case 6: "b"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
