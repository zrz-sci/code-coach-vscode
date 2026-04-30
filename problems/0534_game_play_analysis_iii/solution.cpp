// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.sql

/*
 * LeetCode Problem 534: Game Play Analysis III
 * Difficulty: Medium
 * Tags: Database
 *
 * Problem Description:
 * Table: `Activity`
 * 
 *     
 *     
 *     +--------------+---------+
 *     | Column Name  | Type    |
 *     +--------------+---------+
 *     | player_id    | int     |
 *     | device_id    | int     |
 *     | event_date   | date    |
 *     | games_played | int     |
 *     +--------------+---------+
 * (player_id, event_date) is the primary key (column with unique values) of
 * this table.
 *     This table shows the activity of players of some games.
 * Each row is a record of a player who logged in and played a number of games
 * (possibly 0) before logging out on someday using some device.
 *     
 * 
 * 
 * 
 * Write a solution to report for each player and date, how many games played
 * **so
 * far** by the player. That is, the total number of games played by the player
 * until that date. Check the example for clarity.
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
 *     Activity table:
 *     +-----------+-----------+------------+--------------+
 *     | player_id | device_id | event_date | games_played |
 *     +-----------+-----------+------------+--------------+
 *     | 1         | 2         | 2016-03-01 | 5            |
 *     | 1         | 2         | 2016-05-02 | 6            |
 *     | 1         | 3         | 2017-06-25 | 1            |
 *     | 3         | 1         | 2016-03-02 | 0            |
 *     | 3         | 4         | 2018-07-03 | 5            |
 *     +-----------+-----------+------------+--------------+
 *     **Output:** 
 *     +-----------+------------+---------------------+
 *     | player_id | event_date | games_played_so_far |
 *     +-----------+------------+---------------------+
 *     | 1         | 2016-03-01 | 5                   |
 *     | 1         | 2016-05-02 | 11                  |
 *     | 1         | 2017-06-25 | 12                  |
 *     | 3         | 2016-03-02 | 0                   |
 *     | 3         | 2018-07-03 | 5                   |
 *     +-----------+------------+---------------------+
 *     **Explanation:** 
 * For the player with id 1, 5 + 6 = 11 games played by 2016-05-02, and 5 + 6
 * + 1 = 12 games played by 2017-06-25.
 *     For the player with id 3, 0 + 5 = 5 games played by 2018-07-03.
 * Note that for each player we only care about the days when the player
 * logged in.
 *     
 * 
 * 
 *
 * Link: https://leetcode.com/problems/game-play-analysis-iii/
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
    // {"headers":{"Activity":["player_id","device_id","event_date","games_played"]},"rows":{"Activity":[[1,2,"2016-03-01",5],[1,2,"2016-05-02",6],[1,3,"2017-06-25",1],[3,1,"2016-03-02",0],[3,4,"2018-07-03",5]]}}

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
