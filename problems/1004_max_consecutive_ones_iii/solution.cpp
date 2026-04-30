// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1004: Max Consecutive Ones III
 * Difficulty: Medium
 * Tags: Array, Binary Search, Prefix Sum, Sliding Window
 *
 * Problem Description:
 * Given a binary array `nums` and an integer `k`, return _the maximum number
 * of
 * consecutive_`1` _' s in the array if you can flip at most_ `k` `0`'s.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,1,0,0,0,1,1,1,1,0], k = 2
 *     **Output:** 6
 *     **Explanation:** [1,1,1,0,0,_**1** ,1,1,1,1,**1**_]
 * Bolded numbers were flipped from 0 to 1. The longest subarray is
 * underlined.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1], k = 3
 *     **Output:** 10
 * **Explanation:** [0,0,_1,1,**1** ,**1** ,1,1,1,**1** ,1,1_,0,0,0,1,1,1,1]
 * Bolded numbers were flipped from 0 to 1. The longest subarray is
 * underlined.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `nums[i]` is either `0` or `1`.
 *   * `0 <= k <= nums.length`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/max-consecutive-ones-iii/
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
    int longestOnes(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,1,1,0,0,0,1,1,1,1,0]
    // Test case 2: 2
    // Test case 3: [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1]
    // Test case 4: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
