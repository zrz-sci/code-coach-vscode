// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 251: Flatten 2D Vector
 * Difficulty: Medium
 * Tags: Design, Array, Two Pointers, Iterator
 *
 * Problem Description:
 * Design an iterator to flatten a 2D vector. It should support the `next` and
 * `hasNext` operations.
 * 
 * Implement the `Vector2D` class:
 * 
 *   * `Vector2D(int[][] vec)` initializes the object with the 2D vector `vec`.
 * * `next()` returns the next element from the 2D vector and moves the
 * pointer one step forward. You may assume that all the calls to `next` are
 * valid.
 * * `hasNext()` returns `true` if there are still some elements in the
 * vector, and `false` otherwise.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 * ["Vector2D", "next", "next", "next", "hasNext", "hasNext", "next",
 * "hasNext"]
 *     [[[[1, 2], [3], [4]]], [], [], [], [], [], [], []]
 *     **Output**
 *     [null, 1, 2, 3, true, true, 4, false]
 *     
 *     **Explanation**
 *     Vector2D vector2D = new Vector2D([[1, 2], [3], [4]]);
 *     vector2D.next();    // return 1
 *     vector2D.next();    // return 2
 *     vector2D.next();    // return 3
 *     vector2D.hasNext(); // return True
 *     vector2D.hasNext(); // return True
 *     vector2D.next();    // return 4
 *     vector2D.hasNext(); // return False
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= vec.length <= 200`
 *   * `0 <= vec[i].length <= 500`
 *   * `-500 <= vec[i][j] <= 500`
 *   * At most `105` calls will be made to `next` and `hasNext`.
 * 
 * 
 * 
 * **Follow up:** As an added challenge, try to code it using only [iterators
 * in
 * C++](http://www.cplusplus.com/reference/iterator/iterator/) or [iterators in
 * Java](http://docs.oracle.com/javase/7/docs/api/java/util/Iterator.html).
 * 
 * 
 *
 * Link: https://leetcode.com/problems/flatten-2d-vector/
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

class Vector2D {
public:
    Vector2D(vector<vector<int>>& vec) {
        
    }
    
    int next() {
        
    }
    
    bool hasNext() {
        
    }
};

/**
 * Your Vector2D object will be instantiated and called as such:
 * Vector2D* obj = new Vector2D(vec);
 * int param_1 = obj->next();
 * bool param_2 = obj->hasNext();
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: Vector2D
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["Vector2D","next","next","next","hasNext","hasNext","next","hasNext"]
    // [[[[1,2],[3],[4]]],[],[],[],[],[],[],[]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // Vector2D obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
