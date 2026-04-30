// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 952: Largest Component Size by Common Factor
 * Difficulty: Hard
 * Tags: Union Find, Array, Hash Table, Math, Number Theory
 *
 * Problem Description:
 * You are given an integer array of unique positive integers `nums`. Consider
 * the
 * following graph:
 * 
 * * There are `nums.length` nodes, labeled `nums[0]` to `nums[nums.length -
 * 1]`,
 * * There is an undirected edge between `nums[i]` and `nums[j]` if `nums[i]`
 * and `nums[j]` share a common factor greater than `1`.
 * 
 * Return _the size of the largest connected component in the graph_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [4,6,15,35]
 *     **Output:** 4
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [20,50,9,63]
 *     **Output:** 2
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [2,3,6,7,4,12,21,39]
 *     **Output:** 8
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 2 * 104`
 *   * `1 <= nums[i] <= 105`
 *   * All the values of `nums` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/largest-component-size-by-common-factor/
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
    int largestComponentSize(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,6,15,35]
    // Test case 2: [20,50,9,63]
    // Test case 3: [2,3,6,7,4,12,21,39]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
