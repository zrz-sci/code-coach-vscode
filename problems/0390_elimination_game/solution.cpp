// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 390: Elimination Game
 * Difficulty: Medium
 * Tags: Recursion, Math
 *
 * Problem Description:
 * You have a list `arr` of all integers in the range `[1, n]` sorted in a
 * strictly
 * increasing order. Apply the following algorithm on `arr`:
 * 
 * * Starting from left to right, remove the first number and every other
 * number afterward until you reach the end of the list.
 * * Repeat the previous step again, but this time from right to left, remove
 * the rightmost number and every other number from the remaining numbers.
 * * Keep repeating the steps again, alternating left to right and right to
 * left, until a single number remains.
 * 
 * Given the integer `n`, return _the last number that remains in_ `arr`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 9
 *     **Output:** 6
 *     **Explanation:**
 *     arr = [_1_ , 2, _3_ , 4, _5_ , 6, _7_ , 8, _9_]
 *     arr = [2, _4_ , 6, _8_]
 *     arr = [_2_ , 6]
 *     arr = [6]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 1
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/elimination-game/
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
    int lastRemaining(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 9
    // Test case 2: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
