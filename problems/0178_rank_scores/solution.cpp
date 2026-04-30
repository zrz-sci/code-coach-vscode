// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 178: Rank Scores
 * Difficulty: Medium
 * Tags: Database
 *
 * Problem Description:
 * Table: `Scores`
 * 
 *     
 *     
 *     +-------------+---------+
 *     | Column Name | Type    |
 *     +-------------+---------+
 *     | id          | int     |
 *     | score       | decimal |
 *     +-------------+---------+
 *     id is the primary key (column with unique values) for this table.
 * Each row of this table contains the score of a game. Score is a floating
 * point value with two decimal places.
 *     
 * 
 * 
 * 
 * Write a solution to find the rank of the scores. The ranking should be
 * calculated according to the following rules:
 * 
 *   * The scores should be ranked from the highest to the lowest.
 *   * If there is a tie between two scores, both should have the same ranking.
 * * After a tie, the next ranking number should be the next consecutive
 * integer value. In other words, there should be no holes between ranks.
 * 
 * Return the result table ordered by `score` in descending order.
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
 *     Scores table:
 *     +----+-------+
 *     | id | score |
 *     +----+-------+
 *     | 1  | 3.50  |
 *     | 2  | 3.65  |
 *     | 3  | 4.00  |
 *     | 4  | 3.85  |
 *     | 5  | 4.00  |
 *     | 6  | 3.65  |
 *     +----+-------+
 *     **Output:** 
 *     +-------+------+
 *     | score | rank |
 *     +-------+------+
 *     | 4.00  | 1    |
 *     | 4.00  | 1    |
 *     | 3.85  | 2    |
 *     | 3.65  | 3    |
 *     | 3.65  | 3    |
 *     | 3.50  | 4    |
 *     +-------+------+
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/rank-scores/
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
    // {"headers": {"Scores": ["id", "score"]}, "rows": {"Scores": [[1, 3.50], [2, 3.65], [3, 4.00], [4, 3.85], [5, 4.00], [6, 3.65]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
