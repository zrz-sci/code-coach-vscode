// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 584: Find Customer Referee
 * Difficulty: Easy
 * Tags: Database
 *
 * Problem Description:
 * Table: `Customer`
 * 
 *     
 *     
 *     +-------------+---------+
 *     | Column Name | Type    |
 *     +-------------+---------+
 *     | id          | int     |
 *     | name        | varchar |
 *     | referee_id  | int     |
 *     +-------------+---------+
 *     In SQL, id is the primary key column for this table.
 * Each row of this table indicates the id of a customer, their name, and the
 * id of the customer who referred them.
 *     
 * 
 * 
 * 
 * Find the names of the customer that are either:
 * 
 *   1. **referred by**  any customer with `id != 2`.
 *   2. **not referred by** any customer.
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
 *     Customer table:
 *     +----+------+------------+
 *     | id | name | referee_id |
 *     +----+------+------------+
 *     | 1  | Will | null       |
 *     | 2  | Jane | null       |
 *     | 3  | Alex | 2          |
 *     | 4  | Bill | null       |
 *     | 5  | Zack | 1          |
 *     | 6  | Mark | 2          |
 *     +----+------+------------+
 *     **Output:** 
 *     +------+
 *     | name |
 *     +------+
 *     | Will |
 *     | Jane |
 *     | Bill |
 *     | Zack |
 *     +------+
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-customer-referee/
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
    // {"headers":{"Customer":["id","name","referee_id"]},"rows":{"Customer":[[1,"Will",null],[2,"Jane",null],[3,"Alex",2],[4,"Bill",null],[5,"Zack",1],[6,"Mark",2]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
