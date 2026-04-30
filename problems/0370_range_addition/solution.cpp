// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 370: Range Addition
 * Difficulty: Medium
 * Tags: Array, Prefix Sum
 *
 * Problem Description:
 * You are given an integer `length` and an array `updates` where `updates[i] =
 * [startIdxi, endIdxi, inci]`.
 * 
 * You have an array `arr` of length `length` with all zeros, and you have some
 * operation to apply on `arr`. In the `ith` operation, you should increment
 * all
 * the elements `arr[startIdxi], arr[startIdxi + 1], ..., arr[endIdxi]` by
 * `inci`.
 * 
 * Return `arr` _after applying all the_ `updates`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** length = 5, updates = [[1,3,2],[2,4,3],[0,2,-2]]
 *     **Output:** [-2,0,3,5,3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** length = 10, updates = [[2,4,6],[5,6,8],[1,9,-4]]
 *     **Output:** [0,-4,2,2,2,4,4,-4,-4,-4]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= length <= 105`
 *   * `0 <= updates.length <= 104`
 *   * `0 <= startIdxi <= endIdxi < length`
 *   * `-1000 <= inci <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/range-addition/
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
    vector<int> getModifiedArray(int length, vector<vector<int>>& updates) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 5
    // Test case 2: [[1,3,2],[2,4,3],[0,2,-2]]
    // Test case 3: 10
    // Test case 4: [[2,4,6],[5,6,8],[1,9,-4]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
