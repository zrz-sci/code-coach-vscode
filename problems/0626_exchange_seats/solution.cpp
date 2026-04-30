// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 626: Exchange Seats
 * Difficulty: Medium
 * Tags: Database
 *
 * Problem Description:
 * Table: `Seat`
 * 
 *     
 *     
 *     +-------------+---------+
 *     | Column Name | Type    |
 *     +-------------+---------+
 *     | id          | int     |
 *     | student     | varchar |
 *     +-------------+---------+
 *     id is the primary key (unique value) column for this table.
 *     Each row of this table indicates the name and the ID of a student.
 *     The ID sequence always starts from 1 and increments continuously.
 *     
 * 
 * 
 * 
 * Write a solution to swap the seat id of every two consecutive students. If
 * the
 * number of students is odd, the id of the last student is not swapped.
 * 
 * Return the result table ordered by `id` **in ascending order**.
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
 *     Seat table:
 *     +----+---------+
 *     | id | student |
 *     +----+---------+
 *     | 1  | Abbot   |
 *     | 2  | Doris   |
 *     | 3  | Emerson |
 *     | 4  | Green   |
 *     | 5  | Jeames  |
 *     +----+---------+
 *     **Output:** 
 *     +----+---------+
 *     | id | student |
 *     +----+---------+
 *     | 1  | Doris   |
 *     | 2  | Abbot   |
 *     | 3  | Green   |
 *     | 4  | Emerson |
 *     | 5  | Jeames  |
 *     +----+---------+
 *     **Explanation:** 
 * Note that if the number of students is odd, there is no need to change the
 * last one's seat.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/exchange-seats/
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
    // {"headers": {"Seat": ["id","student"]}, "rows": {"Seat": [[1,"Abbot"],[2,"Doris"],[3,"Emerson"],[4,"Green"],[5,"Jeames"]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
