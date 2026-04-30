// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 503: Next Greater Element II
 * Difficulty: Medium
 * Tags: Stack, Array, Monotonic Stack
 *
 * Problem Description:
 * Given a circular integer array `nums` (i.e., the next element of
 * `nums[nums.length - 1]` is `nums[0]`), return _the**next greater number**
 * for
 * every element in_ `nums`.
 * 
 * The **next greater number** of a number `x` is the first greater number to
 * its
 * traversing-order next in the array, which means you could search circularly
 * to
 * find its next greater number. If it doesn't exist, return `-1` for this
 * number.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,1]
 *     **Output:** [2,-1,2]
 *     Explanation: The first 1's next greater number is 2; 
 *     The number 2 can't find next greater number. 
 * The second 1's next greater number needs to search circularly, which is
 * also 2.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,4,3]
 *     **Output:** [2,3,4,-1,4]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `-109 <= nums[i] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/next-greater-element-ii/
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
    vector<int> nextGreaterElements(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,1]
    // Test case 2: [1,2,3,4,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
