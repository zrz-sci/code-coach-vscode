// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 492: Construct the Rectangle
 * Difficulty: Easy
 * Tags: Math
 *
 * Problem Description:
 * A web developer needs to know how to design a web page's size. So, given a
 * specific rectangular web page's area, your job by now is to design a
 * rectangular
 * web page, whose length L and width W satisfy the following requirements:
 * 
 * 1. The area of the rectangular web page you designed must equal to the
 * given target area.
 * 2. The width `W` should not be larger than the length `L`, which means `L
 * >= W`.
 * 3. The difference between length `L` and width `W` should be as small as
 * possible.
 * 
 * Return _an array`[L, W]` where `L` and `W` are the length and width of the
 * web
 * page you designed in sequence._
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** area = 4
 *     **Output:** [2,2]
 * **Explanation:** The target area is 4, and all the possible ways to
 * construct it are [1,4], [2,2], [4,1].
 * But according to requirement 2, [1,4] is illegal; according to requirement
 * 3, [4,1] is not optimal compared to [2,2]. So the length L is 2, and the
 * width W is 2.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** area = 37
 *     **Output:** [37,1]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** area = 122122
 *     **Output:** [427,286]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= area <= 107`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/construct-the-rectangle/
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
    vector<int> constructRectangle(int area) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 4
    // Test case 2: 37
    // Test case 3: 122122

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
