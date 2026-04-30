// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 618: Students Report By Geography
 * Difficulty: Hard
 * Tags: Database
 *
 * Problem Description:
 * Table: `Student`
 * 
 *     
 *     
 *     +-------------+---------+
 *     | Column Name | Type    |
 *     +-------------+---------+
 *     | name        | varchar |
 *     | continent   | varchar |
 *     +-------------+---------+
 *     This table may contain duplicate rows.
 * Each row of this table indicates the name of a student and the continent
 * they came from.
 *     
 * 
 * 
 * 
 * A school has students from Asia, Europe, and America.
 * 
 * Write a solution to [pivot](https://en.wikipedia.org/wiki/Pivot_table) the
 * continent column in the `Student` table so that each name is **sorted
 * alphabetically** and displayed underneath its corresponding continent. The
 * output headers should be `America`, `Asia`, and `Europe`, respectively.
 * 
 * The test cases are generated so that the student number from America is not
 * less
 * than either Asia or Europe.
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
 *     Student table:
 *     +--------+-----------+
 *     | name   | continent |
 *     +--------+-----------+
 *     | Jane   | America   |
 *     | Pascal | Europe    |
 *     | Xi     | Asia      |
 *     | Jack   | America   |
 *     +--------+-----------+
 *     **Output:** 
 *     +---------+------+--------+
 *     | America | Asia | Europe |
 *     +---------+------+--------+
 *     | Jack    | Xi   | Pascal |
 *     | Jane    | null | null   |
 *     +---------+------+--------+
 *     
 * 
 * 
 * 
 * **Follow up:** If it is unknown which continent has the most students,
 * could you
 * write a solution to generate the student report?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/students-report-by-geography/
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
    // {"headers":{"Student":["name","continent"]},"rows":{"Student":[["Jane","America"],["Pascal","Europe"],["Xi","Asia"],["Jack","America"]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
