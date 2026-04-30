// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 690: Employee Importance
 * Difficulty: Medium
 * Tags: Tree, Depth-First Search, Breadth-First Search, Array, Hash Table
 *
 * Problem Description:
 * You have a data structure of employee information, including the employee's
 * unique ID, importance value, and direct subordinates' IDs.
 * 
 * You are given an array of employees `employees` where:
 * 
 *   * `employees[i].id` is the ID of the `ith` employee.
 *   * `employees[i].importance` is the importance value of the `ith` employee.
 * * `employees[i].subordinates` is a list of the IDs of the direct
 * subordinates of the `ith` employee.
 * 
 * Given an integer `id` that represents an employee's ID, return _the**total**
 * importance value of this employee and all their direct and indirect
 * subordinates_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** employees = [[1,5,[2,3]],[2,3,[]],[3,3,[]]], id = 1
 *     **Output:** 11
 * **Explanation:** Employee 1 has an importance value of 5 and has two direct
 * subordinates: employee 2 and employee 3.
 *     They both have an importance value of 3.
 *     Thus, the total importance value of employee 1 is 5 + 3 + 3 = 11.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** employees = [[1,2,[5]],[5,-3,[]]], id = 5
 *     **Output:** -3
 * **Explanation:** Employee 5 has an importance value of -3 and has no direct
 * subordinates.
 *     Thus, the total importance value of employee 5 is -3.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= employees.length <= 2000`
 *   * `1 <= employees[i].id <= 2000`
 *   * All `employees[i].id` are **unique**.
 *   * `-100 <= employees[i].importance <= 100`
 * * One employee has at most one direct leader and may have several
 * subordinates.
 *   * The IDs in `employees[i].subordinates` are valid IDs.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/employee-importance/
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
// Definition for Employee.
class Employee {
public:
    int id;
    int importance;
    vector<int> subordinates;
};
*/

class Solution {
public:
    int getImportance(vector<Employee*> employees, int id) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,5,[2,3]],[2,3,[]],[3,3,[]]]
    // Test case 2: 1
    // Test case 3: [[1,2,[5]],[5,-3,[]]]
    // Test case 4: 5

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
