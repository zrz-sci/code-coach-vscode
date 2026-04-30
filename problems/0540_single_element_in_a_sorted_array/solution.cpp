// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 540: Single Element in a Sorted Array
 * Difficulty: Medium
 * Tags: Array, Binary Search
 *
 * Problem Description:
 * You are given a sorted array consisting of only integers where every element
 * appears exactly twice, except for one element which appears exactly once.
 * 
 * Return _the single element that appears only once_.
 * 
 * Your solution must run in `O(log n)` time and `O(1)` space.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,2,3,3,4,4,8,8]
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [3,3,7,7,10,11,11]
 *     **Output:** 10
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `0 <= nums[i] <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/single-element-in-a-sorted-array/
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
    int singleNonDuplicate(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,1,2,3,3,4,4,8,8]
    // Test case 2: [3,3,7,7,10,11,11]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
