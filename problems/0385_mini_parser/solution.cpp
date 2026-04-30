// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 385: Mini Parser
 * Difficulty: Medium
 * Tags: Stack, Depth-First Search, String
 *
 * Problem Description:
 * Given a string s represents the serialization of a nested list, implement a
 * parser to deserialize it and return _the deserialized_ `NestedInteger`.
 * 
 * Each element is either an integer or a list whose elements may also be
 * integers
 * or other lists.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** s = "324"
 *     **Output:** 324
 * **Explanation:** You should return a NestedInteger object which contains a
 * single integer 324.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** s = "[123,[456,[789]]]"
 *     **Output:** [123,[456,[789]]]
 * **Explanation:** Return a NestedInteger object containing a nested list
 * with 2 elements:
 *     1. An integer containing value 123.
 *     2. A nested list containing two elements:
 *         i.  An integer containing value 456.
 *         ii. A nested list with one element:
 *              a. An integer containing value 789
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= s.length <= 5 * 104`
 * * `s` consists of digits, square brackets `"[]"`, negative sign `'-'`, and
 * commas `','`.
 *   * `s` is the serialization of valid `NestedInteger`.
 *   * All the values in the input are in the range `[-106, 106]`.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/mini-parser/
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

/**
 * // This is the interface that allows for creating nested lists.
 * // You should not implement it, or speculate about its implementation
 * class NestedInteger {
 *   public:
 *     // Constructor initializes an empty nested list.
 *     NestedInteger();
 *
 *     // Constructor initializes a single integer.
 *     NestedInteger(int value);
 *
 *     // Return true if this NestedInteger holds a single integer, rather than a nested list.
 *     bool isInteger() const;
 *
 *     // Return the single integer that this NestedInteger holds, if it holds a single integer
 *     // The result is undefined if this NestedInteger holds a nested list
 *     int getInteger() const;
 *
 *     // Set this NestedInteger to hold a single integer.
 *     void setInteger(int value);
 *
 *     // Set this NestedInteger to hold a nested list and adds a nested integer to it.
 *     void add(const NestedInteger &ni);
 *
 *     // Return the nested list that this NestedInteger holds, if it holds a nested list
 *     // The result is undefined if this NestedInteger holds a single integer
 *     const vector<NestedInteger> &getList() const;
 * };
 */
class Solution {
public:
    NestedInteger deserialize(string s) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: "324"
    // Test case 2: "[123,[456,[789]]]"

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
