// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 520: Detect Capital
 * Difficulty: Easy
 * Tags: String
 *
 * Problem Description:
 * We define the usage of capitals in a word to be right when one of the
 * following
 * cases holds:
 * 
 *   * All letters in this word are capitals, like `"USA"`.
 *   * All letters in this word are not capitals, like `"leetcode"`.
 *   * Only the first letter in this word is capital, like `"Google"`.
 * 
 * Given a string `word`, return `true` if the usage of capitals in it is
 * right.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** word = "USA"
 *     **Output:** true
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** word = "FlaG"
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= word.length <= 100`
 *   * `word` consists of lowercase and uppercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/detect-capital/
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
    bool detectCapitalUse(string word) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "USA"
    // Test case 2: "FlaG"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
