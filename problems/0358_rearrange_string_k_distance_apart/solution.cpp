// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 358: Rearrange String k Distance Apart
 * Difficulty: Hard
 * Tags: Greedy, Hash Table, String, Counting, Sorting, Heap (Priority Queue)
 *
 * Problem Description:
 * Given a string `s` and an integer `k`, rearrange `s` such that the same
 * characters are **at least** distance `k` from each other. If it is not
 * possible
 * to rearrange the string, return an empty string `""`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "aabbcc", k = 3
 *     **Output:** "abcabc"
 * **Explanation:** The same letters are at least a distance of 3 from each
 * other.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "aaabc", k = 3
 *     **Output:** ""
 *     **Explanation:** It is not possible to rearrange the string.
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** s = "aaadbbcc", k = 2
 *     **Output:** "abacabcd"
 * **Explanation:** The same letters are at least a distance of 2 from each
 * other.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 3 * 105`
 *   * `s` consists of only lowercase English letters.
 *   * `0 <= k <= s.length`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/rearrange-string-k-distance-apart/
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
    string rearrangeString(string s, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "aabbcc"
    // Test case 2: 3
    // Test case 3: "aaabc"
    // Test case 4: 3
    // Test case 5: "aaadbbcc"
    // Test case 6: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
