// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1095: Find in Mountain Array
 * Difficulty: Hard
 * Tags: Array, Binary Search, Interactive
 *
 * Problem Description:
 * _(This problem is an**interactive problem**.)_
 * 
 * You may recall that an array `arr` is a **mountain array** if and only if:
 * 
 *   * `arr.length >= 3`
 *   * There exists some `i` with `0 < i < arr.length - 1` such that: 
 *     * `arr[0] < arr[1] < ... < arr[i - 1] < arr[i]`
 *     * `arr[i] > arr[i + 1] > ... > arr[arr.length - 1]`
 * 
 * Given a mountain array `mountainArr`, return the **minimum** `index` such
 * that
 * `mountainArr.get(index) == target`. If such an `index` does not exist,
 * return
 * `-1`.
 * 
 * **You cannot access the mountain array directly.** You may only access the
 * array
 * using a `MountainArray` interface:
 * 
 * * `MountainArray.get(k)` returns the element of the array at index `k`
 * (0-indexed).
 *   * `MountainArray.length()` returns the length of the array.
 * 
 * Submissions making more than `100` calls to `MountainArray.get` will be
 * judged
 * _Wrong Answer_. Also, any solutions that attempt to circumvent the judge
 * will
 * result in disqualification.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** mountainArr = [1,2,3,4,5,3,1], target = 3
 *     **Output:** 2
 * **Explanation:** 3 exists in the array, at index=2 and index=5. Return the
 * minimum index, which is 2.
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** mountainArr = [0,1,2,4,2,1], target = 3
 *     **Output:** -1
 *     **Explanation:** 3 does not exist in the array, so we return -1.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `3 <= mountainArr.length() <= 104`
 *   * `0 <= target <= 109`
 *   * `0 <= mountainArr.get(index) <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-in-mountain-array/
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
 * // This is the MountainArray's API interface.
 * // You should not implement it, or speculate about its implementation
 * class MountainArray {
 *   public:
 *     int get(int index);
 *     int length();
 * };
 */

class Solution {
public:
    int findInMountainArray(int target, MountainArray &mountainArr) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4,5,3,1]
    // Test case 2: 3
    // Test case 3: [0,1,2,4,2,1]
    // Test case 4: 3

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
