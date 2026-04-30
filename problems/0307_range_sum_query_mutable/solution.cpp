// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 307: Range Sum Query - Mutable
 * Difficulty: Medium
 * Tags: Design, Binary Indexed Tree, Segment Tree, Array, Divide and Conquer
 *
 * Problem Description:
 * Given an integer array `nums`, handle multiple queries of the following
 * types:
 * 
 *   1. **Update** the value of an element in `nums`.
 * 2. Calculate the **sum** of the elements of `nums` between indices `left`
 * and `right` **inclusive** where `left <= right`.
 * 
 * Implement the `NumArray` class:
 * 
 * * `NumArray(int[] nums)` Initializes the object with the integer array
 * `nums`.
 * * `void update(int index, int val)` **Updates** the value of `nums[index]`
 * to be `val`.
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
 *     ["NumArray", "sumRange", "update", "sumRange"]
 *     [[[1, 3, 5]], [0, 2], [1, 2], [0, 2]]
 *     **Output**
 *     [null, 9, null, 8]
 *     
 *     **Explanation**
 *     NumArray numArray = new NumArray([1, 3, 5]);
 *     numArray.sumRange(0, 2); // return 1 + 3 + 5 = 9
 *     numArray.update(1, 2);   // nums = [1, 2, 5]
 *     numArray.sumRange(0, 2); // return 1 + 2 + 5 = 8
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 3 * 104`
 *   * `-100 <= nums[i] <= 100`
 *   * `0 <= index < nums.length`
 *   * `-100 <= val <= 100`
 *   * `0 <= left <= right < nums.length`
 *   * At most `3 * 104` calls will be made to `update` and `sumRange`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/range-sum-query-mutable/
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
    
    void update(int index, int val) {
        
    }
    
    int sumRange(int left, int right) {
        
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * obj->update(index,val);
 * int param_2 = obj->sumRange(left,right);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: NumArray
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["NumArray","sumRange","update","sumRange"]
    // [[[1,3,5]],[0,2],[1,2],[0,2]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // NumArray obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
