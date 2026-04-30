// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 601: Human Traffic of Stadium
 * Difficulty: Hard
 * Tags: Database
 *
 * Problem Description:
 * Table: `Stadium`
 * 
 *     
 *     
 *     +---------------+---------+
 *     | Column Name   | Type    |
 *     +---------------+---------+
 *     | id            | int     |
 *     | visit_date    | date    |
 *     | people        | int     |
 *     +---------------+---------+
 *     visit_date is the column with unique values for this table.
 * Each row of this table contains the visit date and visit id to the stadium
 * with the number of people during the visit.
 *     As the id increases, the date increases as well.
 *     
 * 
 * 
 * 
 * Write a solution to display the records with three or more rows with
 * **consecutive** `id`'s, and the number of people is greater than or equal
 * to 100
 * for each.
 * 
 * Return the result table ordered by `visit_date` in **ascending order**.
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
 *     Stadium table:
 *     +------+------------+-----------+
 *     | id   | visit_date | people    |
 *     +------+------------+-----------+
 *     | 1    | 2017-01-01 | 10        |
 *     | 2    | 2017-01-02 | 109       |
 *     | 3    | 2017-01-03 | 150       |
 *     | 4    | 2017-01-04 | 99        |
 *     | 5    | 2017-01-05 | 145       |
 *     | 6    | 2017-01-06 | 1455      |
 *     | 7    | 2017-01-07 | 199       |
 *     | 8    | 2017-01-09 | 188       |
 *     +------+------------+-----------+
 *     **Output:** 
 *     +------+------------+-----------+
 *     | id   | visit_date | people    |
 *     +------+------------+-----------+
 *     | 5    | 2017-01-05 | 145       |
 *     | 6    | 2017-01-06 | 1455      |
 *     | 7    | 2017-01-07 | 199       |
 *     | 8    | 2017-01-09 | 188       |
 *     +------+------------+-----------+
 *     **Explanation:** 
 * The four rows with ids 5, 6, 7, and 8 have consecutive ids and each of them
 * has >= 100 people attended. Note that row 8 was included even though the
 * visit_date was not the next day after row 7.
 * The rows with ids 2 and 3 are not included because we need at least three
 * consecutive ids.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/human-traffic-of-stadium/
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
    // {"headers": {"Stadium": ["id", "visit_date", "people"]}, "rows": {"Stadium": [[1, "2017-01-01", 10], [2, "2017-01-02", 109], [3, "2017-01-03", 150], [4, "2017-01-04", 99], [5, "2017-01-05", 145], [6, "2017-01-06", 1455], [7, "2017-01-07", 199], [8, "2017-01-09", 188]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
