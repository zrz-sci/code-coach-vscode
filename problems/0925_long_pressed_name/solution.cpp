// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 925: Long Pressed Name
 * Difficulty: Easy
 * Tags: Two Pointers, String
 *
 * Problem Description:
 * Your friend is typing his `name` into a keyboard. Sometimes, when typing a
 * character `c`, the key might get _long pressed_ , and the character will be
 * typed 1 or more times.
 * 
 * You examine the `typed` characters of the keyboard. Return `True` if it is
 * possible that it was your friends name, with some characters (possibly none)
 * being long pressed.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** name = "alex", typed = "aaleex"
 *     **Output:** true
 *     **Explanation:** 'a' and 'e' in 'alex' were long pressed.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** name = "saeed", typed = "ssaaedd"
 *     **Output:** false
 * **Explanation:** 'e' must have been pressed twice, but it was not in the
 * typed output.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= name.length, typed.length <= 1000`
 *   * `name` and `typed` consist of only lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/long-pressed-name/
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
    bool isLongPressedName(string name, string typed) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "alex"
    // Test case 2: "aaleex"
    // Test case 3: "saeed"
    // Test case 4: "ssaaedd"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
