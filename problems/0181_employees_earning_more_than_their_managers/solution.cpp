// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 181: Employees Earning More Than Their Managers
 * Difficulty: Easy
 * Tags: Database
 *
 * Problem Description:
 * Table: `Employee`
 * 
 *     
 *     
 *     +-------------+---------+
 *     | Column Name | Type    |
 *     +-------------+---------+
 *     | id          | int     |
 *     | name        | varchar |
 *     | salary      | int     |
 *     | managerId   | int     |
 *     +-------------+---------+
 *     id is the primary key (column with unique values) for this table.
 * Each row of this table indicates the ID of an employee, their name, salary,
 * and the ID of their manager.
 *     
 * 
 * 
 * 
 * Write a solution to find the employees who earn more than their managers.
 * 
 * Return the result table in **any order**.
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
 *     +----+-------+--------+-----------+
 *     | id | name  | salary | managerId |
 *     +----+-------+--------+-----------+
 *     | 1  | Joe   | 70000  | 3         |
 *     | 2  | Henry | 80000  | 4         |
 *     | 3  | Sam   | 60000  | Null      |
 *     | 4  | Max   | 90000  | Null      |
 *     +----+-------+--------+-----------+
 *     **Output:** 
 *     +----------+
 *     | Employee |
 *     +----------+
 *     | Joe      |
 *     +----------+
 * **Explanation:** Joe is the only employee who earns more than his manager.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/employees-earning-more-than-their-managers/
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
    // {"headers": {"Employee": ["id", "name", "salary", "managerId"]}, "rows": {"Employee": [[1, "Joe", 70000, 3], [2, "Henry", 80000, 4], [3, "Sam", 60000, null], [4, "Max", 90000, null]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
