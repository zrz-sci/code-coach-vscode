// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 180: Consecutive Numbers
 * Difficulty: Medium
 * Tags: Database
 *
 * Problem Description:
 * Table: `Logs`
 * 
 *     
 *     
 *     +-------------+---------+
 *     | Column Name | Type    |
 *     +-------------+---------+
 *     | id          | int     |
 *     | num         | varchar |
 *     +-------------+---------+
 *     In SQL, id is the primary key for this table.
 *     id is an autoincrement column starting from 1.
 *     
 * 
 * 
 * 
 * Find all numbers that appear at least three times consecutively.
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
 *     Logs table:
 *     +----+-----+
 *     | id | num |
 *     +----+-----+
 *     | 1  | 1   |
 *     | 2  | 1   |
 *     | 3  | 1   |
 *     | 4  | 2   |
 *     | 5  | 1   |
 *     | 6  | 2   |
 *     | 7  | 2   |
 *     +----+-----+
 *     **Output:** 
 *     +-----------------+
 *     | ConsecutiveNums |
 *     +-----------------+
 *     | 1               |
 *     +-----------------+
 * **Explanation:** 1 is the only number that appears consecutively for at
 * least three times.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/consecutive-numbers/
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
    // {"headers":{"Logs":["id","num"]},"rows":{"Logs":[[1,1],[2,1],[3,1],[4,2],[5,1],[6,2],[7,2]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
