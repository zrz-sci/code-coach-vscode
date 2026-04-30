/*
 * LeetCode 859: Buddy Strings
 *
 * Approach: Case analysis with character frequency tracking
 *
 * Key Insight:
 *   We need EXACTLY one swap of two positions in s to make it equal to goal.
 *   Two main cases:
 *   1. s != goal: exactly 2 positions differ, and swapping them makes a match.
 *   2. s == goal: at least one character appears 2+ times (swap those duplicates).
 *
 * Time:  O(n)
 * Space: O(1)  -- only 26 letter counts + at most 2-3 diff indices
 */

// ===================== Solution 1: Clean Case Analysis (Recommended) =====================

class Solution {
public:
    bool buddyStrings(string s, string goal) {
        // Different lengths -> impossible
        if (s.size() != goal.size()) return false;

        int n = s.size();

        // Collect positions where s and goal differ
        vector<int> diff;
        for (int i = 0; i < n; i++) {
            if (s[i] != goal[i]) {
                diff.push_back(i);
                // Early exit: more than 2 differences -> impossible with one swap
                if (diff.size() > 2) return false;
            }
        }

        // Case 1: No differences (s == goal)
        // Need at least one duplicate character to swap without changing the string
        if (diff.size() == 0) {
            // Check for duplicate characters using frequency array
            int freq[26] = {};
            for (char c : s) {
                freq[c - 'a']++;
                if (freq[c - 'a'] >= 2) return true;
            }
            return false;
        }

        // Case 2: Exactly 1 difference -> impossible with a single swap
        if (diff.size() == 1) return false;

        // Case 3: Exactly 2 differences -> check if swapping fixes both
        int i = diff[0], j = diff[1];
        return s[i] == goal[j] && s[j] == goal[i];
    }
};

// ===================== Solution 2: Single-Pass with Early Exit =====================
// All checks done in one traversal, slightly more compact

class SolutionV2 {
public:
    bool buddyStrings(string s, string goal) {
        if (s.size() != goal.size()) return false;

        int n = s.size();
        int first = -1, second = -1;
        int diffCount = 0;
        int freq[26] = {};
        bool hasDuplicate = false;

        for (int i = 0; i < n; i++) {
            // Track character frequencies for the "equal strings" case
            freq[s[i] - 'a']++;
            if (freq[s[i] - 'a'] >= 2) hasDuplicate = true;

            if (s[i] != goal[i]) {
                diffCount++;
                if (diffCount > 2) return false;  // More than 2 differences

                if (first == -1) first = i;
                else second = i;
            }
        }

        // Strings are identical -> need a duplicate character to swap
        if (diffCount == 0) return hasDuplicate;

        // Exactly 1 difference -> can't fix with one swap
        if (diffCount == 1) return false;

        // Exactly 2 differences -> cross-check
        return s[first] == goal[second] && s[second] == goal[first];
    }
};

// ===================== Solution 3: Using Set for Duplicate Check =====================

class SolutionV3 {
public:
    bool buddyStrings(string s, string goal) {
        if (s.size() != goal.size()) return false;

        // Find all differing positions
        vector<int> diff;
        for (int i = 0; i < (int)s.size(); i++) {
            if (s[i] != goal[i]) {
                diff.push_back(i);
            }
        }

        if (diff.size() == 0) {
            // s == goal: check for duplicate characters
            // If the set of characters is smaller than string length, duplicates exist
            set<char> chars(s.begin(), s.end());
            return chars.size() < s.size();
        }

        if (diff.size() == 2) {
            // Exactly two positions differ: check if swap makes them match
            return s[diff[0]] == goal[diff[1]] && s[diff[1]] == goal[diff[0]];
        }

        // 1 or 3+ differences -> impossible
        return false;
    }
};

// ===================== Walkthrough with Examples =====================
/*
 * Example 1: s = "ab", goal = "ba"
 *   Length check: 2 == 2, OK
 *   Differences: pos 0 (a vs b), pos 1 (b vs a) -> diff = [0, 1]
 *   diff.size() == 2
 *   Check: s[0]=='a' == goal[1]=='a' YES, s[1]=='b' == goal[0]=='b' YES
 *   Return true
 *
 * Example 2: s = "ab", goal = "ab"
 *   Differences: none -> diff = []
 *   diff.size() == 0 -> check for duplicates
 *   freq: a=1, b=1 -> no duplicates
 *   Return false
 *   (We MUST swap but any swap would change the string)
 *
 * Example 3: s = "aa", goal = "aa"
 *   Differences: none -> diff = []
 *   diff.size() == 0 -> check for duplicates
 *   freq: a=2 -> has duplicate!
 *   Return true
 *   (Swap s[0] and s[1]: both 'a', string unchanged, still equals goal)
 *
 * Edge case: s = "ab", goal = "ca"
 *   Differences: pos 0 (a vs c), pos 1 (b vs a) -> diff = [0, 1]
 *   Check: s[0]=='a' == goal[1]=='a' YES, but s[1]=='b' == goal[0]=='c' NO
 *   Return false
 *   (Swapping positions 0 and 1 gives "ba", not "ca")
 *
 * Edge case: s = "abc", goal = "cba"
 *   Differences: pos 0 (a vs c), pos 2 (c vs a) -> diff = [0, 2]
 *   Wait, also pos 1: b vs b -> same. So diff = [0, 2]
 *   Check: s[0]=='a' == goal[2]=='a' YES, s[2]=='c' == goal[0]=='c' YES
 *   Return true
 *
 * Edge case: s = "abcd", goal = "dcba"
 *   Differences: pos 0,1,2,3 -> diff.size() > 2
 *   Return false (early exit)
 */

// ===================== Complexity Analysis =====================
/*
 * Time Complexity: O(n)
 *   - Single pass through both strings
 *   - Early exit when diff count exceeds 2
 *
 * Space Complexity: O(1)
 *   - freq[26] is constant space
 *   - diff vector stores at most 3 elements before early exit
 *
 * Edge Cases to consider:
 *   - s.length == 1: can't swap two different positions -> always false
 *     (unless goal.length != 1, then also false)
 *     Wait: s="a", goal="a" -> diff=0, but no duplicate possible with 1 char
 *     Actually freq[a]=1, no dup -> false. Correct! Can't swap in single char.
 *   - All same characters: s="aaa", goal="aaa" -> diff=0, hasDup=true -> true
 *   - s and goal have same chars but wrong arrangement with 3+ diffs: false
 */
