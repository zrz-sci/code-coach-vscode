// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 46: Permutations
 * Difficulty: Medium
 * Tags: Array, Backtracking
 *
 * Problem Description:
 * Given an array `nums` of distinct integers, return all the possible
 * permutations. You can return the answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3]
 *     **Output:** [[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0,1]
 *     **Output:** [[0,1],[1,0]]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1]
 *     **Output:** [[1]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 6`
 *   * `-10 <= nums[i] <= 10`
 *   * All the integers of `nums` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/permutations/
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
    vector<vector<int>> permute(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3]
    // Test case 2: [0,1]
    // Test case 3: [1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
