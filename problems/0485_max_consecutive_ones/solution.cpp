// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 485: Max Consecutive Ones
 * Difficulty: Easy
 * Tags: Array
 *
 * Problem Description:
 * Given a binary array `nums`, return _the maximum number of consecutive_`1`
 * _' s
 * in the array_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,0,1,1,1]
 *     **Output:** 3
 * **Explanation:** The first two digits or the last three digits are
 * consecutive 1s. The maximum number of consecutive 1s is 3.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,0,1,1,0,1]
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `nums[i]` is either `0` or `1`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/max-consecutive-ones/
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
    int findMaxConsecutiveOnes(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,1,0,1,1,1]
    // Test case 2: [1,0,1,1,0,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
