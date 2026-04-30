// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 627: Swap Sex of Employees
 * Difficulty: Easy
 * Tags: Database
 *
 * Problem Description:
 * Table: `Salary`
 * 
 *     
 *     
 *     +-------------+----------+
 *     | Column Name | Type     |
 *     +-------------+----------+
 *     | id          | int      |
 *     | name        | varchar  |
 *     | sex         | ENUM     |
 *     | salary      | int      |
 *     +-------------+----------+
 *     id is the primary key (column with unique values) for this table.
 *     The sex column is ENUM (category) value of type ('m', 'f').
 *     The table contains information about an employee.
 *     
 * 
 * 
 * 
 * Write a solution to swap all `'f'` and `'m'` values (i.e., change all `'f'`
 * values to `'m'` and vice versa) with a **single update statement** and no
 * intermediate temporary tables.
 * 
 * Note that you must write a single update statement, **do not** write any
 * select
 * statement for this problem.
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
 *     Salary table:
 *     +----+------+-----+--------+
 *     | id | name | sex | salary |
 *     +----+------+-----+--------+
 *     | 1  | A    | m   | 2500   |
 *     | 2  | B    | f   | 1500   |
 *     | 3  | C    | m   | 5500   |
 *     | 4  | D    | f   | 500    |
 *     +----+------+-----+--------+
 *     **Output:** 
 *     +----+------+-----+--------+
 *     | id | name | sex | salary |
 *     +----+------+-----+--------+
 *     | 1  | A    | f   | 2500   |
 *     | 2  | B    | m   | 1500   |
 *     | 3  | C    | f   | 5500   |
 *     | 4  | D    | m   | 500    |
 *     +----+------+-----+--------+
 *     **Explanation:** 
 *     (1, A) and (3, C) were changed from 'm' to 'f'.
 *     (2, B) and (4, D) were changed from 'f' to 'm'.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/swap-sex-of-employees/
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
    // {"headers":{"Salary":["id","name","sex","salary"]},"rows":{"Salary":[[1,"A","m",2500],[2,"B","f",1500],[3,"C","m",5500],[4,"D","f",500]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
