// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 603: Consecutive Available Seats
 * Difficulty: Easy
 * Tags: Database
 *
 * Problem Description:
 * Table: `Cinema`
 * 
 *     
 *     
 *     +-------------+------+
 *     | Column Name | Type |
 *     +-------------+------+
 *     | seat_id     | int  |
 *     | free        | bool |
 *     +-------------+------+
 *     seat_id is an auto-increment column for this table.
 * Each row of this table indicates whether the ith seat is free or not. 1
 * means free while 0 means occupied.
 *     
 * 
 * 
 * 
 * Find all the consecutive available seats in the cinema.
 * 
 * Return the result table **ordered** by `seat_id` **in ascending order**.
 * 
 * The test cases are generated so that more than two seats are consecutively
 * available.
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
 *     Cinema table:
 *     +---------+------+
 *     | seat_id | free |
 *     +---------+------+
 *     | 1       | 1    |
 *     | 2       | 0    |
 *     | 3       | 1    |
 *     | 4       | 1    |
 *     | 5       | 1    |
 *     +---------+------+
 *     **Output:** 
 *     +---------+
 *     | seat_id |
 *     +---------+
 *     | 3       |
 *     | 4       |
 *     | 5       |
 *     +---------+
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/consecutive-available-seats/
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
    // {"headers":{"Cinema":["seat_id","free"]},"rows":{"Cinema":[[1,1],[2,0],[3,1],[4,1],[5,1]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
