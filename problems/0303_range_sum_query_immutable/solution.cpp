// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 303: Range Sum Query - Immutable
 * Difficulty: Easy
 * Tags: Design, Array, Prefix Sum
 *
 * Problem Description:
 * Given an integer array `nums`, handle multiple queries of the following
 * type:
 * 
 * 1. Calculate the **sum** of the elements of `nums` between indices `left`
 * and `right` **inclusive** where `left <= right`.
 * 
 * Implement the `NumArray` class:
 * 
 * * `NumArray(int[] nums)` Initializes the object with the integer array
 * `nums`.
 * * `int sumRange(int left, int right)` Returns the **sum** of the elements
 * of `nums` between indices `left` and `right` **inclusive** (i.e. `nums[left]
 * + nums[left + 1] + ... + nums[right]`).
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["NumArray", "sumRange", "sumRange", "sumRange"]
 *     [[[-2, 0, 3, -5, 2, -1]], [0, 2], [2, 5], [0, 5]]
 *     **Output**
 *     [null, 1, -1, -3]
 *     
 *     **Explanation**
 *     NumArray numArray = new NumArray([-2, 0, 3, -5, 2, -1]);
 *     numArray.sumRange(0, 2); // return (-2) + 0 + 3 = 1
 *     numArray.sumRange(2, 5); // return 3 + (-5) + 2 + (-1) = -1
 *     numArray.sumRange(0, 5); // return (-2) + 0 + 3 + (-5) + 2 + (-1) = -3
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `-105 <= nums[i] <= 105`
 *   * `0 <= left <= right < nums.length`
 *   * At most `104` calls will be made to `sumRange`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/range-sum-query-immutable/
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

class NumArray {
public:
    NumArray(vector<int>& nums) {
        
    }
    
    int sumRange(int left, int right) {
        
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * int param_1 = obj->sumRange(left,right);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: NumArray
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["NumArray","sumRange","sumRange","sumRange"]
    // [[[-2,0,3,-5,2,-1]],[0,2],[2,5],[0,5]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // NumArray obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
