// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 493: Reverse Pairs
 * Difficulty: Hard
 * Tags: Binary Indexed Tree, Segment Tree, Array, Binary Search, Divide and Conquer, Ordered Set, Merge Sort
 *
 * Problem Description:
 * Given an integer array `nums`, return _the number of**reverse pairs** in the
 * array_.
 * 
 * A **reverse pair** is a pair `(i, j)` where:
 * 
 *   * `0 <= i < j < nums.length` and
 *   * `nums[i] > 2 * nums[j]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,2,3,1]
 *     **Output:** 2
 *     **Explanation:** The reverse pairs are:
 *     (1, 4) --> nums[1] = 3, nums[4] = 1, 3 > 2 * 1
 *     (3, 4) --> nums[3] = 3, nums[4] = 1, 3 > 2 * 1
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [2,4,3,5,1]
 *     **Output:** 3
 *     **Explanation:** The reverse pairs are:
 *     (1, 4) --> nums[1] = 4, nums[4] = 1, 4 > 2 * 1
 *     (2, 4) --> nums[2] = 3, nums[4] = 1, 3 > 2 * 1
 *     (3, 4) --> nums[3] = 5, nums[4] = 1, 5 > 2 * 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 5 * 104`
 *   * `-231 <= nums[i] <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reverse-pairs/
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
    int reversePairs(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,2,3,1]
    // Test case 2: [2,4,3,5,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
