// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 610: Triangle Judgement
 * Difficulty: Easy
 * Tags: Database
 *
 * Problem Description:
 * Table: `Triangle`
 * 
 *     
 *     
 *     +-------------+------+
 *     | Column Name | Type |
 *     +-------------+------+
 *     | x           | int  |
 *     | y           | int  |
 *     | z           | int  |
 *     +-------------+------+
 *     In SQL, (x, y, z) is the primary key column for this table.
 *     Each row of this table contains the lengths of three line segments.
 *     
 * 
 * 
 * 
 * Report for every three line segments whether they can form a triangle.
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
 *     Triangle table:
 *     +----+----+----+
 *     | x  | y  | z  |
 *     +----+----+----+
 *     | 13 | 15 | 30 |
 *     | 10 | 20 | 15 |
 *     +----+----+----+
 *     **Output:** 
 *     +----+----+----+----------+
 *     | x  | y  | z  | triangle |
 *     +----+----+----+----------+
 *     | 13 | 15 | 30 | No       |
 *     | 10 | 20 | 15 | Yes      |
 *     +----+----+----+----------+
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/triangle-judgement/
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
    // {"headers":{"Triangle":["x","y","z"]},"rows":{"Triangle":[[13,15,30],[10,20,15]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
