// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 90: Subsets II
 * Difficulty: Medium
 * Tags: Bit Manipulation, Array, Backtracking
 *
 * Problem Description:
 * Given an integer array `nums` that may contain duplicates, return _all
 * possible_
 * _subsets_ _(the power set)_.
 * 
 * The solution set **must not** contain duplicate subsets. Return the
 * solution in
 * **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,2]
 *     **Output:** [[],[1],[1,2],[1,2,2],[2],[2,2]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0]
 *     **Output:** [[],[0]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 10`
 *   * `-10 <= nums[i] <= 10`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/subsets-ii/
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
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,2]
    // Test case 2: [0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
