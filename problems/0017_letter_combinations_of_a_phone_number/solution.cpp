// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 17: Letter Combinations of a Phone Number
 * Difficulty: Medium
 * Tags: Hash Table, String, Backtracking
 *
 * Problem Description:
 * Given a string containing digits from `2-9` inclusive, return all possible
 * letter combinations that the number could represent. Return the answer in
 * **any
 * order**.
 * 
 * A mapping of digits to letters (just like on the telephone buttons) is given
 * below. Note that 1 does not map to any letters.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** digits = "23"
 *     **Output:** ["ad","ae","af","bd","be","bf","cd","ce","cf"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** digits = "2"
 *     **Output:** ["a","b","c"]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= digits.length <= 4`
 *   * `digits[i]` is a digit in the range `['2', '9']`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/letter-combinations-of-a-phone-number/
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
    vector<string> letterCombinations(string digits) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "23"
    // Test case 2: "2"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
