// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 56: Merge Intervals
 * Difficulty: Medium
 * Tags: Array, Sorting
 *
 * Problem Description:
 * Given an array of `intervals` where `intervals[i] = [starti, endi]`, merge
 * all
 * overlapping intervals, and return _an array of the non-overlapping intervals
 * that cover all the intervals in the input_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** intervals = [[1,3],[2,6],[8,10],[15,18]]
 *     **Output:** [[1,6],[8,10],[15,18]]
 * **Explanation:** Since intervals [1,3] and [2,6] overlap, merge them into
 * [1,6].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** intervals = [[1,4],[4,5]]
 *     **Output:** [[1,5]]
 *     **Explanation:** Intervals [1,4] and [4,5] are considered overlapping.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** intervals = [[4,7],[1,4]]
 *     **Output:** [[1,7]]
 *     **Explanation:** Intervals [1,4] and [4,7] are considered overlapping.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= intervals.length <= 104`
 *   * `intervals[i].length == 2`
 *   * `0 <= starti <= endi <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/merge-intervals/
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
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,3],[2,6],[8,10],[15,18]]
    // Test case 2: [[1,4],[4,5]]
    // Test case 3: [[4,7],[1,4]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
