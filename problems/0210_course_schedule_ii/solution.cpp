// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 210: Course Schedule II
 * Difficulty: Medium
 * Tags: Depth-First Search, Breadth-First Search, Graph, Topological Sort
 *
 * Problem Description:
 * There are a total of `numCourses` courses you have to take, labeled from
 * `0` to
 * `numCourses - 1`. You are given an array `prerequisites` where
 * `prerequisites[i]
 * = [ai, bi]` indicates that you **must** take course `bi` first if you want
 * to
 * take course `ai`.
 * 
 * * For example, the pair `[0, 1]`, indicates that to take course `0` you
 * have to first take course `1`.
 * 
 * Return _the ordering of courses you should take to finish all courses_. If
 * there
 * are many valid answers, return **any** of them. If it is impossible to
 * finish
 * all courses, return **an empty array**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** numCourses = 2, prerequisites = [[1,0]]
 *     **Output:** [0,1]
 * **Explanation:** There are a total of 2 courses to take. To take course 1
 * you should have finished course 0. So the correct course order is [0,1].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** numCourses = 4, prerequisites = [[1,0],[2,0],[3,1],[3,2]]
 *     **Output:** [0,2,1,3]
 * **Explanation:** There are a total of 4 courses to take. To take course 3
 * you should have finished both courses 1 and 2. Both courses 1 and 2 should
 * be taken after you finished course 0.
 * So one correct course order is [0,1,2,3]. Another correct ordering is
 * [0,2,1,3].
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** numCourses = 1, prerequisites = []
 *     **Output:** [0]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= numCourses <= 2000`
 *   * `0 <= prerequisites.length <= numCourses * (numCourses - 1)`
 *   * `prerequisites[i].length == 2`
 *   * `0 <= ai, bi < numCourses`
 *   * `ai != bi`
 *   * All the pairs `[ai, bi]` are **distinct**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/course-schedule-ii/
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
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 2
    // Test case 2: [[1,0]]
    // Test case 3: 4
    // Test case 4: [[1,0],[2,0],[3,1],[3,2]]
    // Test case 5: 1
    // Test case 6: []

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
