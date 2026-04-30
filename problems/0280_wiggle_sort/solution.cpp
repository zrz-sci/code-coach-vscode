// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 280: Wiggle Sort
 * Difficulty: Medium
 * Tags: Greedy, Array, Sorting
 *
 * Problem Description:
 * Given an integer array `nums`, reorder it such that `nums[0] <= nums[1] >=
 * nums[2] <= nums[3]...`.
 * 
 * You may assume the input array always has a valid answer.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [3,5,2,1,6,4]
 *     **Output:** [3,5,1,6,2,4]
 *     **Explanation:** [1,6,2,5,3,4] is also accepted.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [6,6,5,6,3,8]
 *     **Output:** [6,6,5,6,3,8]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 5 * 104`
 *   * `0 <= nums[i] <= 104`
 * * It is guaranteed that there will be an answer for the given input `nums`.
 * 
 * 
 * 
 * **Follow up:** Could you solve the problem in `O(n)` time complexity?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/wiggle-sort/
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
    // Test case 1: [3,5,2,1,6,4]
    // Test case 2: [6,6,5,6,3,8]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
