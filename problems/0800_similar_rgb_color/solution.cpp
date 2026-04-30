// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 800: Similar RGB Color
 * Difficulty: Easy
 * Tags: Math, String, Enumeration
 *
 * Problem Description:
 * The red-green-blue color `"#AABBCC"` can be written as `"#ABC"` in
 * shorthand.
 * 
 *   * For example, `"#15c"` is shorthand for the color `"#1155cc"`.
 * 
 * The similarity between the two colors `"#ABCDEF"` and `"#UVWXYZ"` is `-(AB -
 * UV)2 - (CD - WX)2 - (EF - YZ)2`.
 * 
 * Given a string `color` that follows the format `"#ABCDEF"`, return a string
 * represents the color that is most similar to the given color and has a
 * shorthand
 * (i.e., it can be represented as some `"#XYZ"`).
 * 
 * **Any answer** which has the same highest similarity as the best answer
 * will be
 * accepted.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** color = "#09f166"
 *     **Output:** "#11ee66"
 *     **Explanation:** 
 * The similarity is -(0x09 - 0x11)2 -(0xf1 - 0xee)2 - (0x66 - 0x66)2 = -64 -9
 * -0 = -73.
 *     This is the highest among any shorthand color.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** color = "#4e3fe1"
 *     **Output:** "#5544dd"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `color.length == 7`
 *   * `color[0] == '#'`
 * * `color[i]` is either digit or character in the range `['a', 'f']` for `i
 * > 0`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/similar-rgb-color/
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
    string similarRGB(string color) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "#09f166"
    // Test case 2: "#4e3fe1"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
