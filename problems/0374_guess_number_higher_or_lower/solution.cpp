// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 374: Guess Number Higher or Lower
 * Difficulty: Easy
 * Tags: Binary Search, Interactive
 *
 * Problem Description:
 * We are playing the Guess Game. The game is as follows:
 * 
 * I pick a number from `1` to `n`. You have to guess which number I picked
 * (the
 * number I picked stays the same throughout the game).
 * 
 * Every time you guess wrong, I will tell you whether the number I picked is
 * higher or lower than your guess.
 * 
 * You call a pre-defined API `int guess(int num)`, which returns three
 * possible
 * results:
 * 
 *   * `-1`: Your guess is higher than the number I picked (i.e. `num > pick`).
 *   * `1`: Your guess is lower than the number I picked (i.e. `num < pick`).
 *   * `0`: your guess is equal to the number I picked (i.e. `num == pick`).
 * 
 * Return _the number that I picked_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 10, pick = 6
 *     **Output:** 6
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 1, pick = 1
 *     **Output:** 1
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** n = 2, pick = 1
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 231 - 1`
 *   * `1 <= pick <= n`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/guess-number-higher-or-lower/
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

/** 
 * Forward declaration of guess API.
 * @param  num   your guess
 * @return 	     -1 if num is higher than the picked number
 *			      1 if num is lower than the picked number
 *               otherwise return 0
 * int guess(int num);
 */

class Solution {
public:
    int guessNumber(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 10
    // Test case 2: 6
    // Test case 3: 1
    // Test case 4: 1
    // Test case 5: 2
    // Test case 6: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
