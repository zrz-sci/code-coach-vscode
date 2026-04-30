/*
 * LeetCode 858: Mirror Reflection
 *
 * Approach: Math / GCD / Parity analysis
 *
 * Key Insight:
 *   Instead of simulating reflections, "unfold" the room into an infinite grid.
 *   The laser becomes a straight line from (0,0) with slope q/p.
 *   It hits a corner when it reaches a point (m*p, n*q) where both coordinates
 *   are multiples of p. The first such point determines which receptor is hit.
 *
 *   Let g = gcd(p, q). The number of vertical bounces = p/g, horizontal = q/g.
 *   Parity of these counts determines the receptor:
 *     - p/g odd, q/g odd  -> receptor 1 (top-right)
 *     - p/g odd, q/g even -> receptor 0 (bottom-right)
 *     - p/g even, q/g odd -> receptor 2 (top-left)
 *
 * Time:  O(log(min(p,q)))   -- for GCD computation
 * Space: O(1)
 */

// ===================== Solution 1: GCD-based Parity (Recommended) =====================

class Solution {
public:
    int mirrorReflection(int p, int q) {
        // Find GCD of p and q
        int g = __gcd(p, q);

        // Number of times the ray bounces vertically and horizontally
        // before reaching a corner receptor
        int m = p / g;  // vertical bounces (how many q-steps to reach multiple of p)
        int n = q / g;  // horizontal bounces

        // Determine receptor based on parity:
        // m odd, n odd  -> receptor 1 (right wall, top)
        // m odd, n even -> receptor 0 (right wall, bottom)
        // m even, n odd -> receptor 2 (left wall, top)
        // m even, n even -> impossible (would mean gcd wasn't fully reduced)

        if (m % 2 == 1 && n % 2 == 1) return 1;
        if (m % 2 == 1 && n % 2 == 0) return 0;
        return 2;  // m even, n odd
    }
};

// ===================== Solution 2: Divide-by-2 Simplification =====================
// Equivalent but more elegant: repeatedly divide both p and q by 2
// until at least one is odd.

class SolutionV2 {
public:
    int mirrorReflection(int p, int q) {
        // Keep dividing out common factors of 2
        while (p % 2 == 0 && q % 2 == 0) {
            p /= 2;
            q /= 2;
        }

        // Now at least one of p, q is odd
        if (p % 2 == 0) return 2;  // p even, q odd -> left wall top
        if (q % 2 == 0) return 0;  // p odd, q even -> right wall bottom
        return 1;                   // both odd -> right wall top
    }
};

// ===================== Solution 3: Simulation =====================
// Simulate the ray bouncing until it hits a receptor.
// Less elegant but very intuitive.

class SolutionV3 {
public:
    int mirrorReflection(int p, int q) {
        // Track vertical position and direction
        // The ray starts at (0, 0) heading to (p, q)
        // After each reflection off east/west wall, it continues with same vertical speed

        // We track the cumulative vertical distance traveled
        // When it's a multiple of p, the ray is at a corner
        int verticalPos = 0;
        int reflections = 0;

        // Simulate: each step the ray travels q units vertically
        // and bounces off east or west wall alternately
        do {
            verticalPos += q;
            reflections++;
        } while (verticalPos % p != 0);

        // verticalPos / p tells us how many "rooms" high we are
        int roomsUp = verticalPos / p;

        // reflections is odd -> ray is on east (right) wall
        // reflections is even -> ray is on west (left) wall
        // roomsUp is odd -> ray is at top of a room
        // roomsUp is even -> ray is at bottom of a room

        bool onEastWall = (reflections % 2 == 1);
        bool atTop = (roomsUp % 2 == 1);

        if (onEastWall && !atTop) return 0;   // right wall, bottom
        if (onEastWall && atTop) return 1;     // right wall, top
        if (!onEastWall && atTop) return 2;    // left wall, top

        // Should not reach here given problem constraints
        return -1;
    }
};

// ===================== Walkthrough with Example =====================
/*
 * Example 1: p = 2, q = 1
 *
 * GCD approach:
 *   g = gcd(2, 1) = 1
 *   m = p/g = 2 (even)
 *   n = q/g = 1 (odd)
 *   -> m even, n odd -> receptor 2  CORRECT!
 *
 * Divide-by-2 approach:
 *   p=2 even, q=1 odd -> can't divide both by 2
 *   p even -> return 2  CORRECT!
 *
 * Simulation:
 *   Step 1: verticalPos = 1, reflections = 1 -> 1 % 2 != 0, continue
 *   Step 2: verticalPos = 2, reflections = 2 -> 2 % 2 == 0, stop
 *   roomsUp = 2/2 = 1 (odd -> top)
 *   reflections = 2 (even -> west/left wall)
 *   Left wall, top -> receptor 2  CORRECT!
 *
 * ---------------------------------------------------------------
 * Example 2: p = 3, q = 1
 *
 * GCD approach:
 *   g = gcd(3, 1) = 1
 *   m = 3 (odd), n = 1 (odd)
 *   -> both odd -> receptor 1  CORRECT!
 *
 * ---------------------------------------------------------------
 * Visual "unfolding" for p=2, q=1:
 *
 *   The ray goes from (0,0) with slope 1/2.
 *   Unfolded rooms:
 *     y=4 |----|----|     Receptor positions (in unfolded grid):
 *     y=3 |    |    |       (0, even*p) = left bottom -> no receptor
 *     y=2 |----X----|       (p, odd*p)  = receptor 1 or 2
 *     y=1 |   /|    |       (0, odd*p)  = receptor 2 (left top)
 *     y=0 |--/-|----|       (p, even*p) = receptor 0 (right bottom)
 *         0   p   2p
 *
 *   Ray hits X at (2*1, 2*1) but that's (2, 2) = (p, p) -> right wall, top
 *   Wait -- let me re-check. The ray hits east wall at y=1 first time.
 *   Then reflects and hits west wall at y=2 -> receptor 2.
 */

// ===================== Complexity Analysis =====================
/*
 * Solution 1 (GCD):
 *   Time:  O(log(min(p,q))) for GCD via Euclidean algorithm
 *   Space: O(1)
 *
 * Solution 2 (Divide-by-2):
 *   Time:  O(log(max(p,q))) worst case dividing by 2
 *   Space: O(1)
 *
 * Solution 3 (Simulation):
 *   Time:  O(p / gcd(p,q)) steps until hitting a corner
 *   Space: O(1)
 *
 * Edge Cases:
 *   - p == q: ray goes directly to receptor 1 (diagonal)
 *   - q == 0: not possible per constraints (q >= 1)
 *   - p = 1000, q = 1: ray bounces 1000 times -> simulation works but slow
 */
