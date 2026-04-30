// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 293: Flip Game
 * Difficulty: Easy
 * Tags: String
 *
 * Problem Description:
 * You are playing a Flip Game with your friend.
 * 
 * You are given a string `currentState` that contains only `'+'` and `'-'`.
 * You
 * and your friend take turns to flip **two consecutive** `"++"` into `"--"`.
 * The
 * game ends when a person can no longer make a move, and therefore the other
 * person will be the winner.
 * 
 * Return all possible states of the string `currentState` after **one valid
 * move**. You may return the answer in **any order**. If there is no valid
 * move,
 * return an empty list `[]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** currentState = "++++"
 *     **Output:** ["--++","+--+","++--"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** currentState = "+"
 *     **Output:** []
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= currentState.length <= 500`
 *   * `currentState[i]` is either `'+'` or `'-'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/flip-game/
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
    vector<string> generatePossibleNextMoves(string currentState) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "++++"
    // Test case 2: "+"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
