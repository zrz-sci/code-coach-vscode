// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 984: String Without AAA or BBB
 * Difficulty: Medium
 * Tags: Greedy, String
 *
 * Problem Description:
 * Given two integers `a` and `b`, return **any** string `s` such that:
 * 
 * * `s` has length `a + b` and contains exactly `a` `'a'` letters, and
 * exactly `b` `'b'` letters,
 *   * The substring `'aaa'` does not occur in `s`, and
 *   * The substring `'bbb'` does not occur in `s`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** a = 1, b = 2
 *     **Output:** "abb"
 *     **Explanation:** "abb", "bab" and "bba" are all correct answers.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** a = 4, b = 1
 *     **Output:** "aabaa"
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= a, b <= 100`
 *   * It is guaranteed such an `s` exists for the given `a` and `b`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/string-without-aaa-or-bbb/
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
    string strWithout3a3b(int a, int b) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 1
    // Test case 2: 2
    // Test case 3: 4
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
