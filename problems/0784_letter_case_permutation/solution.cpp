// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 784: Letter Case Permutation
 * Difficulty: Medium
 * Tags: Bit Manipulation, String, Backtracking
 *
 * Problem Description:
 * Given a string `s`, you can transform every letter individually to be
 * lowercase
 * or uppercase to create another string.
 * 
 * Return _a list of all possible strings we could create_. Return the output
 * in
 * **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "a1b2"
 *     **Output:** ["a1b2","a1B2","A1b2","A1B2"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "3z4"
 *     **Output:** ["3z4","3Z4"]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 12`
 * * `s` consists of lowercase English letters, uppercase English letters, and
 * digits.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/letter-case-permutation/
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
    vector<string> letterCasePermutation(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "a1b2"
    // Test case 2: "3z4"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
