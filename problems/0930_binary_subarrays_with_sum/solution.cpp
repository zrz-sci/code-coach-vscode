// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 930: Binary Subarrays With Sum
 * Difficulty: Medium
 * Tags: Array, Hash Table, Prefix Sum, Sliding Window
 *
 * Problem Description:
 * Given a binary array `nums` and an integer `goal`, return _the number of
 * non-
 * empty**subarrays** with a sum_ `goal`.
 * 
 * A **subarray** is a contiguous part of the array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,0,1,0,1], goal = 2
 *     **Output:** 4
 *     **Explanation:** The 4 subarrays are bolded and underlined below:
 *     [_**1,0,1**_ ,0,1]
 *     [_**1,0,1,0**_ ,1]
 *     [1,_**0,1,0,1**_]
 *     [1,0,_**1,0,1**_]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0,0,0,0,0], goal = 0
 *     **Output:** 15
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 3 * 104`
 *   * `nums[i]` is either `0` or `1`.
 *   * `0 <= goal <= nums.length`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/binary-subarrays-with-sum/
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
    int numSubarraysWithSum(vector<int>& nums, int goal) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,0,1,0,1]
    // Test case 2: 2
    // Test case 3: [0,0,0,0,0]
    // Test case 4: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
