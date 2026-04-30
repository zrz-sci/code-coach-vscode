/*
 * LeetCode 989: Add to Array-Form of Integer
 *
 * Approach 1: Unified carry (treat k as initial carry)
 * - Start with carry = k and process digits of num from right to left.
 * - At each step, add the current digit of num to carry, extract the
 *   result digit (carry % 10), and propagate (carry /= 10).
 * - After processing all digits of num, continue while carry > 0.
 * - Reverse the result at the end.
 *
 * Time:  O(max(N, log k)) where N = num.size()
 * Space: O(max(N, log k)) for the result array
 */

class Solution {
public:
    vector<int> addToArrayForm(vector<int>& num, int k) {
        vector<int> result;
        int carry = k;
        int i = num.size() - 1;

        while (i >= 0 || carry > 0) {
            if (i >= 0) {
                carry += num[i];
                i--;
            }
            result.push_back(carry % 10);
            carry /= 10;
        }

        reverse(result.begin(), result.end());
        return result;
    }
};


/*
 * Approach 2: Explicit carry with separate k digit extraction
 * - Maintains a separate carry variable and extracts digits from k
 *   independently. More verbose but easier to understand for some.
 * - Process from right to left: sum = num[i] + (k % 10) + carry
 * - Result digit = sum % 10, carry = sum / 10, k /= 10
 *
 * Time:  O(max(N, log k))
 * Space: O(max(N, log k))
 */

class Solution2 {
public:
    vector<int> addToArrayForm(vector<int>& num, int k) {
        vector<int> result;
        int carry = 0;
        int i = num.size() - 1;

        while (i >= 0 || k > 0 || carry > 0) {
            int sum = carry;

            if (i >= 0) {
                sum += num[i];
                i--;
            }

            if (k > 0) {
                sum += k % 10;
                k /= 10;
            }

            result.push_back(sum % 10);
            carry = sum / 10;
        }

        reverse(result.begin(), result.end());
        return result;
    }
};


/*
 * Approach 3: In-place modification of num (space optimization)
 * - Instead of creating a new result vector, modify num in place.
 * - Process from right to left, updating num[i] with the result digit.
 * - If carry remains after processing all digits, insert new digits
 *   at the front (or use a deque approach).
 *
 * Time:  O(max(N, log k))
 * Space: O(1) extra beyond the output (modifying num in place)
 *
 * Note: Inserting at the front of a vector is O(N), so if many digits
 * need to be prepended, this approach may not be faster in practice.
 * Using a deque or building in reverse avoids this issue.
 */

class Solution3 {
public:
    vector<int> addToArrayForm(vector<int>& num, int k) {
        int carry = k;

        // Process existing digits from right to left
        for (int i = num.size() - 1; i >= 0; i--) {
            carry += num[i];
            num[i] = carry % 10;
            carry /= 10;
        }

        // If carry remains, we need to add digits at the front
        // Build them in a temporary vector and prepend
        vector<int> prefix;
        while (carry > 0) {
            prefix.push_back(carry % 10);
            carry /= 10;
        }

        // Reverse prefix (it was built least-significant first)
        reverse(prefix.begin(), prefix.end());

        // Prepend to num
        // Insert prefix at the beginning of num
        num.insert(num.begin(), prefix.begin(), prefix.end());

        return num;
    }
};


/*
 * Key insights and common pitfalls:
 *
 * 1. Do NOT convert the array to a single integer. The array can have
 *    up to 10,000 digits, which cannot fit in any integer type (even
 *    long long only handles ~18 digits). Process digit by digit.
 *
 * 2. The "carry = k" trick is elegant: instead of separately handling
 *    k's digits and carries, you merge them. At each step, carry holds
 *    whatever needs to be added to the current position. Initially
 *    that is k, and as you process digits, it naturally becomes the
 *    propagated carry value.
 *
 * 3. Remember to handle the case where k has more digits than num.
 *    After i goes below 0, the carry (which contains remaining digits
 *    of k plus any propagated carry) continues to be processed.
 *
 * 4. The result may be longer than num. For example:
 *    [9,9,9] + 1 = [1,0,0,0] (4 digits from 3)
 *    [1] + 9999 = [1,0,0,0,0] (5 digits from 1)
 *
 * 5. Edge case: num = [0], k = 0 should return [0], not empty.
 *    The algorithm handles this because carry starts at 0, num[0] = 0,
 *    result gets one digit (0), and carry becomes 0, ending the loop.
 *
 * 6. Comparison with related problems:
 *    - "Add Two Numbers" (LC 2) uses linked lists with reversed digits
 *    - "Plus One" (LC 66) is the special case where k = 1
 *    - "Add Binary" (LC 67) uses base 2 instead of base 10
 *    All share the same fundamental pattern of digit-wise addition
 *    with carry propagation.
 */
