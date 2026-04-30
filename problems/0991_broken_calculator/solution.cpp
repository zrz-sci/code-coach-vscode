// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 991: Broken Calculator
 * Difficulty: Medium
 * Tags: Greedy, Math
 *
 * Problem Description:
 * There is a broken calculator that has the integer `startValue` on its
 * display
 * initially. In one operation, you can:
 * 
 *   * multiply the number on display by `2`, or
 *   * subtract `1` from the number on display.
 * 
 * Given two integers `startValue` and `target`, return _the minimum number of
 * operations needed to display_`target` _on the calculator_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** startValue = 2, target = 3
 *     **Output:** 2
 * **Explanation:** Use double operation and then decrement operation {2 -> 4
 * -> 3}.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** startValue = 5, target = 8
 *     **Output:** 2
 *     **Explanation:** Use decrement and then double {5 -> 4 -> 8}.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** startValue = 3, target = 10
 *     **Output:** 3
 *     **Explanation:** Use double, decrement and double {3 -> 6 -> 5 -> 10}.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= startValue, target <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/broken-calculator/
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
    int brokenCalc(int startValue, int target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 2
    // Test case 2: 3
    // Test case 3: 5
    // Test case 4: 8
    // Test case 5: 3
    // Test case 6: 10

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
