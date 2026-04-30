// LeetCode 911: Online Election
// Approach: Precompute leaders at each vote event + Binary Search
// Constructor: O(n), Query: O(log n), Space: O(n)

// ============================================================
// Design Overview:
// - In the constructor, process all votes sequentially and record
//   the leading candidate at each vote event.
// - For queries, binary search in the times array to find the
//   most recent vote event at or before time t, then return
//   the precomputed leader at that event.
//
// Tie-breaking: "most recent vote wins" is handled by using >=
// when comparing vote counts. If a newly voted person ties with
// the current leader, the newly voted person becomes the leader.
// ============================================================

class TopVotedCandidate {
private:
    vector<int> timesArr;   // Stores the vote timestamps
    vector<int> leaders;     // leaders[i] = who was leading after vote i

public:
    TopVotedCandidate(vector<int>& persons, vector<int>& times) {
        int n = persons.size();
        timesArr = times;
        leaders.resize(n);

        // Vote count for each person
        unordered_map<int, int> voteCount;

        // Track the current leader and their vote count
        int currentLeader = -1;
        int currentLeaderVotes = 0;

        for (int i = 0; i < n; i++) {
            int person = persons[i];
            voteCount[person]++;

            // Use >= for tie-breaking: most recent vote wins ties
            // If this person now has votes >= the current leader's count,
            // they become the new leader (handles both surpassing and tying)
            if (voteCount[person] >= currentLeaderVotes) {
                currentLeader = person;
                currentLeaderVotes = voteCount[person];
            }

            leaders[i] = currentLeader;
        }
    }

    int q(int t) {
        // Find the latest vote event at or before time t
        // upper_bound gives the first element > t
        // Subtracting 1 gives the last element <= t
        auto it = upper_bound(timesArr.begin(), timesArr.end(), t);
        int idx = (it - timesArr.begin()) - 1;
        return leaders[idx];
    }
};

// ============================================================
// Walkthrough with Example:
// persons = [0, 1, 1, 0, 0, 1, 0]
// times   = [0, 5, 10, 15, 20, 25, 30]
// ============================================================
//
// Processing votes:
// i=0: person=0, voteCount={0:1}
//      voteCount[0]=1 >= currentLeaderVotes=0 -> leader=0, votes=1
//      leaders[0] = 0
//
// i=1: person=1, voteCount={0:1, 1:1}
//      voteCount[1]=1 >= currentLeaderVotes=1 -> leader=1, votes=1
//      (Tie: person 1 voted more recently, so person 1 leads)
//      leaders[1] = 1
//
// i=2: person=1, voteCount={0:1, 1:2}
//      voteCount[1]=2 >= currentLeaderVotes=1 -> leader=1, votes=2
//      leaders[2] = 1
//
// i=3: person=0, voteCount={0:2, 1:2}
//      voteCount[0]=2 >= currentLeaderVotes=2 -> leader=0, votes=2
//      (Tie: person 0 voted more recently)
//      leaders[3] = 0
//
// i=4: person=0, voteCount={0:3, 1:2}
//      voteCount[0]=3 >= currentLeaderVotes=2 -> leader=0, votes=3
//      leaders[4] = 0
//
// i=5: person=1, voteCount={0:3, 1:3}
//      voteCount[1]=3 >= currentLeaderVotes=3 -> leader=1, votes=3
//      (Tie: person 1 voted more recently)
//      leaders[5] = 1
//
// i=6: person=0, voteCount={0:4, 1:3}
//      voteCount[0]=4 >= currentLeaderVotes=3 -> leader=0, votes=4
//      leaders[6] = 0
//
// Final: leaders = [0, 1, 1, 0, 0, 1, 0]
//        times   = [0, 5, 10, 15, 20, 25, 30]
//
// Query examples:
// q(3):  upper_bound(3) -> points to 5 (index 1), idx=0. leaders[0]=0.
// q(12): upper_bound(12) -> points to 15 (index 3), idx=2. leaders[2]=1.
// q(25): upper_bound(25) -> points to 30 (index 6), idx=5. leaders[5]=1.
// q(15): upper_bound(15) -> points to 20 (index 4), idx=3. leaders[3]=0.
// q(24): upper_bound(24) -> points to 25 (index 5), idx=4. leaders[4]=0.
// q(8):  upper_bound(8) -> points to 10 (index 2), idx=1. leaders[1]=1.
// ============================================================

// ============================================================
// Alternative: Using std::map for O(log n) query without
// separate binary search
// ============================================================

class TopVotedCandidateMap {
private:
    map<int, int> timeToLeader; // time -> leader at that time

public:
    TopVotedCandidateMap(vector<int>& persons, vector<int>& times) {
        unordered_map<int, int> voteCount;
        int currentLeader = -1;
        int currentLeaderVotes = 0;

        for (int i = 0; i < (int)persons.size(); i++) {
            voteCount[persons[i]]++;
            if (voteCount[persons[i]] >= currentLeaderVotes) {
                currentLeader = persons[i];
                currentLeaderVotes = voteCount[persons[i]];
            }
            timeToLeader[times[i]] = currentLeader;
        }
    }

    int q(int t) {
        // upper_bound returns iterator to first key > t
        // prev gives the last key <= t
        auto it = timeToLeader.upper_bound(t);
        --it;
        return it->second;
    }
};

// ============================================================
// Why >= handles tie-breaking correctly:
// ============================================================
// The problem states: "In the case of a tie, the most recent vote
// (among tied candidates) wins."
//
// When processing vote i for person P:
//   - If P's new count == current leader's count, P should win
//     (P voted more recently).
//   - If P's new count > current leader's count, P should win
//     (P has more votes).
//   - If P's new count < current leader's count, current leader stays.
//
// Using >= covers both the tie case and the surpassing case.
// Using > alone would miss the tie case (wrong answer).
//
// Example: persons = [0, 1], times = [0, 5]
//   After vote 0: counts={0:1}, leader=0
//   After vote 1: counts={0:1, 1:1}
//     With >=: 1 >= 1 is true, leader changes to 1. CORRECT.
//     With >:  1 > 1 is false, leader stays 0. WRONG.
// ============================================================

// ============================================================
// Optimization Note:
// ============================================================
// The vector-based approach (main solution) is preferred over
// the map-based alternative because:
// 1. Vector binary search has better cache locality than map traversal.
// 2. No overhead from balanced BST node allocation.
// 3. Both have O(log n) query time, but the constant factor is
//    smaller for vector + upper_bound.
//
// Space is O(n) in both cases.
// ============================================================
