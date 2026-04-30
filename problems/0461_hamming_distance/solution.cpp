// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 461: Hamming Distance
 * Difficulty: Easy
 * Tags: Bit Manipulation
 *
 * Problem Description:
 * The [Hamming distance](https://en.wikipedia.org/wiki/Hamming_distance)
 * between
 * two integers is the number of positions at which the corresponding bits are
 * different.
 * 
 * Given two integers `x` and `y`, return _the**Hamming distance** between
 * them_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** x = 1, y = 4
 *     **Output:** 2
 *     **Explanation:**
 *     1   (0 0 0 1)
 *     4   (0 1 0 0)
 *            ↑   ↑
 * The above arrows point to positions where the corresponding bits are
 * different.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** x = 3, y = 1
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= x, y <= 231 - 1`
 * 
 * 
 * 
 * **Note:** This question is the same as [ 2220: Minimum Bit Flips to Convert
 * Number.](https://leetcode.com/problems/minimum-bit-flips-to-convert-
 * number/description/)
 * 
 * 
 *
 * Link: https://leetcode.com/problems/hamming-distance/
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
    int hammingDistance(int x, int y) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 1
    // Test case 2: 4
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
