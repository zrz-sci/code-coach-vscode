// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1353: Maximum Number of Events That Can Be Attended
 * Difficulty: Medium
 * Tags: Greedy, Array, Sorting, Heap (Priority Queue)
 *
 * Problem Description:
 * You are given an array of `events` where `events[i] = [startDayi, endDayi]`.
 * Every event `i` starts at `startDayi` and ends at `endDayi`.
 * 
 * You can attend an event `i` at any day `d` where `startDayi <= d <=
 * endDayi`.
 * You can only attend one event at any time `d`.
 * 
 * Return _the maximum number of events you can attend_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** events = [[1,2],[2,3],[3,4]]
 *     **Output:** 3
 *     **Explanation:** You can attend all the three events.
 *     One way to attend them all is as shown.
 *     Attend the first event on day 1.
 *     Attend the second event on day 2.
 *     Attend the third event on day 3.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** events= [[1,2],[2,3],[3,4],[1,2]]
 *     **Output:** 4
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= events.length <= 105`
 *   * `events[i].length == 2`
 *   * `1 <= startDayi <= endDayi <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/maximum-number-of-events-that-can-be-attended/
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
    int maxEvents(vector<vector<int>>& events) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,2],[2,3],[3,4]]
    // Test case 2: [[1,2],[2,3],[3,4],[1,2]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
