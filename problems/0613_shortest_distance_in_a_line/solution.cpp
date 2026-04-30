// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 613: Shortest Distance in a Line
 * Difficulty: Easy
 * Tags: Database
 *
 * Problem Description:
 * Table: `Point`
 * 
 *     
 *     
 *     +-------------+------+
 *     | Column Name | Type |
 *     +-------------+------+
 *     | x           | int  |
 *     +-------------+------+
 *     In SQL, x is the primary key column for this table.
 *     Each row of this table indicates the position of a point on the X-axis.
 *     
 * 
 * 
 * 
 * Find the shortest distance between any two points from the `Point` table.
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
 *     Point table:
 *     +----+
 *     | x  |
 *     +----+
 *     | -1 |
 *     | 0  |
 *     | 2  |
 *     +----+
 *     **Output:** 
 *     +----------+
 *     | shortest |
 *     +----------+
 *     | 1        |
 *     +----------+
 * **Explanation:** The shortest distance is between points -1 and 0 which is
 * |(-1) - 0| = 1.
 *     
 * 
 * 
 * 
 * **Follow up:** How could you optimize your solution if the `Point` table is
 * ordered **in ascending order**?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shortest-distance-in-a-line/
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
    // {"headers":{"Point":["x"]},"rows":{"Point":[[-1],[0],[2]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
