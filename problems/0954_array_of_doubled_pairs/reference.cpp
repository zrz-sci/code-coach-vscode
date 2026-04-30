// LeetCode 954: Array of Doubled Pairs
// Difficulty: Medium
// Tags: Greedy, Array, Hash Table, Sorting
// Link: https://leetcode.com/problems/array-of-doubled-pairs/
//
// Approach: Sort by absolute value + greedy matching with frequency map.
// For each element (processed smallest abs first), consume it and its double.
// Absolute-value sorting guarantees we always process x before 2x.
//
// Time:  O(n log n) for sorting
// Space: O(n) for the frequency map

// ============================================================
// Solution 1: Sort by Absolute Value + Hash Map (Optimal)
// ============================================================
// Core idea: Process elements from smallest |value| to largest.
// For each x, if it hasn't been consumed, find and consume 2x.
// Negative numbers work naturally: sort by abs puts -2 before -4,
// and -2 * 2 = -4 correctly identifies the partner.

class Solution {
public:
    bool canReorderDoubled(vector<int>& arr) {
        // Build a frequency map: value -> remaining count
        unordered_map<int, int> freq;
        for (int x : arr) {
            freq[x]++;
        }

        // Sort by absolute value so we process x before 2x
        // For negatives: |-2| < |-4|, so -2 processed first -> looks for -4
        // For positives: |1| < |2|, so 1 processed first -> looks for 2
        // For zero: |0| is smallest, zeros pair with zeros
        sort(arr.begin(), arr.end(), [](int a, int b) {
            return abs(a) < abs(b);
        });

        // Greedy matching pass
        for (int x : arr) {
            // This element was already consumed as someone's double
            if (freq[x] == 0) {
                continue;
            }

            // Check if the double of x exists and is available
            int target = 2 * x;
            if (freq[target] <= 0) {
                // No available partner for x -> impossible to pair
                return false;
            }

            // Consume the pair (x, 2x)
            freq[x]--;
            freq[target]--;
        }

        // All elements successfully paired
        return true;
    }
};


// ============================================================
// Solution 2: Ordered Map (TreeMap) Approach
// ============================================================
// Instead of sorting the array, use a std::map which keeps keys
// sorted. Iterate from smallest to largest key and match greedily.
// For negatives, we iterate from most-negative to least-negative,
// which means we process -4 before -2. We handle this by looking
// for x/2 when x is negative and even, or 2x when positive.
// Alternatively, split into negatives (reversed) and non-negatives.

class SolutionOrderedMap {
public:
    bool canReorderDoubled(vector<int>& arr) {
        map<int, int> freq;
        for (int x : arr) {
            freq[x]++;
        }

        // Process keys in sorted order (ascending)
        // For non-negatives: process 0, 1, 2, 3, ... -> match x with 2x
        // For negatives: process -10, -8, -4, -2, ... -> match x with x/2
        //   BUT we need to match -2 with -4, i.e., smaller abs first.
        //   Since map goes from most-negative to 0, we iterate and for
        //   negative x, we match x with 2x (e.g., -4 looks for -8),
        //   which is wrong. Instead we iterate by abs:

        // Collect keys sorted by absolute value
        vector<int> keys;
        keys.reserve(freq.size());
        for (auto& [k, v] : freq) {
            keys.push_back(k);
        }
        sort(keys.begin(), keys.end(), [](int a, int b) {
            return abs(a) < abs(b);
        });

        for (int x : keys) {
            // Process all remaining copies of x
            while (freq[x] > 0) {
                int target = 2 * x;
                if (freq[target] <= 0) {
                    return false;
                }
                freq[x]--;
                freq[target]--;
            }
        }

        return true;
    }
};


// ============================================================
// Solution 3: Multiset Approach (Alternative)
// ============================================================
// Use a multiset for automatic sorting. Extract the smallest
// absolute value element, find and erase its double.
// Slightly less efficient due to multiset operations but
// conceptually clean.

class SolutionMultiset {
public:
    bool canReorderDoubled(vector<int>& arr) {
        // Custom comparator: sort by absolute value, break ties by actual value
        auto cmp = [](int a, int b) {
            if (abs(a) != abs(b)) return abs(a) < abs(b);
            return a < b;
        };
        multiset<int, decltype(cmp)> ms(cmp);

        for (int x : arr) {
            ms.insert(x);
        }

        while (!ms.empty()) {
            // Take the element with smallest absolute value
            int x = *ms.begin();
            ms.erase(ms.begin());

            // Find its double
            int target = 2 * x;
            auto it = ms.find(target);
            if (it == ms.end()) {
                return false;
            }
            ms.erase(it);  // erase exactly one occurrence
        }

        return true;
    }
};


// ============================================================
// Solution 4: Separate Negatives and Positives
// ============================================================
// Split array into negatives and non-negatives.
// For non-negatives: sort ascending, match x with 2x.
// For negatives: sort descending (closest to 0 first), match x with 2x.
// This avoids the absolute-value sort trick.

class SolutionSplit {
public:
    bool canReorderDoubled(vector<int>& arr) {
        vector<int> neg, pos;
        for (int x : arr) {
            if (x < 0) neg.push_back(x);
            else pos.push_back(x);
        }

        // Negatives: sort descending so -1 comes before -2
        sort(neg.begin(), neg.end(), greater<int>());
        // Non-negatives: sort ascending so 0 comes before 1
        sort(pos.begin(), pos.end());

        // Helper lambda to check one half
        auto check = [](vector<int>& v) -> bool {
            unordered_map<int, int> freq;
            for (int x : v) freq[x]++;
            for (int x : v) {
                if (freq[x] == 0) continue;
                if (freq[2 * x] <= 0) return false;
                freq[x]--;
                freq[2 * x]--;
            }
            return true;
        };

        return check(neg) && check(pos);
    }
};


// ============================================================
// Walkthrough with Example: arr = [4, -2, 2, -4]
// ============================================================
//
// Step 1 - Build frequency map:
//   freq = {4: 1, -2: 1, 2: 1, -4: 1}
//
// Step 2 - Sort by absolute value:
//   sorted = [-2, 2, 4, -4]  (|vals| = [2, 2, 4, 4])
//   (tie-breaking order between -2 and 2 doesn't matter)
//
// Step 3 - Greedy matching:
//   x = -2: freq[-2]=1 > 0. target = -4. freq[-4]=1 > 0.
//           Consume: freq[-2]=0, freq[-4]=0.
//   x = 2:  freq[2]=1 > 0. target = 4. freq[4]=1 > 0.
//           Consume: freq[2]=0, freq[4]=0.
//   x = 4:  freq[4]=0 -> skip (already consumed).
//   x = -4: freq[-4]=0 -> skip (already consumed).
//
// All elements paired -> return true.
//
// Walkthrough with Example: arr = [3, 1, 3, 6]
//
// freq = {3: 2, 1: 1, 6: 1}
// sorted by abs = [1, 3, 3, 6]
//
// x = 1: freq[1]=1 > 0. target = 2. freq[2]=0 -> return false.
//
// Correct! [3,1,3,6] cannot form doubled pairs.
