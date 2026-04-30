// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 40: Combination Sum II
 * Difficulty: Medium
 * Tags: Array, Backtracking
 *
 * Problem Description:
 * Given a collection of candidate numbers (`candidates`) and a target number
 * (`target`), find all unique combinations in `candidates` where the candidate
 * numbers sum to `target`.
 * 
 * Each number in `candidates` may only be used **once** in the combination.
 * 
 * **Note:**  The solution set must not contain duplicate combinations.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** candidates = [10,1,2,7,6,1,5], target = 8
 *     **Output:** 
 *     [
 *     [1,1,6],
 *     [1,2,5],
 *     [1,7],
 *     [2,6]
 *     ]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** candidates = [2,5,2,1,2], target = 5
 *     **Output:** 
 *     [
 *     [1,2,2],
 *     [5]
 *     ]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= candidates.length <= 100`
 *   * `1 <= candidates[i] <= 50`
 *   * `1 <= target <= 30`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/combination-sum-ii/
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
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [10,1,2,7,6,1,5]
    // Test case 2: 8
    // Test case 3: [2,5,2,1,2]
    // Test case 4: 5

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
