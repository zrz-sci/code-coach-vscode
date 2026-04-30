// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 571: Find Median Given Frequency of Numbers
 * Difficulty: Hard
 * Tags: Database
 *
 * Problem Description:
 * Table: `Numbers`
 * 
 *     
 *     
 *     +-------------+------+
 *     | Column Name | Type |
 *     +-------------+------+
 *     | num         | int  |
 *     | frequency   | int  |
 *     +-------------+------+
 *     num is the primary key (column with unique values) for this table.
 *     Each row of this table shows the frequency of a number in the database.
 *     
 * 
 * 
 * 
 * The [**median**](https://en.wikipedia.org/wiki/Median) is the value
 * separating
 * the higher half from the lower half of a data sample.
 * 
 * Write a solution to report the **median** of all the numbers in the database
 * after decompressing the `Numbers` table. Round the median to **one decimal
 * point**.
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
 *     Numbers table:
 *     +-----+-----------+
 *     | num | frequency |
 *     +-----+-----------+
 *     | 0   | 7         |
 *     | 1   | 1         |
 *     | 2   | 3         |
 *     | 3   | 1         |
 *     +-----+-----------+
 *     **Output:** 
 *     +--------+
 *     | median |
 *     +--------+
 *     | 0.0    |
 *     +--------+
 *     **Explanation:** 
 * If we decompress the Numbers table, we will get [0, 0, 0, 0, 0, 0, 0, 1, 2,
 * 2, 2, 3], so the median is (0 + 0) / 2 = 0.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-median-given-frequency-of-numbers/
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
    // {"headers": {"Numbers": ["num", "frequency"]}, "rows": {"Numbers": [[0, 7], [1, 1], [2, 3], [3, 1]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
