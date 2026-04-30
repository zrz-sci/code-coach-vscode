// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 31: Next Permutation
 * Difficulty: Medium
 * Tags: Array, Two Pointers
 *
 * Problem Description:
 * A **permutation** of an array of integers is an arrangement of its members
 * into
 * a sequence or linear order.
 * 
 * * For example, for `arr = [1,2,3]`, the following are all the permutations
 * of `arr`: `[1,2,3], [1,3,2], [2, 1, 3], [2, 3, 1], [3,1,2], [3,2,1]`.
 * 
 * The **next permutation** of an array of integers is the next
 * lexicographically
 * greater permutation of its integer. More formally, if all the permutations
 * of
 * the array are sorted in one container according to their lexicographical
 * order,
 * then the **next permutation** of that array is the permutation that follows
 * it
 * in the sorted container. If such arrangement is not possible, the array
 * must be
 * rearranged as the lowest possible order (i.e., sorted in ascending order).
 * 
 *   * For example, the next permutation of `arr = [1,2,3]` is `[1,3,2]`.
 *   * Similarly, the next permutation of `arr = [2,3,1]` is `[3,1,2]`.
 * * While the next permutation of `arr = [3,2,1]` is `[1,2,3]` because
 * `[3,2,1]` does not have a lexicographical larger rearrangement.
 * 
 * Given an array of integers `nums`, _find the next permutation of_ `nums`.
 * 
 * The replacement must be **[in place](http://en.wikipedia.org/wiki/In-
 * place_algorithm)** and use only constant extra memory.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3]
 *     **Output:** [1,3,2]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [3,2,1]
 *     **Output:** [1,2,3]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,5]
 *     **Output:** [1,5,1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 100`
 *   * `0 <= nums[i] <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/next-permutation/
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
    void nextPermutation(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3]
    // Test case 2: [3,2,1]
    // Test case 3: [1,1,5]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
