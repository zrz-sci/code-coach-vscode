// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 596: Classes With at Least 5 Students
 * Difficulty: Easy
 * Tags: Database
 *
 * Problem Description:
 * Table: `Courses`
 * 
 *     
 *     
 *     +-------------+---------+
 *     | Column Name | Type    |
 *     +-------------+---------+
 *     | student     | varchar |
 *     | class       | varchar |
 *     +-------------+---------+
 * (student, class) is the primary key (combination of columns with unique
 * values) for this table.
 * Each row of this table indicates the name of a student and the class in
 * which they are enrolled.
 *     
 * 
 * 
 * 
 * Write a solution to find all the classes that have **at least five
 * students**.
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
 *     Courses table:
 *     +---------+----------+
 *     | student | class    |
 *     +---------+----------+
 *     | A       | Math     |
 *     | B       | English  |
 *     | C       | Math     |
 *     | D       | Biology  |
 *     | E       | Math     |
 *     | F       | Computer |
 *     | G       | Math     |
 *     | H       | Math     |
 *     | I       | Math     |
 *     +---------+----------+
 *     **Output:** 
 *     +---------+
 *     | class   |
 *     +---------+
 *     | Math    |
 *     +---------+
 *     **Explanation:** 
 *     - Math has 6 students, so we include it.
 *     - English has 1 student, so we do not include it.
 *     - Biology has 1 student, so we do not include it.
 *     - Computer has 1 student, so we do not include it.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/classes-with-at-least-5-students/
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
    // {"headers": {"Courses": ["student", "class"]}, "rows": {"Courses": [["A", "Math"], ["B", "English"], ["C", "Math"], ["D", "Biology"], ["E", "Math"], ["F", "Computer"], ["G", "Math"], ["H", "Math"], ["I", "Math"]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
