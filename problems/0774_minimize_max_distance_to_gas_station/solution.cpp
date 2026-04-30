// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 774: Minimize Max Distance to Gas Station
 * Difficulty: Hard
 * Tags: Array, Binary Search
 *
 * Problem Description:
 * You are given an integer array `stations` that represents the positions of
 * the
 * gas stations on the **x-axis**. You are also given an integer `k`.
 * 
 * You should add `k` new gas stations. You can add the stations anywhere on
 * the
 * **x-axis** , and not necessarily on an integer position.
 * 
 * Let `penalty()` be the maximum distance between **adjacent** gas stations
 * after
 * adding the `k` new stations.
 * 
 * Return _the smallest possible value of_ `penalty()`. Answers within `10-6`
 * of
 * the actual answer will be accepted.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** stations = [1,2,3,4,5,6,7,8,9,10], k = 9
 *     **Output:** 0.50000
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** stations = [23,24,36,39,46,56,57,65,84,98], k = 1
 *     **Output:** 14.00000
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `10 <= stations.length <= 2000`
 *   * `0 <= stations[i] <= 108`
 *   * `stations` is sorted in a **strictly increasing** order.
 *   * `1 <= k <= 106`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimize-max-distance-to-gas-station/
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
    double minmaxGasDist(vector<int>& stations, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4,5,6,7,8,9,10]
    // Test case 2: 9
    // Test case 3: [23,24,36,39,46,56,57,65,84,98]
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
