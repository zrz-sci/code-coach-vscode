/*
 * LeetCode 941 - Valid Mountain Array
 * Approach: Two-Pointer Climb from both ends
 *
 * Two pointers start at opposite ends of the array and each "climbs"
 * uphill.  If they meet at the same interior index, the array is a
 * valid mountain.
 *
 * Requirements for a valid mountain:
 *   1. arr.length >= 3
 *   2. Strict increase from index 0 to some peak i
 *   3. Strict decrease from peak i to index n-1
 *   4. Peak i is not at index 0 or n-1
 *
 * Time:  O(n) -- each element visited at most twice
 * Space: O(1) -- only two integer pointers
 */

class Solution {
public:
    bool validMountainArray(vector<int>& arr) {
        int n = arr.size();

        // A mountain needs at least 3 elements
        if (n < 3) return false;

        // ---- Left pointer: climb from the left ----
        // Move right as long as the array is strictly increasing
        int left = 0;
        while (left + 1 < n && arr[left] < arr[left + 1]) {
            left++;
        }

        // ---- Right pointer: climb from the right ----
        // Move left as long as the array is strictly increasing
        // (i.e., from the right side, it looks like going uphill)
        int right = n - 1;
        while (right - 1 >= 0 && arr[right] < arr[right - 1]) {
            right--;
        }

        // ---- Validate the meeting point ----
        // Both pointers must meet at the same index,
        // and that index must NOT be at either boundary.
        //
        // left == right   -> both climbs reached the same peak
        // left != 0       -> there is a genuine ascending portion
        // right != n - 1  -> there is a genuine descending portion
        return left == right && left != 0 && right != n - 1;
    }
};

/*
 * Walkthrough with arr = [0, 3, 2, 1]:
 *
 *   n = 4
 *
 *   Left climb:
 *     left=0: arr[0]=0 < arr[1]=3 -> YES, left=1
 *     left=1: arr[1]=3 < arr[2]=2 -> NO, stop.  left=1
 *
 *   Right climb:
 *     right=3: arr[3]=1 < arr[2]=2 -> YES, right=2
 *     right=2: arr[2]=2 < arr[1]=3 -> YES, right=1
 *     right=1: arr[1]=3 < arr[0]=0 -> NO, stop.  right=1
 *
 *   Check: left=1, right=1
 *     left == right? YES
 *     left != 0?     YES
 *     right != 3?    YES
 *   Return TRUE  -- CORRECT
 *
 * ---------------------------------------------------------------
 * Walkthrough with arr = [2, 1]:
 *
 *   n = 2 < 3 -> return FALSE immediately
 *
 * ---------------------------------------------------------------
 * Walkthrough with arr = [3, 5, 5]:
 *
 *   n = 3
 *
 *   Left climb:
 *     left=0: arr[0]=3 < arr[1]=5 -> YES, left=1
 *     left=1: arr[1]=5 < arr[2]=5 -> NO (equal!), stop.  left=1
 *
 *   Right climb:
 *     right=2: arr[2]=5 < arr[1]=5 -> NO (equal!), stop.  right=2
 *
 *   Check: left=1, right=2
 *     left == right? NO
 *   Return FALSE  -- CORRECT (plateau at top violates strict mountain)
 *
 * ---------------------------------------------------------------
 * Walkthrough with arr = [1, 2, 3, 4, 5]:  (monotonically increasing)
 *
 *   n = 5
 *
 *   Left climb:
 *     left goes 0 -> 1 -> 2 -> 3 -> 4 (end of array).  left=4
 *
 *   Right climb:
 *     right=4: arr[4]=5 < arr[3]=4 -> NO, stop.  right=4
 *
 *   Check: left=4, right=4
 *     left == right? YES
 *     right != n-1 = 4? NO (right is at the boundary)
 *   Return FALSE  -- CORRECT (no descending part)
 *
 * ---------------------------------------------------------------
 * Alternative approach: State Machine
 *
 * Walk through the array with a state variable:
 *   state = "ASCENDING"
 *
 *   for i from 1 to n-1:
 *     if state == "ASCENDING":
 *       if arr[i] > arr[i-1]: continue
 *       else if arr[i] < arr[i-1] and i > 1: state = "DESCENDING"
 *       else: return false  (equal or started descending at i=1)
 *     else:  // DESCENDING
 *       if arr[i] < arr[i-1]: continue
 *       else: return false  (increase or plateau after the peak)
 *
 *   return state == "DESCENDING"
 *
 * This is also O(n) time, O(1) space, and equally valid.
 * The two-pointer approach is preferred for its symmetry and
 * minimal branching.
 * ---------------------------------------------------------------
 */
