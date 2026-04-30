// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 757: Set Intersection Size At Least Two
 * Difficulty: Hard
 * Tags: Greedy, Array, Sorting
 *
 * Problem Description:
 * You are given a 2D integer array `intervals` where `intervals[i] = [starti,
 * endi]` represents all the integers from `starti` to `endi` inclusively.
 * 
 * A **containing set** is an array `nums` where each interval from
 * `intervals` has
 * **at least two** integers in `nums`.
 * 
 * * For example, if `intervals = [[1,3], [3,7], [8,9]]`, then `[1,2,4,7,8,9]`
 * and `[2,3,4,8,9]` are **containing sets**.
 * 
 * Return _the minimum possible size of a containing set_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** intervals = [[1,3],[3,7],[8,9]]
 *     **Output:** 5
 *     **Explanation:** let nums = [2, 3, 4, 8, 9].
 *     It can be shown that there cannot be any containing array of size 4.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** intervals = [[1,3],[1,4],[2,5],[3,5]]
 *     **Output:** 3
 *     **Explanation:** let nums = [2, 3, 4].
 *     It can be shown that there cannot be any containing array of size 2.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** intervals = [[1,2],[2,3],[2,4],[4,5]]
 *     **Output:** 5
 *     **Explanation:** let nums = [1, 2, 3, 4, 5].
 *     It can be shown that there cannot be any containing array of size 4.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= intervals.length <= 3000`
 *   * `intervals[i].length == 2`
 *   * `0 <= starti < endi <= 108`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/set-intersection-size-at-least-two/
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
    int intersectionSizeTwo(vector<vector<int>>& intervals) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,3],[3,7],[8,9]]
    // Test case 2: [[1,3],[1,4],[2,5],[3,5]]
    // Test case 3: [[1,2],[2,3],[2,4],[4,5]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
