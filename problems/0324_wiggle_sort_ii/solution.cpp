// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 324: Wiggle Sort II
 * Difficulty: Medium
 * Tags: Greedy, Array, Divide and Conquer, Quickselect, Sorting
 *
 * Problem Description:
 * Given an integer array `nums`, reorder it such that `nums[0] < nums[1] >
 * nums[2]
 * < nums[3]...`.
 * 
 * You may assume the input array always has a valid answer.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,5,1,1,6,4]
 *     **Output:** [1,6,1,5,1,4]
 *     **Explanation:** [1,4,1,5,1,6] is also accepted.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,2,2,3,1]
 *     **Output:** [2,3,1,3,1,2]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 5 * 104`
 *   * `0 <= nums[i] <= 5000`
 * * It is guaranteed that there will be an answer for the given input `nums`.
 * 
 * 
 * 
 * **Follow Up:** Can you do it in `O(n)` time and/or **in-place** with `O(1)`
 * extra space?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/wiggle-sort-ii/
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
    void wiggleSort(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,5,1,1,6,4]
    // Test case 2: [1,3,2,2,3,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
