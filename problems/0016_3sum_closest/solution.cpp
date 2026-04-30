// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 16: 3Sum Closest
 * Difficulty: Medium
 * Tags: Array, Two Pointers, Sorting
 *
 * Problem Description:
 * Given an integer array `nums` of length `n` and an integer `target`, find
 * three
 * integers at **distinct indices** in `nums` such that the sum is closest to
 * `target`.
 * 
 * Return _the sum of the three integers_.
 * 
 * You may assume that each input would have exactly one solution.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [-1,2,1,-4], target = 1
 *     **Output:** 2
 * **Explanation:** The sum that is closest to the target is 2. (-1 + 2 + 1 =
 * 2).
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0,0,0], target = 1
 *     **Output:** 0
 * **Explanation:** The sum that is closest to the target is 0. (0 + 0 + 0 =
 * 0).
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `3 <= nums.length <= 500`
 *   * `-1000 <= nums[i] <= 1000`
 *   * `-104 <= target <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/3sum-closest/
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
    int threeSumClosest(vector<int>& nums, int target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [-1,2,1,-4]
    // Test case 2: 1
    // Test case 3: [0,0,0]
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
