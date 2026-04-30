// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 759: Employee Free Time
 * Difficulty: Hard
 * Tags: Array, Sorting, Sweep Line, Heap (Priority Queue)
 *
 * Problem Description:
 * We are given a list `schedule` of employees, which represents the working
 * time
 * for each employee.
 * 
 * Each employee has a list of non-overlapping `Intervals`, and these
 * intervals are
 * in sorted order.
 * 
 * Return the list of finite intervals representing **common, positive-length
 * free
 * time** for _all_ employees, also in sorted order.
 * 
 * (Even though we are representing `Intervals` in the form `[x, y]`, the
 * objects
 * inside are `Intervals`, not lists or arrays. For example,
 * `schedule[0][0].start
 * = 1`, `schedule[0][0].end = 2`, and `schedule[0][0][0]` is not defined).
 * Also,
 * we wouldn't include intervals like [5, 5] in our answer, as they have zero
 * length.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** schedule = [[[1,2],[5,6]],[[1,3]],[[4,10]]]
 *     **Output:** [[3,4]]
 *     **Explanation:** There are a total of three employees, and all common
 *     free time intervals would be [-inf, 1], [3, 4], [10, inf].
 *     We discard any intervals that contain inf as they aren't finite.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** schedule = [[[1,3],[6,7]],[[2,4]],[[2,5],[9,12]]]
 *     **Output:** [[5,6],[7,9]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= schedule.length , schedule[i].length <= 50`
 *   * `0 <= schedule[i].start < schedule[i].end <= 10^8`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/employee-free-time/
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

/*
// Definition for an Interval.
class Interval {
public:
    int start;
    int end;

    Interval() {}

    Interval(int _start, int _end) {
        start = _start;
        end = _end;
    }
};
*/

class Solution {
public:
    vector<Interval> employeeFreeTime(vector<vector<Interval>> schedule) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[[1,2],[5,6]],[[1,3]],[[4,10]]]
    // Test case 2: [[[1,3],[6,7]],[[2,4]],[[2,5],[9,12]]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
