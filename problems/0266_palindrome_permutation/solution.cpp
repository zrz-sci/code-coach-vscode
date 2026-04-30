// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 266: Palindrome Permutation
 * Difficulty: Easy
 * Tags: Bit Manipulation, Hash Table, String
 *
 * Problem Description:
 * Given a string `s`, return `true` _if a permutation of the string could
 * form a_
 * _**palindrome**_ _and_`false` _otherwise_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "code"
 *     **Output:** false
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "aab"
 *     **Output:** true
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "carerac"
 *     **Output:** true
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 5000`
 *   * `s` consists of only lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/palindrome-permutation/
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
    bool canPermutePalindrome(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "code"
    // Test case 2: "aab"
    // Test case 3: "carerac"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
