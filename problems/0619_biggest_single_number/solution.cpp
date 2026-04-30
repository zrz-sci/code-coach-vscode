// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 619: Biggest Single Number
 * Difficulty: Easy
 * Tags: Database
 *
 * Problem Description:
 * Table: `MyNumbers`
 * 
 *     
 *     
 *     +-------------+------+
 *     | Column Name | Type |
 *     +-------------+------+
 *     | num         | int  |
 *     +-------------+------+
 * This table may contain duplicates (In other words, there is no primary key
 * for this table in SQL).
 *     Each row of this table contains an integer.
 *     
 * 
 * 
 * 
 * A **single number** is a number that appeared only once in the `MyNumbers`
 * table.
 * 
 * Find the largest **single number**. If there is no **single number** ,
 * report
 * `null`.
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
 *     MyNumbers table:
 *     +-----+
 *     | num |
 *     +-----+
 *     | 8   |
 *     | 8   |
 *     | 3   |
 *     | 3   |
 *     | 1   |
 *     | 4   |
 *     | 5   |
 *     | 6   |
 *     +-----+
 *     **Output:** 
 *     +-----+
 *     | num |
 *     +-----+
 *     | 6   |
 *     +-----+
 *     **Explanation:** The single numbers are 1, 4, 5, and 6.
 *     Since 6 is the largest single number, we return it.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** 
 *     MyNumbers table:
 *     +-----+
 *     | num |
 *     +-----+
 *     | 8   |
 *     | 8   |
 *     | 7   |
 *     | 7   |
 *     | 3   |
 *     | 3   |
 *     | 3   |
 *     +-----+
 *     **Output:** 
 *     +------+
 *     | num  |
 *     +------+
 *     | null |
 *     +------+
 * **Explanation:** There are no single numbers in the input table so we
 * return null.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/biggest-single-number/
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
    // {"headers": {"MyNumbers": ["num"]}, "rows": {"MyNumbers": [[8],[8],[3],[3],[1],[4],[5],[6]]}}
    // {"headers": {"MyNumbers": ["num"]}, "rows": {"MyNumbers": [[8],[8],[7],[7],[3],[3],[3]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
