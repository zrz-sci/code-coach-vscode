// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 327: Count of Range Sum
 * Difficulty: Hard
 * Tags: Binary Indexed Tree, Segment Tree, Array, Binary Search, Divide and Conquer, Ordered Set, Merge Sort
 *
 * Problem Description:
 * Given an integer array `nums` and two integers `lower` and `upper`, return
 * _the
 * number of range sums that lie in_ `[lower, upper]` _inclusive_.
 * 
 * Range sum `S(i, j)` is defined as the sum of the elements in `nums` between
 * indices `i` and `j` inclusive, where `i <= j`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [-2,5,-1], lower = -2, upper = 2
 *     **Output:** 3
 * **Explanation:** The three ranges are: [0,0], [2,2], and [0,2] and their
 * respective sums are: -2, -1, 2.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0], lower = 0, upper = 0
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `-231 <= nums[i] <= 231 - 1`
 *   * `-105 <= lower <= upper <= 105`
 *   * The answer is **guaranteed** to fit in a **32-bit** integer.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-of-range-sum/
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
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [-2,5,-1]
    // Test case 2: -2
    // Test case 3: 2
    // Test case 4: [0]
    // Test case 5: 0
    // Test case 6: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
