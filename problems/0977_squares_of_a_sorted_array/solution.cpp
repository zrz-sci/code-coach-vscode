// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 977: Squares of a Sorted Array
 * Difficulty: Easy
 * Tags: Array, Two Pointers, Sorting
 *
 * Problem Description:
 * Given an integer array `nums` sorted in **non-decreasing** order, return _an
 * array of**the squares of each number** sorted in non-decreasing order_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [-4,-1,0,3,10]
 *     **Output:** [0,1,9,16,100]
 *     **Explanation:** After squaring, the array becomes [16,1,0,9,100].
 *     After sorting, it becomes [0,1,9,16,100].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [-7,-3,2,3,11]
 *     **Output:** [4,9,9,49,121]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `-104 <= nums[i] <= 104`
 *   * `nums` is sorted in **non-decreasing** order.
 * 
 * 
 * 
 * **Follow up:** Squaring each element and sorting the new array is very
 * trivial,
 * could you find an `O(n)` solution using a different approach?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/squares-of-a-sorted-array/
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
    vector<int> sortedSquares(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [-4,-1,0,3,10]
    // Test case 2: [-7,-3,2,3,11]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
