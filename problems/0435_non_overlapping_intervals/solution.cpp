// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 435: Non-overlapping Intervals
 * Difficulty: Medium
 * Tags: Greedy, Array, Dynamic Programming, Sorting
 *
 * Problem Description:
 * Given an array of intervals `intervals` where `intervals[i] = [starti,
 * endi]`,
 * return _the minimum number of intervals you need to remove to make the rest
 * of
 * the intervals non-overlapping_.
 * 
 * **Note** that intervals which only touch at a point are
 * **non-overlapping**. For
 * example, `[1, 2]` and `[2, 3]` are non-overlapping.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** intervals = [[1,2],[2,3],[3,4],[1,3]]
 *     **Output:** 1
 * **Explanation:** [1,3] can be removed and the rest of the intervals are
 * non-overlapping.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** intervals = [[1,2],[1,2],[1,2]]
 *     **Output:** 2
 * **Explanation:** You need to remove two [1,2] to make the rest of the
 * intervals non-overlapping.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** intervals = [[1,2],[2,3]]
 *     **Output:** 0
 * **Explanation:** You don't need to remove any of the intervals since
 * they're already non-overlapping.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= intervals.length <= 105`
 *   * `intervals[i].length == 2`
 *   * `-5 * 104 <= starti < endi <= 5 * 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/non-overlapping-intervals/
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
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2],[2,3],[3,4],[1,3]]
    // Test case 2: [[1,2],[1,2],[1,2]]
    // Test case 3: [[1,2],[2,3]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
