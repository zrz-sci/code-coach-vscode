// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 65: Valid Number
 * Difficulty: Hard
 * Tags: String
 *
 * Problem Description:
 * Given a string `s`, return whether `s` is a **valid number**.  
 *   
 * For example, all the following are valid numbers: `"2", "0089", "-0.1",
 * "+3.14",
 * "4.", "-.9", "2e10", "-90E3", "3e+7", "+6e-1", "53.5e93", "-123.456e789"`,
 * while
 * the following are not valid numbers: `"abc", "1a", "1e", "e3", "99e2.5",
 * "--6",
 * "-+3", "95a54e53"`.
 * 
 * Formally, a **valid number** is defined using one of the following
 * definitions:
 * 
 *   1. An **integer number** followed by an **optional exponent**.
 *   2. A **decimal number** followed by an **optional exponent**.
 * 
 * An **integer number** is defined with an **optional sign** `'-'` or `'+'`
 * followed by **digits**.
 * 
 * A **decimal number** is defined with an **optional sign** `'-'` or `'+'`
 * followed by one of the following definitions:
 * 
 *   1. **Digits** followed by a **dot** `'.'`.
 *   2. **Digits** followed by a **dot** `'.'` followed by **digits**.
 *   3. A **dot** `'.'` followed by **digits**.
 * 
 * An **exponent** is defined with an **exponent notation** `'e'` or `'E'`
 * followed
 * by an **integer number**.
 * 
 * The **digits** are defined as one or more digits.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** s = "0"
 * 
 * **Output:** true
 * 
 * **Example 2:**
 * 
 * **Input:** s = "e"
 * 
 * **Output:** false
 * 
 * **Example 3:**
 * 
 * **Input:** s = "."
 * 
 * **Output:** false
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 20`
 * * `s` consists of only English letters (both uppercase and lowercase),
 * digits (`0-9`), plus `'+'`, minus `'-'`, or dot `'.'`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/valid-number/
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
    bool isNumber(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "0"
    // Test case 2: "e"
    // Test case 3: "."

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
