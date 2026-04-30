// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 384: Shuffle an Array
 * Difficulty: Medium
 * Tags: Design, Array, Math, Randomized
 *
 * Problem Description:
 * Given an integer array `nums`, design an algorithm to randomly shuffle the
 * array. All permutations of the array should be **equally likely** as a
 * result of
 * the shuffling.
 * 
 * Implement the `Solution` class:
 * 
 * * `Solution(int[] nums)` Initializes the object with the integer array
 * `nums`.
 * * `int[] reset()` Resets the array to its original configuration and
 * returns it.
 *   * `int[] shuffle()` Returns a random shuffling of the array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["Solution", "shuffle", "reset", "shuffle"]
 *     [[[1, 2, 3]], [], [], []]
 *     **Output**
 *     [null, [3, 1, 2], [1, 2, 3], [1, 3, 2]]
 *     
 *     **Explanation**
 *     Solution solution = new Solution([1, 2, 3]);
 * solution.shuffle(); // Shuffle the array [1,2,3] and return its result.
 * // Any permutation of [1,2,3] must be equally likely to be returned.
 *                            // Example: return [3, 1, 2]
 * solution.reset(); // Resets the array back to its original configuration
 * [1,2,3]. Return [1, 2, 3]
 * solution.shuffle(); // Returns the random shuffling of array [1,2,3].
 * Example: return [1, 3, 2]
 *     
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 50`
 *   * `-106 <= nums[i] <= 106`
 *   * All the elements of `nums` are **unique**.
 *   * At most `104` calls **in total** will be made to `reset` and `shuffle`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/shuffle-an-array/
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
    Solution(vector<int>& nums) {
        
    }
    
    vector<int> reset() {
        
    }
    
    vector<int> shuffle() {
        
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(nums);
 * vector<int> param_1 = obj->reset();
 * vector<int> param_2 = obj->shuffle();
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: Solution
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["Solution","shuffle","reset","shuffle"]
    // [[[1,2,3]],[],[],[]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Solution obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
