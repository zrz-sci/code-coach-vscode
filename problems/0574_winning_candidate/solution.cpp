// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql
/*
 * LeetCode Problem 574: Winning Candidate
 * Difficulty: Medium
 * Tags: Database
 *
 * Problem Description:
 * Table: `Candidate`
 * 
 *     
 *     
 *     +-------------+----------+
 *     | Column Name | Type     |
 *     +-------------+----------+
 *     | id          | int      |
 *     | name        | varchar  |
 *     +-------------+----------+
 *     id is the column with unique values for this table.
 * Each row of this table contains information about the id and the name of a
 * candidate.
 *     
 * 
 * 
 * 
 * Table: `Vote`
 * 
 *     
 *     
 *     +-------------+------+
 *     | Column Name | Type |
 *     +-------------+------+
 *     | id          | int  |
 *     | candidateId | int  |
 *     +-------------+------+
 *     id is an auto-increment primary key (column with unique values).
 * candidateId is a foreign key (reference column) to id from the Candidate
 * table.
 * Each row of this table determines the candidate who got the ith vote in the
 * elections.
 *     
 * 
 * 
 * 
 * Write a solution to report the name of the winning candidate (i.e., the
 * candidate who got the largest number of votes).
 * 
 * The test cases are generated so that **exactly one candidate wins** the
 * elections.
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
 *     Candidate table:
 *     +----+------+
 *     | id | name |
 *     +----+------+
 *     | 1  | A    |
 *     | 2  | B    |
 *     | 3  | C    |
 *     | 4  | D    |
 *     | 5  | E    |
 *     +----+------+
 *     Vote table:
 *     +----+-------------+
 *     | id | candidateId |
 *     +----+-------------+
 *     | 1  | 2           |
 *     | 2  | 4           |
 *     | 3  | 3           |
 *     | 4  | 2           |
 *     | 5  | 5           |
 *     +----+-------------+
 *     **Output:** 
 *     +------+
 *     | name |
 *     +------+
 *     | B    |
 *     +------+
 *     **Explanation:** 
 *     Candidate B has 2 votes. Candidates C, D, and E have 1 vote each.
 *     The winner is candidate B.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/winning-candidate/
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
    // {"headers":{"Candidate":["id","name"],"Vote":["id","candidateId"]},"rows":{"Candidate":[[1,"A"],[2,"B"],[3,"C"],[4,"D"],[5,"E"]],"Vote":[[1,2],[2,4],[3,3],[4,2],[5,5]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
