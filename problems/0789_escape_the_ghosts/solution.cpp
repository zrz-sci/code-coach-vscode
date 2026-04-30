// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 789: Escape The Ghosts
 * Difficulty: Medium
 * Tags: Array, Math
 *
 * Problem Description:
 * You are playing a simplified PAC-MAN game on an infinite 2-D grid. You
 * start at
 * the point `[0, 0]`, and you are given a destination point `target =
 * [xtarget,
 * ytarget]` that you are trying to get to. There are several ghosts on the map
 * with their starting positions given as a 2D array `ghosts`, where
 * `ghosts[i] =
 * [xi, yi]` represents the starting position of the `ith` ghost. All inputs
 * are
 * **integral coordinates**.
 * 
 * Each turn, you and all the ghosts may independently choose to either **move
 * 1
 * unit** in any of the four cardinal directions: north, east, south, or west,
 * or
 * **stay still**. All actions happen **simultaneously**.
 * 
 * You escape if and only if you can reach the target **before** any ghost
 * reaches
 * you. If you reach any square (including the target) at the **same time** as
 * a
 * ghost, it **does not** count as an escape.
 * 
 * Return `true` _if it is possible to escape regardless of how the ghosts
 * move,
 * otherwise return_`false` _._
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** ghosts = [[1,0],[0,3]], target = [0,1]
 *     **Output:** true
 * **Explanation:** You can reach the destination (0, 1) after 1 turn, while
 * the ghosts located at (1, 0) and (0, 3) cannot catch up with you.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** ghosts = [[1,0]], target = [2,0]
 *     **Output:** false
 * **Explanation:** You need to reach the destination (2, 0), but the ghost at
 * (1, 0) lies between you and the destination.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** ghosts = [[2,0]], target = [1,0]
 *     **Output:** false
 *     **Explanation:** The ghost can reach the target at the same time as you.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= ghosts.length <= 100`
 *   * `ghosts[i].length == 2`
 *   * `-104 <= xi, yi <= 104`
 *   * There can be **multiple ghosts** in the same location.
 *   * `target.length == 2`
 *   * `-104 <= xtarget, ytarget <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/escape-the-ghosts/
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
    bool escapeGhosts(vector<vector<int>>& ghosts, vector<int>& target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,0],[0,3]]
    // Test case 2: [0,1]
    // Test case 3: [[1,0]]
    // Test case 4: [2,0]
    // Test case 5: [[2,0]]
    // Test case 6: [1,0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
