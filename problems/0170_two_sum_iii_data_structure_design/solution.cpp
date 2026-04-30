// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 170: Two Sum III - Data structure design
 * Difficulty: Easy
 * Tags: Design, Array, Hash Table, Two Pointers, Data Stream
 *
 * Problem Description:
 * Design a data structure that accepts a stream of integers and checks if it
 * has a
 * pair of integers that sum up to a particular value.
 * 
 * Implement the `TwoSum` class:
 * 
 * * `TwoSum()` Initializes the `TwoSum` object, with an empty array
 * initially.
 *   * `void add(int number)` Adds `number` to the data structure.
 * * `boolean find(int value)` Returns `true` if there exists any pair of
 * numbers whose sum is equal to `value`, otherwise, it returns `false`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input**
 *     ["TwoSum", "add", "add", "add", "find", "find"]
 *     [[], [1], [3], [5], [4], [7]]
 *     **Output**
 *     [null, null, null, null, true, false]
 *     
 *     **Explanation**
 *     TwoSum twoSum = new TwoSum();
 *     twoSum.add(1);   // [] --> [1]
 *     twoSum.add(3);   // [1] --> [1,3]
 *     twoSum.add(5);   // [1,3] --> [1,3,5]
 *     twoSum.find(4);  // 1 + 3 = 4, return true
 *     twoSum.find(7);  // No two integers sum up to 7, return false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `-105 <= number <= 105`
 *   * `-231 <= value <= 231 - 1`
 *   * At most `104` calls will be made to `add` and `find`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/two-sum-iii-data-structure-design/
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

class TwoSum {
public:
    TwoSum() {
        
    }
    
    void add(int number) {
        
    }
    
    bool find(int value) {
        
    }
};

/**
 * Your TwoSum object will be instantiated and called as such:
 * TwoSum* obj = new TwoSum();
 * obj->add(number);
 * bool param_2 = obj->find(value);
 */

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    // This is a design problem using class: TwoSum
    // The test cases follow format: ["ClassName", "method1", "method2", ...]
    //                              [[], [arg1], [arg2], ...]

    // Example test cases:
    // ["TwoSum","add","add","add","find","find"]
    // [[],[1],[3],[5],[4],[7]]

    // TODO: Implement your tests here based on the test cases above
    // Example:
    // TwoSum obj;
    // obj.method(args);

    cout << "Tests completed!" << endl;
    return 0;
}
