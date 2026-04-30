// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 651: 4 Keys Keyboard
 * Difficulty: Medium
 * Tags: Math, Dynamic Programming
 *
 * Problem Description:
 * Imagine you have a special keyboard with the following keys:
 * 
 *   * A: Print one `'A'` on the screen.
 *   * Ctrl-A: Select the whole screen.
 *   * Ctrl-C: Copy selection to buffer.
 * * Ctrl-V: Print buffer on screen appending it after what has already been
 * printed.
 * 
 * Given an integer n, return _the maximum number of_`'A'`_you can print on the
 * screen with**at most** _`n` _presses on the keys_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 3
 *     **Output:** 3
 * **Explanation:** We can at most get 3 A's on screen by pressing the
 * following key sequence:
 *     A, A, A
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 7
 *     **Output:** 9
 * **Explanation:** We can at most get 9 A's on screen by pressing following
 * key sequence:
 *     A, A, A, Ctrl A, Ctrl C, Ctrl V, Ctrl V
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 50`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/4-keys-keyboard/
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
    int maxA(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 7

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
