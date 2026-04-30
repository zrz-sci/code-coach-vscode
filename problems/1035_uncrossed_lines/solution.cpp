// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 1035: Uncrossed Lines
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming
 *
 * Problem Description:
 * You are given two integer arrays `nums1` and `nums2`. We write the integers
 * of
 * `nums1` and `nums2` (in the order they are given) on two separate horizontal
 * lines.
 * 
 * We may draw connecting lines: a straight line connecting two numbers
 * `nums1[i]`
 * and `nums2[j]` such that:
 * 
 *   * `nums1[i] == nums2[j]`, and
 * * the line we draw does not intersect any other connecting (non-horizontal)
 * line.
 * 
 * Note that a connecting line cannot intersect even at the endpoints (i.e.,
 * each
 * number can only belong to one connecting line).
 * 
 * Return _the maximum number of connecting lines we can draw in this way_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums1 = [1,4,2], nums2 = [1,2,4]
 *     **Output:** 2
 *     **Explanation:** We can draw 2 uncrossed lines as in the diagram.
 * We cannot draw 3 uncrossed lines, because the line from nums1[1] = 4 to
 * nums2[2] = 4 will intersect the line from nums1[2]=2 to nums2[1]=2.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums1 = [2,5,1,2,5], nums2 = [10,5,2,1,5,2]
 *     **Output:** 3
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums1 = [1,3,7,1,7,5], nums2 = [1,9,2,5,1]
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums1.length, nums2.length <= 500`
 *   * `1 <= nums1[i], nums2[j] <= 2000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/uncrossed-lines/
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
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,4,2]
    // Test case 2: [1,2,4]
    // Test case 3: [2,5,1,2,5]
    // Test case 4: [10,5,2,1,5,2]
    // Test case 5: [1,3,7,1,7,5]
    // Test case 6: [1,9,2,5,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
