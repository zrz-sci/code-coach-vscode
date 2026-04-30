// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 183: Customers Who Never Order
 * Difficulty: Easy
 * Tags: Database
 *
 * Problem Description:
 * Table: `Customers`
 * 
 *     
 *     
 *     +-------------+---------+
 *     | Column Name | Type    |
 *     +-------------+---------+
 *     | id          | int     |
 *     | name        | varchar |
 *     +-------------+---------+
 *     id is the primary key (column with unique values) for this table.
 *     Each row of this table indicates the ID and name of a customer.
 *     
 * 
 * 
 * 
 * Table: `Orders`
 * 
 *     
 *     
 *     +-------------+------+
 *     | Column Name | Type |
 *     +-------------+------+
 *     | id          | int  |
 *     | customerId  | int  |
 *     +-------------+------+
 *     id is the primary key (column with unique values) for this table.
 * customerId is a foreign key (reference columns) of the ID from the
 * Customers table.
 * Each row of this table indicates the ID of an order and the ID of the
 * customer who ordered it.
 *     
 * 
 * 
 * 
 * Write a solution to find all customers who never order anything.
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
 *     Customers table:
 *     +----+-------+
 *     | id | name  |
 *     +----+-------+
 *     | 1  | Joe   |
 *     | 2  | Henry |
 *     | 3  | Sam   |
 *     | 4  | Max   |
 *     +----+-------+
 *     Orders table:
 *     +----+------------+
 *     | id | customerId |
 *     +----+------------+
 *     | 1  | 3          |
 *     | 2  | 1          |
 *     +----+------------+
 *     **Output:** 
 *     +-----------+
 *     | Customers |
 *     +-----------+
 *     | Henry     |
 *     | Max       |
 *     +-----------+
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/customers-who-never-order/
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
    // {"headers": {"Customers": ["id", "name"], "Orders": ["id", "customerId"]}, "rows": {"Customers": [[1, "Joe"], [2, "Henry"], [3, "Sam"], [4, "Max"]], "Orders": [[1, 3], [2, 1]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
