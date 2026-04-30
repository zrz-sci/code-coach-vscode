// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 611: Valid Triangle Number
 * Difficulty: Medium
 * Tags: Greedy, Array, Two Pointers, Binary Search, Sorting
 *
 * Problem Description:
 * Given an integer array `nums`, return _the number of triplets chosen from
 * the
 * array that can make triangles if we take them as side lengths of a
 * triangle_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [2,2,3,4]
 *     **Output:** 3
 *     **Explanation:** Valid combinations are: 
 *     2,3,4 (using the first 2)
 *     2,3,4 (using the second 2)
 *     2,2,3
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [4,2,3,4]
 *     **Output:** 4
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 1000`
 *   * `0 <= nums[i] <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/valid-triangle-number/
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
    int triangleNumber(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,2,3,4]
    // Test case 2: [4,2,3,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
