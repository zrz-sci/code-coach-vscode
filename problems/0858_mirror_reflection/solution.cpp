// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 858: Mirror Reflection
 * Difficulty: Medium
 * Tags: Geometry, Math, Number Theory
 *
 * Problem Description:
 * There is a special square room with mirrors on each of the four walls.
 * Except
 * for the southwest corner, there are receptors on each of the remaining
 * corners,
 * numbered `0`, `1`, and `2`.
 * 
 * The square room has walls of length `p` and a laser ray from the southwest
 * corner first meets the east wall at a distance `q` from the `0th` receptor.
 * 
 * Given the two integers `p` and `q`, return _the number of the receptor that
 * the
 * ray meets first_.
 * 
 * The test cases are guaranteed so that the ray will meet a receptor
 * eventually.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** p = 2, q = 1
 *     **Output:** 2
 * **Explanation:** The ray meets receptor 2 the first time it gets reflected
 * back to the left wall.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** p = 3, q = 1
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= q <= p <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/mirror-reflection/
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
    int mirrorReflection(int p, int q) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 2
    // Test case 2: 1
    // Test case 3: 3
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
