// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1570: Dot Product of Two Sparse Vectors
 * Difficulty: Medium
 * Tags: Design, Array, Hash Table, Two Pointers
 *
 * Problem Description:
 * Given two sparse vectors, compute their dot product.
 * 
 * Implement class `SparseVector`:
 * 
 *   * `SparseVector(nums)` Initializes the object with the vector `nums`
 * * `dotProduct(vec)` Compute the dot product between the instance of
 * _SparseVector_ and `vec`
 * 
 * A **sparse vector** is a vector that has mostly zero values, you should
 * store
 * the sparse vector **efficiently** and compute the dot product between two
 * _SparseVector_.
 * 
 * **Follow up:  **What if only one of the vectors is sparse?
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums1 = [1,0,0,2,3], nums2 = [0,3,0,4,0]
 *     **Output:** 8
 *     **Explanation:** v1 = SparseVector(nums1) , v2 = SparseVector(nums2)
 *     v1.dotProduct(v2) = 1*0 + 0*3 + 0*0 + 2*4 + 3*0 = 8
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums1 = [0,1,0,0,0], nums2 = [0,0,0,0,2]
 *     **Output:** 0
 *     **Explanation:** v1 = SparseVector(nums1) , v2 = SparseVector(nums2)
 *     v1.dotProduct(v2) = 0*0 + 1*0 + 0*0 + 0*0 + 0*2 = 0
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums1 = [0,1,0,0,2,0,0], nums2 = [1,0,0,0,3,0,4]
 *     **Output:** 6
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums1.length == nums2.length`
 *   * `1 <= n <= 10^5`
 *   * `0 <= nums1[i], nums2[i] <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/dot-product-of-two-sparse-vectors/
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

class SparseVector {
public:
    
    SparseVector(vector<int> &nums) {
        
    }
    
    // Return the dotProduct of two sparse vectors
    int dotProduct(SparseVector& vec) {
        
    }
};

// Your SparseVector object will be instantiated and called as such:
// SparseVector v1(nums1);
// SparseVector v2(nums2);
// int ans = v1.dotProduct(v2);

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: SparseVector
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // [1,0,0,2,3]
    // [0,3,0,4,0]
    // [0,1,0,0,0]
    // [0,0,0,0,2]
    // [0,1,0,0,2,0,0]
    // [1,0,0,0,3,0,4]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // SparseVector obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
