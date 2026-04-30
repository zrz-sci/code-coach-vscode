// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1732: Find the Highest Altitude
 * Difficulty: Easy
 * Tags: Array, Prefix Sum
 *
 * Problem Description:
 * There is a biker going on a road trip. The road trip consists of `n + 1`
 * points
 * at different altitudes. The biker starts his trip on point `0` with altitude
 * equal `0`.
 * 
 * You are given an integer array `gain` of length `n` where `gain[i]` is the
 * **net
 * gain in altitude** between points `i`​​​​​​ and `i + 1` for all (`0 <= i <
 * n)`.
 * Return _the**highest altitude** of a point._
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** gain = [-5,1,5,0,-7]
 *     **Output:** 1
 *     **Explanation:** The altitudes are [0,-5,-4,1,1,-6]. The highest is 1.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** gain = [-4,-3,-2,-1,4,3,2]
 *     **Output:** 0
 * **Explanation:** The altitudes are [0,-4,-7,-9,-10,-6,-3,-1]. The highest
 * is 0.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == gain.length`
 *   * `1 <= n <= 100`
 *   * `-100 <= gain[i] <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-the-highest-altitude/
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
    int largestAltitude(vector<int>& gain) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [-5,1,5,0,-7]
    // Test case 2: [-4,-3,-2,-1,4,3,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
