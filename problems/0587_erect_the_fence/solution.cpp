// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 587: Erect the Fence
 * Difficulty: Hard
 * Tags: Geometry, Array, Math
 *
 * Problem Description:
 * You are given an array `trees` where `trees[i] = [xi, yi]` represents the
 * location of a tree in the garden.
 * 
 * Fence the entire garden using the minimum length of rope, as it is
 * expensive.
 * The garden is well-fenced only if **all the trees are enclosed**.
 * 
 * Return _the coordinates of trees that are exactly located on the fence
 * perimeter_. You may return the answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** trees = [[1,1],[2,2],[2,0],[2,4],[3,3],[4,2]]
 *     **Output:** [[1,1],[2,0],[4,2],[3,3],[2,4]]
 * **Explanation:** All the trees will be on the perimeter of the fence except
 * the tree at [2, 2], which will be inside the fence.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** trees = [[1,2],[2,2],[4,2]]
 *     **Output:** [[4,2],[2,2],[1,2]]
 * **Explanation:** The fence forms a line that passes through all the trees.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= trees.length <= 3000`
 *   * `trees[i].length == 2`
 *   * `0 <= xi, yi <= 100`
 *   * All the given positions are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/erect-the-fence/
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
    vector<vector<int>> outerTrees(vector<vector<int>>& trees) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,1],[2,2],[2,0],[2,4],[3,3],[4,2]]
    // Test case 2: [[1,2],[2,2],[4,2]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
