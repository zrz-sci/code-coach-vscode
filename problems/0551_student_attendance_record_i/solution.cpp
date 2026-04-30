// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 551: Student Attendance Record I
 * Difficulty: Easy
 * Tags: String
 *
 * Problem Description:
 * You are given a string `s` representing an attendance record for a student
 * where
 * each character signifies whether the student was absent, late, or present on
 * that day. The record only contains the following three characters:
 * 
 *   * `'A'`: Absent.
 *   * `'L'`: Late.
 *   * `'P'`: Present.
 * 
 * The student is eligible for an attendance award if they meet **both** of the
 * following criteria:
 * 
 * * The student was absent (`'A'`) for **strictly** fewer than 2 days
 * **total**.
 * * The student was **never** late (`'L'`) for 3 or more **consecutive**
 * days.
 * 
 * Return `true` _if the student is eligible for an attendance award,
 * or_`false`
 * _otherwise_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "PPALLP"
 *     **Output:** true
 * **Explanation:** The student has fewer than 2 absences and was never late 3
 * or more consecutive days.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "PPALLL"
 *     **Output:** false
 * **Explanation:** The student was late 3 consecutive days in the last 3
 * days, so is not eligible for the award.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 1000`
 *   * `s[i]` is either `'A'`, `'L'`, or `'P'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/student-attendance-record-i/
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

class Solution {
public:
    bool checkRecord(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "PPALLP"
    // Test case 2: "PPALLL"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
