// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 239: Sliding Window Maximum
 * Difficulty: Hard
 * Tags: Queue, Array, Sliding Window, Monotonic Queue, Heap (Priority Queue)
 *
 * Problem Description:
 * You are given an array of integers `nums`, there is a sliding window of
 * size `k`
 * which is moving from the very left of the array to the very right. You can
 * only
 * see the `k` numbers in the window. Each time the sliding window moves right
 * by
 * one position.
 * 
 * Return _the max sliding window_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,3,-1,-3,5,3,6,7], k = 3
 *     **Output:** [3,3,5,5,6,7]
 *     **Explanation:** 
 *     Window position                Max
 *     ---------------               -----
 *     [1  3  -1] -3  5  3  6  7       **3**
 *      1 [3  -1  -3] 5  3  6  7       **3**
 *      1  3 [-1  -3  5] 3  6  7      **5**
 *      1  3  -1 [-3  5  3] 6  7       **5**
 *      1  3  -1  -3 [5  3  6] 7       **6**
 *      1  3  -1  -3  5 [3  6  7]      **7**
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1], k = 1
 *     **Output:** [1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `-104 <= nums[i] <= 104`
 *   * `1 <= k <= nums.length`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/sliding-window-maximum/
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
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,3,-1,-3,5,3,6,7]
    // Test case 2: 3
    // Test case 3: [1]
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
