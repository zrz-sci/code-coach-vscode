// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1335: Minimum Difficulty of a Job Schedule
 * Difficulty: Hard
 * Tags: Array, Dynamic Programming
 *
 * Problem Description:
 * You want to schedule a list of jobs in `d` days. Jobs are dependent (i.e To
 * work
 * on the `ith` job, you have to finish all the jobs `j` where `0 <= j < i`).
 * 
 * You have to finish **at least** one task every day. The difficulty of a job
 * schedule is the sum of difficulties of each day of the `d` days. The
 * difficulty
 * of a day is the maximum difficulty of a job done on that day.
 * 
 * You are given an integer array `jobDifficulty` and an integer `d`. The
 * difficulty of the `ith` job is `jobDifficulty[i]`.
 * 
 * Return _the minimum difficulty of a job schedule_. If you cannot find a
 * schedule
 * for the jobs return `-1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** jobDifficulty = [6,5,4,3,2,1], d = 2
 *     **Output:** 7
 * **Explanation:** First day you can finish the first 5 jobs, total
 * difficulty = 6.
 *     Second day you can finish the last job, total difficulty = 1.
 *     The difficulty of the schedule = 6 + 1 = 7 
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** jobDifficulty = [9,9,9], d = 4
 *     **Output:** -1
 * **Explanation:** If you finish a job per day you will still have a free
 * day. you cannot find a schedule for the given jobs.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** jobDifficulty = [1,1,1], d = 3
 *     **Output:** 3
 * **Explanation:** The schedule is one job per day. total difficulty will be
 * 3.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= jobDifficulty.length <= 300`
 *   * `0 <= jobDifficulty[i] <= 1000`
 *   * `1 <= d <= 10`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-difficulty-of-a-job-schedule/
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
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [6,5,4,3,2,1]
    // Test case 2: 2
    // Test case 3: [9,9,9]
    // Test case 4: 4
    // Test case 5: [1,1,1]
    // Test case 6: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
