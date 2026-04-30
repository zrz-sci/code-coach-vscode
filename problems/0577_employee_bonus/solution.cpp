// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 577: Employee Bonus
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
 *     | empId       | int     |
 *     | name        | varchar |
 *     | supervisor  | int     |
 *     | salary      | int     |
 *     +-------------+---------+
 *     empId is the column with unique values for this table.
 * Each row of this table indicates the name and the ID of an employee in
 * addition to their salary and the id of their manager.
 *     
 * 
 * 
 * 
 * Table: `Bonus`
 * 
 *     
 *     
 *     +-------------+------+
 *     | Column Name | Type |
 *     +-------------+------+
 *     | empId       | int  |
 *     | bonus       | int  |
 *     +-------------+------+
 *     empId is the column of unique values for this table.
 * empId is a foreign key (reference column) to empId from the Employee table.
 * Each row of this table contains the id of an employee and their respective
 * bonus.
 *     
 * 
 * 
 * 
 * Write a solution to report the name and bonus amount of each employee who
 * satisfies either of the following:
 * 
 *   * The employee has a bonus **less than** `1000`.
 *   * The employee did not get any bonus.
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
 *     +-------+--------+------------+--------+
 *     | empId | name   | supervisor | salary |
 *     +-------+--------+------------+--------+
 *     | 3     | Brad   | null       | 4000   |
 *     | 1     | John   | 3          | 1000   |
 *     | 2     | Dan    | 3          | 2000   |
 *     | 4     | Thomas | 3          | 4000   |
 *     +-------+--------+------------+--------+
 *     Bonus table:
 *     +-------+-------+
 *     | empId | bonus |
 *     +-------+-------+
 *     | 2     | 500   |
 *     | 4     | 2000  |
 *     +-------+-------+
 *     **Output:** 
 *     +------+-------+
 *     | name | bonus |
 *     +------+-------+
 *     | Brad | null  |
 *     | John | null  |
 *     | Dan  | 500   |
 *     +------+-------+
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/employee-bonus/
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
    // {"headers":{"Employee":["empId","name","supervisor","salary"],"Bonus":["empId","bonus"]},"rows":{"Employee":[[3,"Brad",null,4000],[1,"John",3,1000],[2,"Dan",3,2000],[4,"Thomas",3,4000]],"Bonus":[[2,500],[4,2000]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
