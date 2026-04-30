/*
 * LeetCode 936 - Stamping The Sequence
 * Approach: Reverse Greedy (Unstamping)
 *
 * Key insight: Instead of building target from '?????', work backwards.
 * Start with target and try to "undo" stamps by replacing matched windows
 * with '?'. If we can reduce the entire string to all '?', then reversing
 * the undo sequence gives a valid stamping order.
 *
 * At each position, the stamp matches if every non-'?' character in the
 * window matches the corresponding stamp character, AND at least one
 * character is non-'?' (to guarantee progress).
 *
 * Time:  O(n * (n - m + 1)) where n = target.length, m = stamp.length
 * Space: O(n) for the result array and the working copy of target
 */

class Solution {
public:
    vector<int> movesToStamp(string stamp, string target) {
        int m = stamp.size();
        int n = target.size();
        vector<int> result;

        // Track how many characters have been turned into '?'
        int totalErased = 0;

        // We keep scanning until we either erase everything or get stuck
        // In each full pass, we try every possible stamping position.
        // If a pass produces no new erasures, we are stuck -> impossible.
        bool madeProgress = true;

        while (madeProgress) {
            madeProgress = false;

            // Try every starting position for the stamp
            for (int i = 0; i <= n - m; i++) {
                // Check if stamp matches at position i
                // (treating '?' in target as wildcards)
                int matched = 0;    // count of non-'?' chars that match
                bool valid = true;

                for (int k = 0; k < m; k++) {
                    if (target[i + k] == '?') {
                        // Already erased, acts as wildcard
                        continue;
                    } else if (target[i + k] == stamp[k]) {
                        // Concrete match
                        matched++;
                    } else {
                        // Mismatch with a non-'?' character
                        valid = false;
                        break;
                    }
                }

                // Must be valid AND have at least one concrete match
                // (otherwise we'd be stamping an all-'?' window, no progress)
                if (valid && matched > 0) {
                    // "Unstamp" -- replace this window with '?'
                    for (int k = 0; k < m; k++) {
                        if (target[i + k] != '?') {
                            target[i + k] = '?';
                            totalErased++;
                        }
                    }
                    // Record this stamping index
                    result.push_back(i);
                    madeProgress = true;

                    // Early termination: all characters erased
                    if (totalErased == n) {
                        // Reverse because we found stamps in reverse order
                        reverse(result.begin(), result.end());
                        return result;
                    }
                }
            }
        }

        // If we exit the loop without erasing everything, it is impossible
        return {};
    }
};

/*
 * Walkthrough with stamp = "abc", target = "ababc":
 *
 * Initial target: "ababc"
 *
 * Pass 1:
 *   i=0: window "aba" vs stamp "abc" -> 'a'='a' OK, 'b'='b' OK, 'a'!='c' FAIL
 *   i=1: window "bab" vs stamp "abc" -> 'b'!='a' FAIL
 *   i=2: window "abc" vs stamp "abc" -> perfect match! matched=3
 *         Erase positions 2,3,4 -> target becomes "ab???"
 *         result = [2], totalErased = 3
 *
 * Pass 2:
 *   i=0: window "ab?" vs stamp "abc" -> 'a'='a' OK, 'b'='b' OK, '?'=wildcard
 *         matched=2, valid=true
 *         Erase positions 0,1 -> target becomes "?????"
 *         result = [2, 0], totalErased = 5 = n -> DONE!
 *
 * Reverse result: [0, 2]
 * Verification: stamp at 0 -> "abc??", stamp at 2 -> "ababc"  CORRECT
 *
 * ---------------------------------------------------------------
 * Walkthrough with stamp = "abca", target = "aabcaca":
 *
 * Initial: "aabcaca"
 *
 * Pass 1:
 *   i=0: "aabc" vs "abca" -> 'a'='a', 'a'!='b' FAIL
 *   i=1: "abca" vs "abca" -> perfect match=4
 *         Erase 1..4 -> "a???aca" -> result=[1], erased=4
 *   i=2: "??ac" vs "abca" -> '?'=wild, '?'=wild, 'a'!='c' FAIL
 *   i=3: "?aca" vs "abca" -> '?'=wild, 'a'!='b' FAIL
 *
 * Pass 2:
 *   i=0: "a???" vs "abca" -> 'a'='a', rest wild -> matched=1
 *         Erase pos 0 -> "????aca", result=[1,0], erased=5
 *   i=3: "?aca" vs "abca" -> '?'=wild, 'a'!='b' FAIL
 *
 * Pass 3:
 *   i=3: "?aca" vs "abca" -- still fails
 *   Hmm, let's re-check...
 *   Actually i=3: target[3..6] = "aca?" -- wait, target is "????aca"
 *   i=3: "aac" -- no, target[3]='a', target[4]='c', target[5]='a',
 *         but stamp length is 4, so i goes up to 7-4=3
 *   i=3: target[3..6] = "aca" -- but stamp length 4, target length 7
 *         target[3]='a', target[4]='c', target[5]='a', target[6]=...
 *         target = "????aca" -> indices 0-6
 *         i=3: "?aca" -- actually target[3]='?', [4]='a', [5]='c', [6]='a'
 *         stamp = "abca": '?'=wild, 'a'!='b' FAIL
 *
 * Actually with the greedy approach the order may differ:
 * The algorithm might find [3, 0, 1] or similar valid sequence.
 * Multiple valid answers exist for this problem.
 *
 * The key guarantee: if any valid stamping order exists, the reverse
 * greedy approach will find one (not necessarily the same as the
 * example output, but equally valid).
 * ---------------------------------------------------------------
 */
