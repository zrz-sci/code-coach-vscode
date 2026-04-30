// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1502: Can Make Arithmetic Progression From Sequence
 * Difficulty: Easy
 * Tags: Array, Sorting
 *
 * Problem Description:
 * A sequence of numbers is called an **arithmetic progression** if the
 * difference
 * between any two consecutive elements is the same.
 * 
 * Given an array of numbers `arr`, return `true` _if the array can be
 * rearranged
 * to form an**arithmetic progression**. Otherwise, return_ `false`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** arr = [3,5,1]
 *     **Output:** true
 * **Explanation:** We can reorder the elements as [1,3,5] or [5,3,1] with
 * differences 2 and -2 respectively, between each consecutive elements.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** arr = [1,2,4]
 *     **Output:** false
 * **Explanation:** There is no way to reorder the elements to obtain an
 * arithmetic progression.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= arr.length <= 1000`
 *   * `-106 <= arr[i] <= 106`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/can-make-arithmetic-progression-from-sequence/
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
    bool canMakeArithmeticProgression(vector<int>& arr) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [3,5,1]
    // Test case 2: [1,2,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
