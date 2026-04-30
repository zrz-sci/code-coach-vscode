// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 886: Possible Bipartition
 * Difficulty: Medium
 * Tags: Depth-First Search, Breadth-First Search, Union Find, Graph
 *
 * Problem Description:
 * We want to split a group of `n` people (labeled from `1` to `n`) into two
 * groups
 * of **any size**. Each person may dislike some other people, and they should
 * not
 * go into the same group.
 * 
 * Given the integer `n` and the array `dislikes` where `dislikes[i] = [ai,
 * bi]`
 * indicates that the person labeled `ai` does not like the person labeled
 * `bi`,
 * return `true` _if it is possible to split everyone into two groups in this
 * way_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 4, dislikes = [[1,2],[1,3],[2,4]]
 *     **Output:** true
 * **Explanation:** The first group has [1,4], and the second group has [2,3].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 3, dislikes = [[1,2],[1,3],[2,3]]
 *     **Output:** false
 * **Explanation:** We need at least 3 groups to divide them. We cannot put
 * them in two groups.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 2000`
 *   * `0 <= dislikes.length <= 104`
 *   * `dislikes[i].length == 2`
 *   * `1 <= ai < bi <= n`
 *   * All the pairs of `dislikes` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/possible-bipartition/
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
    bool possibleBipartition(int n, vector<vector<int>>& dislikes) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 4
    // Test case 2: [[1,2],[1,3],[2,4]]
    // Test case 3: 3
    // Test case 4: [[1,2],[1,3],[2,3]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
