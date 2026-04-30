// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 1: Two Sum
 * Difficulty: Easy
 * Tags: Array, Hash Table
 *
 * Problem Description:
 * Given an array of integers `nums` and an integer `target`, return _indices
 * of
 * the two numbers such that they add up to`target`_.
 * 
 * You may assume that each input would have **_exactly_ one solution**, and
 * you
 * may not use the _same_ element twice.
 * 
 * You can return the answer in any order.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [2,7,11,15], target = 9
 *     **Output:** [0,1]
 *     **Explanation:** Because nums[0] + nums[1] == 9, we return [0, 1].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [3,2,4], target = 6
 *     **Output:** [1,2]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [3,3], target = 6
 *     **Output:** [0,1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= nums.length <= 104`
 *   * `-109 <= nums[i] <= 109`
 *   * `-109 <= target <= 109`
 *   * **Only one valid answer exists.**
 * 
 * 
 * 
 * **Follow-up: **Can you come up with an algorithm that is less than `O(n2)`
 * time
 * complexity?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/two-sum/
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
    vector<int> twoSum(vector<int>& nums, int target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,7,11,15]
    // Test case 2: 9
    // Test case 3: [3,2,4]
    // Test case 4: 6
    // Test case 5: [3,3]
    // Test case 6: 6

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
