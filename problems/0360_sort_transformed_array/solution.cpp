// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 360: Sort Transformed Array
 * Difficulty: Medium
 * Tags: Array, Math, Two Pointers, Sorting
 *
 * Problem Description:
 * Given a **sorted** integer array `nums` and three integers `a`, `b` and `c`,
 * apply a quadratic function of the form `f(x) = ax2 + bx + c` to each element
 * `nums[i]` in the array, and return _the array in a sorted order_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [-4,-2,2,4], a = 1, b = 3, c = 5
 *     **Output:** [3,9,15,33]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [-4,-2,2,4], a = -1, b = 3, c = 5
 *     **Output:** [-23,-5,1,7]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 200`
 *   * `-100 <= nums[i], a, b, c <= 100`
 *   * `nums` is sorted in **ascending** order.
 * 
 * 
 * 
 * **Follow up:** Could you solve it in `O(n)` time?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sort-transformed-array/
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
    vector<int> sortTransformedArray(vector<int>& nums, int a, int b, int c) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [-4,-2,2,4]
    // Test case 2: 1
    // Test case 3: 3
    // Test case 4: 5
    // Test case 5: [-4,-2,2,4]
    // Test case 6: -1
    // Test case 7: 3
    // Test case 8: 5

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
