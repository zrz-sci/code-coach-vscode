// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 153: Find Minimum in Rotated Sorted Array
 * Difficulty: Medium
 * Tags: Array, Binary Search
 *
 * Problem Description:
 * Suppose an array of length `n` sorted in ascending order is **rotated**
 * between
 * `1` and `n` times. For example, the array `nums = [0,1,2,4,5,6,7]` might
 * become:
 * 
 *   * `[4,5,6,7,0,1,2]` if it was rotated `4` times.
 *   * `[0,1,2,4,5,6,7]` if it was rotated `7` times.
 * 
 * Notice that **rotating** an array `[a[0], a[1], a[2], ..., a[n-1]]` 1 time
 * results in the array `[a[n-1], a[0], a[1], a[2], ..., a[n-2]]`.
 * 
 * Given the sorted rotated array `nums` of **unique** elements, return _the
 * minimum element of this array_.
 * 
 * You must write an algorithm that runs in `O(log n) time`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [3,4,5,1,2]
 *     **Output:** 1
 *     **Explanation:** The original array was [1,2,3,4,5] rotated 3 times.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [4,5,6,7,0,1,2]
 *     **Output:** 0
 * **Explanation:** The original array was [0,1,2,4,5,6,7] and it was rotated
 * 4 times.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [11,13,15,17]
 *     **Output:** 11
 * **Explanation:** The original array was [11,13,15,17] and it was rotated 4
 * times.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `1 <= n <= 5000`
 *   * `-5000 <= nums[i] <= 5000`
 *   * All the integers of `nums` are **unique**.
 *   * `nums` is sorted and rotated between `1` and `n` times.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/
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
    int findMin(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,4,5,1,2]
    // Test case 2: [4,5,6,7,0,1,2]
    // Test case 3: [11,13,15,17]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
