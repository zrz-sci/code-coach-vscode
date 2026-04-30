// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 896: Monotonic Array
 * Difficulty: Easy
 * Tags: Array
 *
 * Problem Description:
 * An array is **monotonic** if it is either monotone increasing or monotone
 * decreasing.
 * 
 * An array `nums` is monotone increasing if for all `i <= j`, `nums[i] <=
 * nums[j]`. An array `nums` is monotone decreasing if for all `i <= j`,
 * `nums[i]
 * >= nums[j]`.
 * 
 * Given an integer array `nums`, return `true` _if the given array is
 * monotonic,
 * or_`false` _otherwise_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,2,3]
 *     **Output:** true
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [6,5,4,4]
 *     **Output:** true
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,2]
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `-105 <= nums[i] <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/monotonic-array/
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
    bool isMonotonic(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,2,3]
    // Test case 2: [6,5,4,4]
    // Test case 3: [1,3,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
