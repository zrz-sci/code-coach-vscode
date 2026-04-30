// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 78: Subsets
 * Difficulty: Medium
 * Tags: Bit Manipulation, Array, Backtracking
 *
 * Problem Description:
 * Given an integer array `nums` of **unique** elements, return _all possible_
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
 *     **Input:** nums = [1,2,3]
 *     **Output:** [[],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]]
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
 *   * All the numbers of `nums` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/subsets/
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
    vector<vector<int>> subsets(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3]
    // Test case 2: [0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
