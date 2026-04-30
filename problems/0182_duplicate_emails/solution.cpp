// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 182: Duplicate Emails
 * Difficulty: Easy
 * Tags: Database
 *
 * Problem Description:
 * Table: `Person`
 * 
 *     
 *     
 *     +-------------+---------+
 *     | Column Name | Type    |
 *     +-------------+---------+
 *     | id          | int     |
 *     | email       | varchar |
 *     +-------------+---------+
 *     id is the primary key (column with unique values) for this table.
 * Each row of this table contains an email. The emails will not contain
 * uppercase letters.
 *     
 * 
 * 
 * 
 * Write a solution to report all the duplicate emails. Note that it's
 * guaranteed
 * that the email field is not NULL.
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
 *     Person table:
 *     +----+---------+
 *     | id | email   |
 *     +----+---------+
 *     | 1  | a@b.com |
 *     | 2  | c@d.com |
 *     | 3  | a@b.com |
 *     +----+---------+
 *     **Output:** 
 *     +---------+
 *     | Email   |
 *     +---------+
 *     | a@b.com |
 *     +---------+
 *     **Explanation:** a@b.com is repeated two times.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/duplicate-emails/
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
    // {"headers": {"Person": ["id", "email"]}, "rows": {"Person": [[1, "a@b.com"], [2, "c@d.com"], [3, "a@b.com"]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
