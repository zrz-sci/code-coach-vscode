// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 912: Sort an Array
 * Difficulty: Medium
 * Tags: Array, Divide and Conquer, Bucket Sort, Counting Sort, Radix Sort, Sorting, Heap (Priority Queue), Merge Sort
 *
 * Problem Description:
 * Given an array of integers `nums`, sort the array in ascending order and
 * return
 * it.
 * 
 * You must solve the problem **without using any built-in** functions in
 * `O(nlog(n))` time complexity and with the smallest space complexity
 * possible.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [5,2,3,1]
 *     **Output:** [1,2,3,5]
 * **Explanation:** After sorting the array, the positions of some numbers are
 * not changed (for example, 2 and 3), while the positions of other numbers are
 * changed (for example, 1 and 5).
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [5,1,1,2,0,0]
 *     **Output:** [0,0,1,1,2,5]
 * **Explanation:** Note that the values of nums are not necessarily unique.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 5 * 104`
 *   * `-5 * 104 <= nums[i] <= 5 * 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sort-an-array/
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
    vector<int> sortArray(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [5,2,3,1]
    // Test case 2: [5,1,1,2,0,0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
