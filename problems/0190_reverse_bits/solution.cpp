// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 190: Reverse Bits
 * Difficulty: Easy
 * Tags: Bit Manipulation, Divide and Conquer
 *
 * Problem Description:
 * Reverse bits of a given 32 bits signed integer.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** n = 43261596
 * 
 * **Output:** 964176192
 * 
 * **Explanation:**
 * 
 * Integer | Binary  
 * ---|---  
 * 43261596 | 00000010100101000001111010011100  
 * 964176192 | 00111001011110000010100101000000  
 *   
 * **Example 2:**
 * 
 * **Input:** n = 2147483644
 * 
 * **Output:** 1073741822
 * 
 * **Explanation:**
 * 
 * Integer | Binary  
 * ---|---  
 * 2147483644 | 01111111111111111111111111111100  
 * 1073741822 | 00111111111111111111111111111110  
 *   
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= n <= 231 - 2`
 *   * `n` is even.
 * 
 * 
 * 
 * **Follow up:** If this function is called many times, how would you
 * optimize it?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/reverse-bits/
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
    int reverseBits(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 43261596
    // Test case 2: 2147483644

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
