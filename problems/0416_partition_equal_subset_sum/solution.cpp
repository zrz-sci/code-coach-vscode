// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 416: Partition Equal Subset Sum
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming
 *
 * Problem Description:
 * Given an integer array `nums`, return `true` _if you can partition the array
 * into two subsets such that the sum of the elements in both subsets is equal
 * or_`false` _otherwise_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,5,11,5]
 *     **Output:** true
 *     **Explanation:** The array can be partitioned as [1, 5, 5] and [11].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,5]
 *     **Output:** false
 *     **Explanation:** The array cannot be partitioned into equal sum subsets.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 200`
 *   * `1 <= nums[i] <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/partition-equal-subset-sum/
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
    bool canPartition(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,5,11,5]
    // Test case 2: [1,2,3,5]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
