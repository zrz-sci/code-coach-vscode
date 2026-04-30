// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 612: Shortest Distance in a Plane
 * Difficulty: Medium
 * Tags: Database
 *
 * Problem Description:
 * Table: `Point2D`
 * 
 *     
 *     
 *     +-------------+------+
 *     | Column Name | Type |
 *     +-------------+------+
 *     | x           | int  |
 *     | y           | int  |
 *     +-------------+------+
 * (x, y) is the primary key column (combination of columns with unique
 * values) for this table.
 * Each row of this table indicates the position of a point on the X-Y plane.
 *     
 * 
 * 
 * 
 * The distance between two points `p1(x1, y1)` and `p2(x2, y2)` is `sqrt((x2 -
 * x1)2 + (y2 - y1)2)`.
 * 
 * Write a solution to report the shortest distance between any two points
 * from the
 * `Point2D` table. Round the distance to **two decimal points**.
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
 *     Point2D table:
 *     +----+----+
 *     | x  | y  |
 *     +----+----+
 *     | -1 | -1 |
 *     | 0  | 0  |
 *     | -1 | -2 |
 *     +----+----+
 *     **Output:** 
 *     +----------+
 *     | shortest |
 *     +----------+
 *     | 1.00     |
 *     +----------+
 * **Explanation:** The shortest distance is 1.00 from point (-1, -1) to (-1,
 * 2).
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shortest-distance-in-a-plane/
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
    // {"headers":{"Point2D":["x","y"]},"rows":{"Point2D":[[-1,-1],[0,0],[-1,-2]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
