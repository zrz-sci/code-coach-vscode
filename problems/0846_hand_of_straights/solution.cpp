// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 846: Hand of Straights
 * Difficulty: Medium
 * Tags: Greedy, Array, Hash Table, Sorting
 *
 * Problem Description:
 * Alice has some number of cards and she wants to rearrange the cards into
 * groups
 * so that each group is of size `groupSize`, and consists of `groupSize`
 * consecutive cards.
 * 
 * Given an integer array `hand` where `hand[i]` is the value written on the
 * `ith`
 * card and an integer `groupSize`, return `true` if she can rearrange the
 * cards,
 * or `false` otherwise.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** hand = [1,2,3,6,2,3,4,7,8], groupSize = 3
 *     **Output:** true
 * **Explanation:** Alice's hand can be rearranged as [1,2,3],[2,3,4],[6,7,8]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** hand = [1,2,3,4,5], groupSize = 4
 *     **Output:** false
 *     **Explanation:** Alice's hand can not be rearranged into groups of 4.
 *     
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= hand.length <= 104`
 *   * `0 <= hand[i] <= 109`
 *   * `1 <= groupSize <= hand.length`
 * 
 * 
 * 
 * **Note:** This question is the same as 1296:
 * <https://leetcode.com/problems/divide-array-in-sets-of-k-consecutive-numbers/>
 * 
 * 
 *
 * Link: https://leetcode.com/problems/hand-of-straights/
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
    bool isNStraightHand(vector<int>& hand, int groupSize) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,6,2,3,4,7,8]
    // Test case 2: 3
    // Test case 3: [1,2,3,4,5]
    // Test case 4: 4

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
