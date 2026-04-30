// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 372: Super Pow
 * Difficulty: Medium
 * Tags: Math, Divide and Conquer
 *
 * Problem Description:
 * Your task is to calculate `ab` mod `1337` where `a` is a positive integer
 * and
 * `b` is an extremely large positive integer given in the form of an array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** a = 2, b = [3]
 *     **Output:** 8
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** a = 2, b = [1,0]
 *     **Output:** 1024
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** a = 1, b = [4,3,3,8,5,2]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= a <= 231 - 1`
 *   * `1 <= b.length <= 2000`
 *   * `0 <= b[i] <= 9`
 *   * `b` does not contain leading zeros.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/super-pow/
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
    int superPow(int a, vector<int>& b) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 2
    // Test case 2: [3]
    // Test case 3: 2
    // Test case 4: [1,0]
    // Test case 5: 1
    // Test case 6: [4,3,3,8,5,2]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
