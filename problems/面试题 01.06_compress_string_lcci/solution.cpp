// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 面试题 01.06: Compress String LCCI
 * Difficulty: Easy
 * Tags: Two Pointers, String
 *
 * Problem Description:
 * Implement a method to perform basic string compression using the counts of
 * repeated characters. For example, the string aabcccccaaa would become
 * a2blc5a3.
 * If the "compressed" string would not become smaller than the original
 * string,
 * your method should return the original string. You can assume the string has
 * only uppercase and lowercase letters (a - z).
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** "aabcccccaaa"
 *     **Output:** "a2b1c5a3"
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** "abbccd"
 *     **Output:** "abbccd"
 *     **Explanation:**
 * The compressed string is "a1b2c2d1", which is longer than the original
 * string.
 *     
 * 
 * 
 * 
 * **Note:**
 * 
 *   1. `0 <= S.length <= 50000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/compress-string-lcci/
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
    string compressString(string S) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aabcccccaa"
    // Test case 2: "abbccd"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
