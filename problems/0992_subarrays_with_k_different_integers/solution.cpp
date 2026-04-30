// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 992: Subarrays with K Different Integers
 * Difficulty: Hard
 * Tags: Array, Hash Table, Counting, Sliding Window
 *
 * Problem Description:
 * Given an integer array `nums` and an integer `k`, return _the number
 * of**good
 * subarrays** of _`nums`.
 * 
 * A **good array** is an array where the number of different integers in that
 * array is exactly `k`.
 * 
 * * For example, `[1,2,3,1,2]` has `3` different integers: `1`, `2`, and `3`.
 * 
 * A **subarray** is a **contiguous** part of an array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,1,2,3], k = 2
 *     **Output:** 7
 * **Explanation:** Subarrays formed with exactly 2 different integers: [1,2],
 * [2,1], [1,2], [2,3], [1,2,1], [2,1,2], [1,2,1,2]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,1,3,4], k = 3
 *     **Output:** 3
 * **Explanation:** Subarrays formed with exactly 3 different integers:
 * [1,2,1,3], [2,1,3], [1,3,4].
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 2 * 104`
 *   * `1 <= nums[i], k <= nums.length`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/subarrays-with-k-different-integers/
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
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,1,2,3]
    // Test case 2: 2
    // Test case 3: [1,2,1,3,4]
    // Test case 4: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
