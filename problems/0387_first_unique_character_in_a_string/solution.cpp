// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 387: First Unique Character in a String
 * Difficulty: Easy
 * Tags: Queue, Hash Table, String, Counting
 *
 * Problem Description:
 * Given a string `s`, find the **first** non-repeating character in it and
 * return
 * its index. If it **does not** exist, return `-1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** s = "leetcode"
 * 
 * **Output:** 0
 * 
 * **Explanation:**
 * 
 * The character `'l'` at index 0 is the first character that does not occur
 * at any
 * other index.
 * 
 * **Example 2:**
 * 
 * **Input:** s = "loveleetcode"
 * 
 * **Output:** 2
 * 
 * **Example 3:**
 * 
 * **Input:** s = "aabb"
 * 
 * **Output:** -1
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 105`
 *   * `s` consists of only lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/first-unique-character-in-a-string/
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
    int firstUniqChar(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "leetcode"
    // Test case 2: "loveleetcode"
    // Test case 3: "aabb"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
