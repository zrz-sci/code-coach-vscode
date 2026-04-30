// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1002: Find Common Characters
 * Difficulty: Easy
 * Tags: Array, Hash Table, String
 *
 * Problem Description:
 * Given a string array `words`, return _an array of all characters that show
 * up in
 * all strings within the_`words` _(including duplicates)_. You may return the
 * answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** words = ["bella","label","roller"]
 *     **Output:** ["e","l","l"]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** words = ["cool","lock","cook"]
 *     **Output:** ["c","o"]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= words.length <= 100`
 *   * `1 <= words[i].length <= 100`
 *   * `words[i]` consists of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-common-characters/
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
    vector<string> commonChars(vector<string>& words) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["bella","label","roller"]
    // Test case 2: ["cool","lock","cook"]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
