// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 177: Nth Highest Salary
 * Difficulty: Medium
 * Tags: Database
 *
 * Problem Description:
 * Table: `Employee`
 * 
 *     
 *     
 *     +-------------+------+
 *     | Column Name | Type |
 *     +-------------+------+
 *     | id          | int  |
 *     | salary      | int  |
 *     +-------------+------+
 *     id is the primary key (column with unique values) for this table.
 * Each row of this table contains information about the salary of an
 * employee.
 *     
 * 
 * 
 * 
 * Write a solution to find the `nth` highest **distinct** salary from the
 * `Employee` table. If there are less than `n` distinct salaries, return
 * `null`.
 * 
 * The result format is in the following example.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** 
 *     Employee table:
 *     +----+--------+
 *     | id | salary |
 *     +----+--------+
 *     | 1  | 100    |
 *     | 2  | 200    |
 *     | 3  | 300    |
 *     +----+--------+
 *     n = 2
 *     **Output:** 
 *     +------------------------+
 *     | getNthHighestSalary(2) |
 *     +------------------------+
 *     | 200                    |
 *     +------------------------+
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** 
 *     Employee table:
 *     +----+--------+
 *     | id | salary |
 *     +----+--------+
 *     | 1  | 100    |
 *     +----+--------+
 *     n = 2
 *     **Output:** 
 *     +------------------------+
 *     | getNthHighestSalary(2) |
 *     +------------------------+
 *     | null                   |
 *     +------------------------+
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/nth-highest-salary/
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



// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: Solution
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // {"headers": {"Employee": ["id", "salary"]}, "argument": 2, "rows": {"Employee": [[1, 100], [2, 200], [3, 300]]}}
    // {"headers": {"Employee": ["id", "salary"]}, "argument": 2, "rows": {"Employee": [[1, 100]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
