// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 941: Valid Mountain Array
 * Difficulty: Easy
 * Tags: Array
 *
 * Problem Description:
 * Given an array of integers `arr`, return _`true` if and only if it is a
 * valid
 * mountain array_.
 * 
 * Recall that arr is a mountain array if and only if:
 * 
 *   * `arr.length >= 3`
 *   * There exists some `i` with `0 < i < arr.length - 1` such that: 
 *     * `arr[0] < arr[1] < ... < arr[i - 1] < arr[i] `
 *     * `arr[i] > arr[i + 1] > ... > arr[arr.length - 1]`
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** arr = [2,1]
 *     **Output:** false
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** arr = [3,5,5]
 *     **Output:** false
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** arr = [0,3,2,1]
 *     **Output:** true
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= arr.length <= 104`
 *   * `0 <= arr[i] <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/valid-mountain-array/
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
    bool validMountainArray(vector<int>& arr) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,1]
    // Test case 2: [3,5,5]
    // Test case 3: [0,3,2,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
