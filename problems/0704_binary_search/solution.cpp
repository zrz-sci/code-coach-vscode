// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * 💡 参考解法: ./reference.cpp
 *
 * LeetCode Problem 704: Binary Search
 * Difficulty: Easy
 * Tags: Array, Binary Search
 *
 * Problem Description:
 * Given an array of integers `nums` which is sorted in ascending order, and an
 * integer `target`, write a function to search `target` in `nums`. If `target`
 * exists, then return its index. Otherwise, return `-1`.
 * 
 * You must write an algorithm with `O(log n)` runtime complexity.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [-1,0,3,5,9,12], target = 9
 *     **Output:** 4
 *     **Explanation:** 9 exists in nums and its index is 4
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [-1,0,3,5,9,12], target = 2
 *     **Output:** -1
 *     **Explanation:** 2 does not exist in nums so return -1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `-104 < nums[i], target < 104`
 *   * All the integers in `nums` are **unique**.
 *   * `nums` is sorted in ascending order.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/binary-search/
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
    int search(vector<int>& nums, int target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [-1,0,3,5,9,12]
    // Test case 2: 9
    // Test case 3: [-1,0,3,5,9,12]
    // Test case 4: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
