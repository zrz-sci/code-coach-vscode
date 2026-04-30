// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 面试题 08.06: Hanota LCCI
 * Difficulty: Easy
 * Tags: Recursion, Array
 *
 * Problem Description:
 * In the classic problem of the Towers of Hanoi, you have 3 towers and N
 * disks of
 * different sizes which can slide onto any tower. The puzzle starts with disks
 * sorted in ascending order of size from top to bottom (i.e., each disk sits
 * on
 * top of an even larger one). You have the following constraints:
 * 
 * (1) Only one disk can be moved at a time.  
 * (2) A disk is slid off the top of one tower onto another tower.  
 * (3) A disk cannot be placed on top of a smaller disk.
 * 
 * Write a program to move the disks from the first tower to the last using
 * stacks.
 * 
 * **Example1:**
 * 
 *     
 *     
 *     **Input** : A = [2, 1, 0], B = [], C = []
 *     **Output** : C = [2, 1, 0]
 *     
 * 
 * **Example2:**
 * 
 *     
 *     
 *     **Input** : A = [1, 0], B = [], C = []
 *     **Output** : C = [1, 0]
 *     
 * 
 * **Note:**
 * 
 *   1. `A.length <= 14`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/hanota-lcci/
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
    void hanota(vector<int>& A, vector<int>& B, vector<int>& C) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [0]
    // Test case 2: []
    // Test case 3: []
    // Test case 4: [1, 0]
    // Test case 5: []
    // Test case 6: []

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
