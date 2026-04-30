// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 434: Number of Segments in a String
 * Difficulty: Easy
 * Tags: String
 *
 * Problem Description:
 * Given a string `s`, return _the number of segments in the string_.
 * 
 * A **segment** is defined to be a contiguous sequence of **non-space
 * characters**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "Hello, my name is John"
 *     **Output:** 5
 * **Explanation:** The five segments are ["Hello,", "my", "name", "is",
 * "John"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "Hello"
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= s.length <= 300`
 * * `s` consists of lowercase and uppercase English letters, digits, or one
 * of the following characters `"!@#$%^&*()_+-=',.:"`.
 *   * The only space character in `s` is `' '`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/number-of-segments-in-a-string/
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
    int countSegments(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "Hello, my name is John"
    // Test case 2: "Hello"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
