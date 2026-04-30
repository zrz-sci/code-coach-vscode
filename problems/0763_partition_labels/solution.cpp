// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 763: Partition Labels
 * Difficulty: Medium
 * Tags: Greedy, Hash Table, Two Pointers, String
 *
 * Problem Description:
 * You are given a string `s`. We want to partition the string into as many
 * parts
 * as possible so that each letter appears in at most one part. For example,
 * the
 * string `"ababcc"` can be partitioned into `["abab", "cc"]`, but partitions
 * such
 * as `["aba", "bcc"]` or `["ab", "ab", "cc"]` are invalid.
 * 
 * Note that the partition is done so that after concatenating all the parts in
 * order, the resultant string should be `s`.
 * 
 * Return _a list of integers representing the size of these parts_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "ababcbacadefegdehijhklij"
 *     **Output:** [9,7,8]
 *     **Explanation:**
 *     The partition is "ababcbaca", "defegde", "hijhklij".
 *     This is a partition so that each letter appears in at most one part.
 * A partition like "ababcbacadefegde", "hijhklij" is incorrect, because it
 * splits s into less parts.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "eccbbbbdec"
 *     **Output:** [10]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 500`
 *   * `s` consists of lowercase English letters.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/partition-labels/
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
    vector<int> partitionLabels(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "ababcbacadefegdehijhklij"
    // Test case 2: "eccbbbbdec"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
