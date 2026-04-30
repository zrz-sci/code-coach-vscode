// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1200: Minimum Absolute Difference
 * Difficulty: Easy
 * Tags: Array, Sorting
 *
 * Problem Description:
 * Given an array of **distinct** integers `arr`, find all pairs of elements
 * with
 * the minimum absolute difference of any two elements.
 * 
 * Return a list of pairs in ascending order(with respect to pairs), each pair
 * `[a,
 * b]` follows
 * 
 *   * `a, b` are from `arr`
 *   * `a < b`
 * * `b - a` equals to the minimum absolute difference of any two elements in
 * `arr`
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** arr = [4,2,1,3]
 *     **Output:** [[1,2],[2,3],[3,4]]
 * **Explanation:** The minimum absolute difference is 1. List all pairs with
 * difference equal to 1 in ascending order.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** arr = [1,3,6,10,15]
 *     **Output:** [[1,3]]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** arr = [3,8,-10,23,19,-4,-14,27]
 *     **Output:** [[-14,-10],[19,23],[23,27]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= arr.length <= 105`
 *   * `-106 <= arr[i] <= 106`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-absolute-difference/
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
    vector<vector<int>> minimumAbsDifference(vector<int>& arr) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,2,1,3]
    // Test case 2: [1,3,6,10,15]
    // Test case 3: [3,8,-10,23,19,-4,-14,27]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
