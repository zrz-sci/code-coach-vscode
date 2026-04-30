// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 771: Jewels and Stones
 * Difficulty: Easy
 * Tags: Hash Table, String
 *
 * Problem Description:
 * You're given strings `jewels` representing the types of stones that are
 * jewels,
 * and `stones` representing the stones you have. Each character in `stones`
 * is a
 * type of stone you have. You want to know how many of the stones you have are
 * also jewels.
 * 
 * Letters are case sensitive, so `"a"` is considered a different type of stone
 * from `"A"`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** jewels = "aA", stones = "aAAbbbb"
 *     **Output:** 3
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** jewels = "z", stones = "ZZ"
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= jewels.length, stones.length <= 50`
 *   * `jewels` and `stones` consist of only English letters.
 *   * All the characters of `jewels` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/jewels-and-stones/
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
    int numJewelsInStones(string jewels, string stones) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aA"
    // Test case 2: "aAAbbbb"
    // Test case 3: "z"
    // Test case 4: "ZZ"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
