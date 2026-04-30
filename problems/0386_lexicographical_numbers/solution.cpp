// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 386: Lexicographical Numbers
 * Difficulty: Medium
 * Tags: Depth-First Search, Trie
 *
 * Problem Description:
 * Given an integer `n`, return all the numbers in the range `[1, n]` sorted in
 * lexicographical order.
 * 
 * You must write an algorithm that runs in `O(n)` time and uses `O(1)` extra
 * space.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 13
 *     **Output:** [1,10,11,12,13,2,3,4,5,6,7,8,9]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 2
 *     **Output:** [1,2]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 5 * 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/lexicographical-numbers/
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
    vector<int> lexicalOrder(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 13
    // Test case 2: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
