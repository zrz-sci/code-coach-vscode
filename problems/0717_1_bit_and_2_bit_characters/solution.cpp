// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 717: 1-bit and 2-bit Characters
 * Difficulty: Easy
 * Tags: Array
 *
 * Problem Description:
 * We have two special characters:
 * 
 *   * The first character can be represented by one bit `0`.
 *   * The second character can be represented by two bits (`10` or `11`).
 * 
 * Given a binary array `bits` that ends with `0`, return `true` if the last
 * character must be a one-bit character.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** bits = [1,0,0]
 *     **Output:** true
 * **Explanation:** The only way to decode it is two-bit character and one-bit
 * character.
 *     So the last character is one-bit character.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** bits = [1,1,1,0]
 *     **Output:** false
 * **Explanation:** The only way to decode it is two-bit character and two-bit
 * character.
 *     So the last character is not one-bit character.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= bits.length <= 1000`
 *   * `bits[i]` is either `0` or `1`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/1-bit-and-2-bit-characters/
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
    bool isOneBitCharacter(vector<int>& bits) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,0,0]
    // Test case 2: [1,1,1,0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
