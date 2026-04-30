// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 414: Third Maximum Number
 * Difficulty: Easy
 * Tags: Array, Sorting
 *
 * Problem Description:
 * Given an integer array `nums`, return _the**third distinct maximum** number
 * in
 * this array. If the third maximum does not exist, return the **maximum**
 * number_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [3,2,1]
 *     **Output:** 1
 *     **Explanation:**
 *     The first distinct maximum is 3.
 *     The second distinct maximum is 2.
 *     The third distinct maximum is 1.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2]
 *     **Output:** 2
 *     **Explanation:**
 *     The first distinct maximum is 2.
 *     The second distinct maximum is 1.
 * The third distinct maximum does not exist, so the maximum (2) is returned
 * instead.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [2,2,3,1]
 *     **Output:** 1
 *     **Explanation:**
 *     The first distinct maximum is 3.
 * The second distinct maximum is 2 (both 2's are counted together since they
 * have the same value).
 *     The third distinct maximum is 1.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `-231 <= nums[i] <= 231 - 1`
 * 
 * 
 * 
 * **Follow up:** Can you find an `O(n)` solution?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/third-maximum-number/
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
    int thirdMax(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,2,1]
    // Test case 2: [1,2]
    // Test case 3: [2,2,3,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
