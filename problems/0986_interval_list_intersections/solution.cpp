// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 986: Interval List Intersections
 * Difficulty: Medium
 * Tags: Array, Two Pointers, Sweep Line
 *
 * Problem Description:
 * You are given two lists of closed intervals, `firstList` and `secondList`,
 * where
 * `firstList[i] = [starti, endi]` and `secondList[j] = [startj, endj]`. Each
 * list
 * of intervals is pairwise **disjoint** and in **sorted order**.
 * 
 * Return _the intersection of these two interval lists_.
 * 
 * A **closed interval** `[a, b]` (with `a <= b`) denotes the set of real
 * numbers
 * `x` with `a <= x <= b`.
 * 
 * The **intersection** of two closed intervals is a set of real numbers that
 * are
 * either empty or represented as a closed interval. For example, the
 * intersection
 * of `[1, 3]` and `[2, 4]` is `[2, 3]`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** firstList = [[0,2],[5,10],[13,23],[24,25]], secondList =
 * [[1,5],[8,12],[15,24],[25,26]]
 *     **Output:** [[1,2],[5,5],[8,10],[15,23],[24,24],[25,25]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** firstList = [[1,3],[5,9]], secondList = []
 *     **Output:** []
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= firstList.length, secondList.length <= 1000`
 *   * `firstList.length + secondList.length >= 1`
 *   * `0 <= starti < endi <= 109`
 *   * `endi < starti+1`
 *   * `0 <= startj < endj <= 109 `
 *   * `endj < startj+1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/interval-list-intersections/
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
    vector<vector<int>> intervalIntersection(vector<vector<int>>& firstList, vector<vector<int>>& secondList) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[0,2],[5,10],[13,23],[24,25]]
    // Test case 2: [[1,5],[8,12],[15,24],[25,26]]
    // Test case 3: [[1,3],[5,9]]
    // Test case 4: []

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
