// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1047: Remove All Adjacent Duplicates In String
 * Difficulty: Easy
 * Tags: Stack, String
 *
 * Problem Description:
 * You are given a string `s` consisting of lowercase English letters. A
 * **duplicate removal** consists of choosing two **adjacent** and **equal**
 * letters and removing them.
 * 
 * We repeatedly make **duplicate removals** on `s` until we no longer can.
 * 
 * Return _the final string after all such duplicate removals have been made_.
 * It
 * can be proven that the answer is **unique**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "abbaca"
 *     **Output:** "ca"
 *     **Explanation:** 
 * For example, in "abbaca" we could remove "bb" since the letters are
 * adjacent and equal, and this is the only possible move. The result of this
 * move is that the string is "aaca", of which only "aa" is possible, so the
 * final string is "ca".
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "azxxzy"
 *     **Output:** "ay"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `s` consists of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/remove-all-adjacent-duplicates-in-string/
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
    string removeDuplicates(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "abbaca"
    // Test case 2: "azxxzy"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
