// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 253: Meeting Rooms II
 * Difficulty: Medium
 * Tags: Greedy, Array, Two Pointers, Prefix Sum, Sorting, Heap (Priority Queue)
 *
 * Problem Description:
 * Given an array of meeting time intervals `intervals` where `intervals[i] =
 * [starti, endi]`, return _the minimum number of conference rooms required_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** intervals = [[0,30],[5,10],[15,20]]
 *     **Output:** 2
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** intervals = [[7,10],[2,4]]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= intervals.length <= 104`
 *   * `0 <= starti < endi <= 106`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/meeting-rooms-ii/
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
    int minMeetingRooms(vector<vector<int>>& intervals) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[0,30],[5,10],[15,20]]
    // Test case 2: [[7,10],[2,4]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
