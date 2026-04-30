// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 215: Kth Largest Element in an Array
 * Difficulty: Medium
 * Tags: Array, Divide and Conquer, Quickselect, Sorting, Heap (Priority Queue)
 *
 * Problem Description:
 * Given an integer array `nums` and an integer `k`, return _the_ `kth`
 * _largest
 * element in the array_.
 * 
 * Note that it is the `kth` largest element in the sorted order, not the `kth`
 * distinct element.
 * 
 * Can you solve it without sorting?
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [3,2,1,5,6,4], k = 2
 *     **Output:** 5
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [3,2,3,1,2,4,5,5,6], k = 4
 *     **Output:** 4
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= k <= nums.length <= 105`
 *   * `-104 <= nums[i] <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/kth-largest-element-in-an-array/
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
    int findKthLargest(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,2,1,5,6,4]
    // Test case 2: 2
    // Test case 3: [3,2,3,1,2,4,5,5,6]
    // Test case 4: 4

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
