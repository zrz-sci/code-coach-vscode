// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 519: Random Flip Matrix
 * Difficulty: Medium
 * Tags: Reservoir Sampling, Hash Table, Math, Randomized
 *
 * Problem Description:
 * There is an `m x n` binary grid `matrix` with all the values set `0`
 * initially.
 * Design an algorithm to randomly pick an index `(i, j)` where `matrix[i][j]
 * == 0`
 * and flips it to `1`. All the indices `(i, j)` where `matrix[i][j] == 0`
 * should
 * be equally likely to be returned.
 * 
 * Optimize your algorithm to minimize the number of calls made to the
 * **built-in**
 * random function of your language and optimize the time and space complexity.
 * 
 * Implement the `Solution` class:
 * 
 * * `Solution(int m, int n)` Initializes the object with the size of the
 * binary matrix `m` and `n`.
 * * `int[] flip()` Returns a random index `[i, j]` of the matrix where
 * `matrix[i][j] == 0` and flips it to `1`.
 *   * `void reset()` Resets all the values of the matrix to be `0`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["Solution", "flip", "flip", "flip", "reset", "flip"]
 *     [[3, 1], [], [], [], [], []]
 *     **Output**
 *     [null, [1, 0], [2, 0], [0, 0], null, [2, 0]]
 *     
 *     **Explanation**
 *     Solution solution = new Solution(3, 1);
 * solution.flip(); // return [1, 0], [0,0], [1,0], and [2,0] should be
 * equally likely to be returned.
 * solution.flip(); // return [2, 0], Since [1,0] was returned, [2,0] and
 * [0,0]
 * solution.flip(); // return [0, 0], Based on the previously returned
 * indices, only [0,0] can be returned.
 *     solution.reset(); // All the values are reset to 0 and can be returned.
 * solution.flip(); // return [2, 0], [0,0], [1,0], and [2,0] should be
 * equally likely to be returned.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= m, n <= 104`
 *   * There will be at least one free cell for each call to `flip`.
 *   * At most `1000` calls will be made to `flip` and `reset`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/random-flip-matrix/
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
    Solution(int m, int n) {
        
    }
    
    vector<int> flip() {
        
    }
    
    void reset() {
        
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(m, n);
 * vector<int> param_1 = obj->flip();
 * obj->reset();
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: ["Solution","flip","flip","flip","reset","flip"]
    // Test case 2: [[3,1],[],[],[],[],[]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
