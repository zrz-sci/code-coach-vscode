// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 279: Perfect Squares
 * Difficulty: Medium
 * Tags: Breadth-First Search, Math, Dynamic Programming
 *
 * Problem Description:
 * Given an integer `n`, return _the least number of perfect square numbers
 * that
 * sum to_ `n`.
 * 
 * A **perfect square** is an integer that is the square of an integer; in
 * other
 * words, it is the product of some integer with itself. For example, `1`, `4`,
 * `9`, and `16` are perfect squares while `3` and `11` are not.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 12
 *     **Output:** 3
 *     **Explanation:** 12 = 4 + 4 + 4.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 13
 *     **Output:** 2
 *     **Explanation:** 13 = 4 + 9.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/perfect-squares/
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
    int numSquares(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 12
    // Test case 2: 13

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
