// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 976: Largest Perimeter Triangle
 * Difficulty: Easy
 * Tags: Greedy, Array, Math, Sorting
 *
 * Problem Description:
 * Given an integer array `nums`, return _the largest perimeter of a triangle
 * with
 * a non-zero area, formed from three of these lengths_. If it is impossible to
 * form any triangle of a non-zero area, return `0`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [2,1,2]
 *     **Output:** 5
 * **Explanation:** You can form a triangle with three side lengths: 1, 2, and
 * 2.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,1,10]
 *     **Output:** 0
 *     **Explanation:** 
 *     You cannot use the side lengths 1, 1, and 2 to form a triangle.
 *     You cannot use the side lengths 1, 1, and 10 to form a triangle.
 *     You cannot use the side lengths 1, 2, and 10 to form a triangle.
 * As we cannot use any three side lengths to form a triangle of non-zero
 * area, we return 0.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `3 <= nums.length <= 104`
 *   * `1 <= nums[i] <= 106`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/largest-perimeter-triangle/
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
    int largestPerimeter(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,1,2]
    // Test case 2: [1,2,1,10]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
