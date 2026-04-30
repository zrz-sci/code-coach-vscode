// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 471: Encode String with Shortest Length
 * Difficulty: Hard
 * Tags: String, Dynamic Programming
 *
 * Problem Description:
 * Given a string `s`, encode the string such that its encoded length is the
 * shortest.
 * 
 * The encoding rule is: `k[encoded_string]`, where the `encoded_string`
 * inside the
 * square brackets is being repeated exactly `k` times. `k` should be a
 * positive
 * integer.
 * 
 * If an encoding process does not make the string shorter, then do not encode
 * it.
 * If there are several solutions, return **any of them**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "aaa"
 *     **Output:** "aaa"
 * **Explanation:** There is no way to encode it such that it is shorter than
 * the input string, so we do not encode it.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "aaaaa"
 *     **Output:** "5[a]"
 *     **Explanation:** "5[a]" is shorter than "aaaaa" by 1 character.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "aaaaaaaaaa"
 *     **Output:** "10[a]"
 * **Explanation:** "a9[a]" or "9[a]a" are also valid solutions, both of them
 * have the same length = 5, which is the same as "10[a]".
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 150`
 *   * `s` consists of only lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/encode-string-with-shortest-length/
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
    string encode(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aaa"
    // Test case 2: "aaaaa"
    // Test case 3: "aaaaaaaaaa"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
