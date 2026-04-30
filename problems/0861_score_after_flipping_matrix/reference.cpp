/*
 * LeetCode 861: Score After Flipping Matrix
 *
 * Approach: Greedy - ensure MSB is 1 for every row, then maximize each column
 *
 * Key Insight:
 *   In binary, the MSB (leftmost bit) has weight 2^(n-1), which is more than
 *   the sum of all remaining bits (2^(n-1) - 1). So we MUST make the first
 *   column all 1s by flipping rows. Then for each subsequent column, flip it
 *   if doing so would increase the count of 1s.
 *
 * Time:  O(m * n)
 * Space: O(1)
 */

// ===================== Solution 1: In-Place Modification =====================

class Solution {
public:
    int matrixScore(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();

        // Step 1: Flip rows to ensure first column is all 1s
        // If grid[i][0] == 0, flip the entire row
        for (int i = 0; i < m; i++) {
            if (grid[i][0] == 0) {
                for (int j = 0; j < n; j++) {
                    grid[i][j] ^= 1;  // Toggle: 0->1, 1->0
                }
            }
        }

        // Step 2: For each column (starting from col 1), flip if more 0s than 1s
        for (int j = 1; j < n; j++) {
            int ones = 0;
            for (int i = 0; i < m; i++) {
                ones += grid[i][j];
            }

            // If fewer 1s than 0s, flip the column
            if (ones * 2 < m) {
                for (int i = 0; i < m; i++) {
                    grid[i][j] ^= 1;
                }
            }
        }

        // Step 3: Calculate the total score
        int score = 0;
        for (int i = 0; i < m; i++) {
            int rowVal = 0;
            for (int j = 0; j < n; j++) {
                rowVal = (rowVal << 1) | grid[i][j];
            }
            score += rowVal;
        }

        return score;
    }
};

// ===================== Solution 2: Math-Only (No Matrix Modification) =====================
// Computes the answer without actually flipping any bits.
// For each column, determine the "effective" number of 1s.

class SolutionV2 {
public:
    int matrixScore(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int score = 0;

        // Column 0: after row flips, all entries become 1
        // Contribution: m * 2^(n-1)
        score += m * (1 << (n - 1));

        // Columns 1 to n-1:
        for (int j = 1; j < n; j++) {
            int ones = 0;
            for (int i = 0; i < m; i++) {
                // If row i was flipped (grid[i][0] was originally 0),
                // then grid[i][j]'s effective value is flipped too.
                // Effective value = grid[i][j] XOR (grid[i][0] == 0)
                // Which simplifies to: grid[i][j] == grid[i][0] means effective 1
                if (grid[i][j] == grid[i][0]) {
                    ones++;
                }
            }

            // We can flip this column, so take max(ones, m - ones)
            int effectiveOnes = max(ones, m - ones);
            score += effectiveOnes * (1 << (n - 1 - j));
        }

        return score;
    }
};

// ===================== Solution 3: Bit-by-Bit Contribution =====================
// Same idea as Solution 2, written differently for clarity

class SolutionV3 {
public:
    int matrixScore(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();

        // Determine which rows should be flipped (those with grid[i][0] == 0)
        // We don't actually flip, just track it
        // rowFlipped[i] = true if grid[i][0] == 0
        // But we can avoid the array by checking grid[i][0] inline

        int totalScore = 0;

        for (int j = 0; j < n; j++) {
            int columnWeight = 1 << (n - 1 - j);
            int onesInColumn = 0;

            for (int i = 0; i < m; i++) {
                // Effective value of grid[i][j] after optimal row flips
                int effectiveVal = grid[i][j];
                if (grid[i][0] == 0) {
                    effectiveVal ^= 1;  // This row was "flipped"
                }
                onesInColumn += effectiveVal;
            }

            // After optimal column flip: take the better of flipping or not
            // If j == 0, onesInColumn == m (all 1s after row flips), no need to flip
            // For j > 0, choose max(onesInColumn, m - onesInColumn)
            int bestOnes = max(onesInColumn, m - onesInColumn);
            totalScore += bestOnes * columnWeight;
        }

        return totalScore;
    }
};

// ===================== Walkthrough with Example =====================
/*
 * Example: grid = [[0,0,1,1],[1,0,1,0],[1,1,0,0]]
 *
 * Solution 1 (In-Place):
 *   Step 1 - Row flips (make col 0 all 1s):
 *     Row 0: grid[0][0]=0 -> flip -> [1,1,0,0]
 *     Row 1: grid[1][0]=1 -> no flip -> [1,0,1,0]
 *     Row 2: grid[2][0]=1 -> no flip -> [1,1,0,0]
 *     Matrix now: [[1,1,0,0],[1,0,1,0],[1,1,0,0]]
 *
 *   Step 2 - Column flips:
 *     Col 0: all 1s (3 ones) -> skip
 *     Col 1: ones=2 (1,0,1) -> 2*2 >= 3 -> skip (2 ones > 1 zero)
 *     Col 2: ones=1 (0,1,0) -> 1*2 < 3 -> flip! -> becomes (1,0,1)
 *     Col 3: ones=0 (0,0,0) -> 0*2 < 3 -> flip! -> becomes (1,1,1)
 *     Matrix now: [[1,1,1,1],[1,0,0,1],[1,1,1,1]]
 *
 *   Step 3 - Calculate:
 *     Row 0: 1111 = 15
 *     Row 1: 1001 = 9
 *     Row 2: 1111 = 15
 *     Total: 15 + 9 + 15 = 39  CORRECT!
 *
 * Solution 2 (Math-Only):
 *   Col 0: score += 3 * 8 = 24
 *   Col 1: ones where grid[i][1]==grid[i][0]: row0(0==0 yes), row1(0==1 no), row2(1==1 yes) -> ones=2
 *          max(2, 1) = 2, score += 2 * 4 = 8
 *   Col 2: row0(1!=0 -> eff=1^1=0.. wait let me recompute)
 *          row0: grid[0][0]=0, so flipped. grid[0][2]=1 XOR 1 = 0. Wait: grid[0][2]==grid[0][0]? 1==0? No.
 *          Hmm, ones where grid[i][j]==grid[i][0]:
 *          row0: grid[0][2]=1, grid[0][0]=0 -> 1!=0 -> not counted
 *          row1: grid[1][2]=1, grid[1][0]=1 -> 1==1 -> counted, ones=1
 *          row2: grid[2][2]=0, grid[2][0]=1 -> 0!=1 -> not counted
 *          ones=1, max(1,2)=2, score += 2*2 = 4
 *   Col 3: row0: 1!=0 no, row1: 0!=1 no, row2: 0!=1 no -> ones=0
 *          max(0,3)=3, score += 3*1 = 3
 *   Total: 24 + 8 + 4 + 3 = 39  CORRECT!
 */

// ===================== Complexity Analysis =====================
/*
 * Time Complexity: O(m * n)
 *   - Each solution iterates through the entire matrix once or twice
 *   - Solution 1: two passes (flip + calculate)
 *   - Solution 2/3: single pass per column, m iterations per column
 *
 * Space Complexity: O(1)
 *   - All solutions use only constant extra space
 *   - Solution 1 modifies the input matrix in-place
 *   - Solutions 2/3 don't modify the input at all
 *
 * Why Greedy Works:
 *   - MSB has weight 2^(n-1), sum of all other bits = 2^(n-1) - 1
 *   - So ensuring MSB = 1 is always optimal regardless of other columns
 *   - After row flips fix MSB, each column is independent
 *   - For each column, maximizing 1s is clearly optimal
 *   - Row and column flips commute, so order doesn't matter
 */
