// LeetCode 914: X of a Kind in a Deck of Cards
// Approach: Frequency Count + GCD
// Time: O(n), Space: O(k) where k = distinct values

// ============================================================
// Core Insight:
// If we count the frequency of each card value, we need a single
// x >= 2 that divides ALL frequencies. This x exists if and only
// if the GCD of all frequencies is >= 2.
//
// Proof:
// Necessity: If x divides every frequency, then x divides GCD.
//            So GCD >= x >= 2.
// Sufficiency: If GCD >= 2, set x = GCD. Then x divides every
//              frequency (by definition of GCD), so we can
//              partition each group of count_i cards into
//              count_i / x groups of x cards each.
// ============================================================

// GCD using the Euclidean algorithm
// gcd(a, b) runs in O(log(min(a, b))) time
int gcdHelper(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

class Solution {
public:
    bool hasGroupsSizeX(vector<int>& deck) {
        // Step 1: Count frequency of each card value
        unordered_map<int, int> freq;
        for (int card : deck) {
            freq[card]++;
        }

        // Step 2: Compute GCD of all frequencies
        int g = 0;
        for (auto& [val, count] : freq) {
            g = gcdHelper(g, count);
        }

        // Step 3: Check if GCD >= 2
        // (Note: gcd(0, x) = x, so initializing g=0 works correctly
        //  because the first gcd call becomes gcd(0, count1) = count1)
        return g >= 2;
    }
};

// ============================================================
// Walkthrough Example 1: deck = [1, 2, 3, 4, 4, 3, 2, 1]
// ============================================================
// Frequencies: {1: 2, 2: 2, 3: 2, 4: 2}
// GCD computation:
//   g = gcd(0, 2) = 2
//   g = gcd(2, 2) = 2
//   g = gcd(2, 2) = 2
//   g = gcd(2, 2) = 2
// g = 2 >= 2 -> return true
// Partition: [1,1], [2,2], [3,3], [4,4] with x=2
// ============================================================

// ============================================================
// Walkthrough Example 2: deck = [1, 1, 1, 2, 2, 2, 3, 3]
// ============================================================
// Frequencies: {1: 3, 2: 3, 3: 2}
// GCD computation:
//   g = gcd(0, 3) = 3
//   g = gcd(3, 3) = 3
//   g = gcd(3, 2) = 1     <-- GCD drops to 1
// g = 1 < 2 -> return false
// No valid x: x=2 fails (3 not divisible), x=3 fails (2 not divisible)
// ============================================================

// ============================================================
// Alternative: Using C++ built-in __gcd or std::gcd (C++17)
// ============================================================
class SolutionBuiltinGCD {
public:
    bool hasGroupsSizeX(vector<int>& deck) {
        unordered_map<int, int> freq;
        for (int card : deck) {
            freq[card]++;
        }

        int g = 0;
        for (auto& [val, count] : freq) {
            g = __gcd(g, count);
        }

        return g >= 2;
    }
};

// ============================================================
// Alternative: Sort-based approach (no hash map, O(1) extra space)
// ============================================================
// Sort the deck, then count consecutive equal elements.
// Compute GCD on the fly.
// ============================================================
class SolutionSortBased {
public:
    bool hasGroupsSizeX(vector<int>& deck) {
        sort(deck.begin(), deck.end());
        int n = deck.size();
        int g = 0;

        int i = 0;
        while (i < n) {
            int j = i;
            // Count consecutive equal elements
            while (j < n && deck[j] == deck[i]) {
                j++;
            }
            int count = j - i;
            g = __gcd(g, count);
            i = j;
        }

        return g >= 2;
    }
};

// ============================================================
// Alternative: Try all possible x values (brute force)
// ============================================================
// Instead of GCD, try each x from 2 to max_count and check
// if x divides all frequencies.
// ============================================================
class SolutionBruteForce {
public:
    bool hasGroupsSizeX(vector<int>& deck) {
        unordered_map<int, int> freq;
        for (int card : deck) {
            freq[card]++;
        }

        // Find max frequency to bound our search
        int maxCount = 0;
        for (auto& [val, count] : freq) {
            maxCount = max(maxCount, count);
        }

        // Try each possible x from 2 to maxCount
        for (int x = 2; x <= maxCount; x++) {
            bool valid = true;
            for (auto& [val, count] : freq) {
                if (count % x != 0) {
                    valid = false;
                    break;
                }
            }
            if (valid) return true;
        }

        return false;
    }
};

// ============================================================
// Edge Case Analysis
// ============================================================
//
// Case 1: Single card - deck = [7]
//   freq = {7: 1}, g = 1 < 2 -> false
//   Correct: cannot form a group of size x >= 2 with 1 card.
//
// Case 2: Two identical cards - deck = [5, 5]
//   freq = {5: 2}, g = 2 >= 2 -> true
//   Partition: [5, 5] with x = 2.
//
// Case 3: All same - deck = [3, 3, 3, 3, 3, 3]
//   freq = {3: 6}, g = 6 >= 2 -> true
//   Multiple valid x: 2 (three groups), 3 (two groups), 6 (one group)
//
// Case 4: Coprime counts - deck = [1,1,1,1,1,2,2,2]
//   freq = {1: 5, 2: 3}, g = gcd(5, 3) = 1 < 2 -> false
//   No x >= 2 divides both 5 and 3.
//
// Case 5: Large common factor - deck with counts [12, 8, 20]
//   g = gcd(12, gcd(8, 20)) = gcd(12, 4) = 4 >= 2 -> true
//   x = 4: groups of 3, 2, 5 respectively.
//   x = 2 also works: groups of 6, 4, 10 respectively.
// ============================================================

// ============================================================
// Mathematical Notes on GCD
// ============================================================
// The Euclidean algorithm: gcd(a, b) = gcd(b, a % b)
// Base case: gcd(a, 0) = a
//
// Properties used:
// 1. gcd(0, x) = x (identity element, useful for initialization)
// 2. gcd is associative: gcd(a, gcd(b, c)) = gcd(gcd(a, b), c)
//    This allows computing GCD of a list by folding.
// 3. If gcd(all counts) = g, then g divides every count.
//    And g is the LARGEST such divisor.
//
// Time complexity of single gcd call: O(log(min(a, b)))
// Total GCD computation for k values: O(k * log(M)) where
// M = maximum frequency. In practice, very fast.
// ============================================================
