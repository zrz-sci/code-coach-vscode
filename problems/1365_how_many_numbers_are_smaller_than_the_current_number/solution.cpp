// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1365: How Many Numbers Are Smaller Than the Current Number
 * Difficulty: Easy
 * Tags: Array, Hash Table, Counting Sort, Sorting
 *
 * Problem Description:
 * Given the array `nums`, for each `nums[i]` find out how many numbers in the
 * array are smaller than it. That is, for each `nums[i]` you have to count the
 * number of valid `j's` such that `j != i` **and** `nums[j] < nums[i]`.
 * 
 * Return the answer in an array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [8,1,2,2,3]
 *     **Output:** [4,0,1,1,3]
 *     **Explanation:** 
 *     For nums[0]=8 there exist four smaller numbers than it (1, 2, 2 and 3). 
 *     For nums[1]=1 does not exist any smaller number than it.
 *     For nums[2]=2 there exist one smaller number than it (1). 
 *     For nums[3]=2 there exist one smaller number than it (1). 
 *     For nums[4]=3 there exist three smaller numbers than it (1, 2 and 2).
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [6,5,4,8]
 *     **Output:** [2,1,0,3]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [7,7,7,7]
 *     **Output:** [0,0,0,0]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= nums.length <= 500`
 *   * `0 <= nums[i] <= 100`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/how-many-numbers-are-smaller-than-the-current-number/
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
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [8,1,2,2,3]
    // Test case 2: [6,5,4,8]
    // Test case 3: [7,7,7,7]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
