// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 650: 2 Keys Keyboard
 * Difficulty: Medium
 * Tags: Math, Dynamic Programming
 *
 * Problem Description:
 * There is only one character `'A'` on the screen of a notepad. You can
 * perform
 * one of two operations on this notepad for each step:
 * 
 * * Copy All: You can copy all the characters present on the screen (a
 * partial copy is not allowed).
 *   * Paste: You can paste the characters which are copied last time.
 * 
 * Given an integer `n`, return _the minimum number of operations to get the
 * character_ `'A'` _exactly_ `n` _times on the screen_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 3
 *     **Output:** 3
 *     **Explanation:** Initially, we have one character 'A'.
 *     In step 1, we use Copy All operation.
 *     In step 2, we use Paste operation to get 'AA'.
 *     In step 3, we use Paste operation to get 'AAA'.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 1
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/2-keys-keyboard/
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
    int minSteps(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
