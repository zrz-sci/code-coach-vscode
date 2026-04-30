// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 191: Number of 1 Bits
 * Difficulty: Easy
 * Tags: Bit Manipulation, Divide and Conquer
 *
 * Problem Description:
 * Given a positive integer `n`, write a function that returns the number of
 * set
 * bits in its binary representation (also known as the [Hamming
 * weight](http://en.wikipedia.org/wiki/Hamming_weight)).
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** n = 11
 * 
 * **Output:** 3
 * 
 * **Explanation:**
 * 
 * The input binary string **1011** has a total of three set bits.
 * 
 * **Example 2:**
 * 
 * **Input:** n = 128
 * 
 * **Output:** 1
 * 
 * **Explanation:**
 * 
 * The input binary string **10000000** has a total of one set bit.
 * 
 * **Example 3:**
 * 
 * **Input:** n = 2147483645
 * 
 * **Output:** 30
 * 
 * **Explanation:**
 * 
 * The input binary string **1111111111111111111111111111101** has a total of
 * thirty set bits.
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 231 - 1`
 * 
 * 
 * 
 * **Follow up:** If this function is called many times, how would you
 * optimize it?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/number-of-1-bits/
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
    int hammingWeight(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 11
    // Test case 2: 128
    // Test case 3: 2147483645

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
