// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 582: Kill Process
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Breadth-First Search, Array, Hash Table
 *
 * Problem Description:
 * You have `n` processes forming a rooted tree structure. You are given two
 * integer arrays `pid` and `ppid`, where `pid[i]` is the ID of the `ith`
 * process
 * and `ppid[i]` is the ID of the `ith` process's parent process.
 * 
 * Each process has only **one parent process** but may have multiple children
 * processes. Only one process has `ppid[i] = 0`, which means this process has
 * **no
 * parent process** (the root of the tree).
 * 
 * When a process is **killed** , all of its children processes will also be
 * killed.
 * 
 * Given an integer `kill` representing the ID of a process you want to kill,
 * return _a list of the IDs of the processes that will be killed. You may
 * return
 * the answer in**any order**._
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** pid = [1,3,10,5], ppid = [3,0,5,3], kill = 5
 *     **Output:** [5,10]
 * **Explanation:** The processes colored in red are the processes that should
 * be killed.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** pid = [1], ppid = [0], kill = 1
 *     **Output:** [1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == pid.length`
 *   * `n == ppid.length`
 *   * `1 <= n <= 5 * 104`
 *   * `1 <= pid[i] <= 5 * 104`
 *   * `0 <= ppid[i] <= 5 * 104`
 *   * Only one process has no parent.
 *   * All the values of `pid` are **unique**.
 *   * `kill` is **guaranteed** to be in `pid`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/kill-process/
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
    vector<int> killProcess(vector<int>& pid, vector<int>& ppid, int kill) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,10,5]
    // Test case 2: [3,0,5,3]
    // Test case 3: 5
    // Test case 4: [1]
    // Test case 5: [0]
    // Test case 6: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
