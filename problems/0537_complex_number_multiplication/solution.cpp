// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 537: Complex Number Multiplication
 * Difficulty: Medium
 * Tags: Math, String, Simulation
 *
 * Problem Description:
 * A [complex number](https://en.wikipedia.org/wiki/Complex_number) can be
 * represented as a string on the form `"**real** +**imaginary** i"` where:
 * 
 *   * `real` is the real part and is an integer in the range `[-100, 100]`.
 * * `imaginary` is the imaginary part and is an integer in the range `[-100,
 * 100]`.
 *   * `i2 == -1`.
 * 
 * Given two complex numbers `num1` and `num2` as strings, return _a string of
 * the
 * complex number that represents their multiplications_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** num1 = "1+1i", num2 = "1+1i"
 *     **Output:** "0+2i"
 * **Explanation:** (1 + i) * (1 + i) = 1 + i2 + 2 * i = 2i, and you need
 * convert it to the form of 0+2i.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** num1 = "1+-1i", num2 = "1+-1i"
 *     **Output:** "0+-2i"
 * **Explanation:** (1 - i) * (1 - i) = 1 + i2 - 2 * i = -2i, and you need
 * convert it to the form of 0+-2i.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `num1` and `num2` are valid complex numbers.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/complex-number-multiplication/
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
    string complexNumberMultiply(string num1, string num2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "1+1i"
    // Test case 2: "1+1i"
    // Test case 3: "1+-1i"
    // Test case 4: "1+-1i"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
