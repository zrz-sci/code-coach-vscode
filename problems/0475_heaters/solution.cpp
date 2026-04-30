// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 475: Heaters
 * Difficulty: Medium
 * Tags: Array, Two Pointers, Binary Search, Sorting
 *
 * Problem Description:
 * Winter is coming! During the contest, your first job is to design a standard
 * heater with a fixed warm radius to warm all the houses.
 * 
 * Every house can be warmed, as long as the house is within the heater's warm
 * radius range.
 * 
 * Given the positions of `houses` and `heaters` on a horizontal line, return
 * _the
 * minimum radius standard of heaters  so that those heaters could cover all
 * houses._
 * 
 * **Notice** that all the `heaters` follow your radius standard, and the warm
 * radius will be the same.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** houses = [1,2,3], heaters = [2]
 *     **Output:** 1
 * **Explanation:** The only heater was placed in the position 2, and if we
 * use the radius 1 standard, then all the houses can be warmed.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** houses = [1,2,3,4], heaters = [1,4]
 *     **Output:** 1
 * **Explanation:** The two heaters were placed at positions 1 and 4. We need
 * to use a radius 1 standard, then all the houses can be warmed.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** houses = [1,5], heaters = [2]
 *     **Output:** 3
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= houses.length, heaters.length <= 3 * 104`
 *   * `1 <= houses[i], heaters[i] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/heaters/
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
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3]
    // Test case 2: [2]
    // Test case 3: [1,2,3,4]
    // Test case 4: [1,4]
    // Test case 5: [1,5]
    // Test case 6: [2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
