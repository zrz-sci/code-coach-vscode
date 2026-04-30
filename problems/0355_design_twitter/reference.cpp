/*
 * 【题目本质】
 * 设计社交媒体系统：核心是 "合并多个有序推文列表取 Top K" → K路归并问题
 * 数据结构设计 + 最大堆应用的经典面试题
 *
 * 【解法总览】
 * 解法1: 收集全部排序 — 暴力但简洁
 * 解法2: 最大堆 K路归并 — 最优，只遍历需要的推文数
 */

// ===================== Solution 1: 收集全部推文排序 =====================
// 思路：getNewsFeed 时收集自己和关注者全部推文 → 排序 → 取前10
// 优点：实现简单  缺点：推文多时排序开销大
// 时间：post O(1), feed O(N logN), follow/unfollow O(1)
// 空间：O(N) N=总推文数
class Twitter_v1 {
    int ts;
    unordered_map<int, vector<pair<int,int>>> tweets;   // uid -> [(time, tid)]
    unordered_map<int, unordered_set<int>> follows;     // uid -> {followees}
public:
    Twitter_v1() : ts(0) {}

    void postTweet(int userId, int tweetId) {
        tweets[userId].push_back({ts++, tweetId});
    }

    vector<int> getNewsFeed(int userId) {
        vector<pair<int,int>> all;
        // 自己的推文
        for (auto& t : tweets[userId]) all.push_back(t);
        // 关注者的推文
        for (int fid : follows[userId]) {
            for (auto& t : tweets[fid]) all.push_back(t);
        }
        // 按时间戳降序排序
        sort(all.begin(), all.end(), greater<pair<int,int>>());

        vector<int> res;
        for (int i = 0; i < min(10, (int)all.size()); i++)
            res.push_back(all[i].second);
        return res;
    }

    void follow(int followerId, int followeeId) {
        follows[followerId].insert(followeeId);
    }

    void unfollow(int followerId, int followeeId) {
        follows[followerId].erase(followeeId);
    }
};

// ===================== Solution 2: 最大堆 K路归并（推荐）=====================
// 思路：每个用户推文按时间有序，getNewsFeed = 合并K个有序序列取Top10
// 堆中每个元素：(timestamp, tweetId, userId, indexInTweets)
// 每次弹出最大时间戳的推文，将该用户的前一条推文入堆
// 最多弹出10次 × log(K) = O(10 logK)，K为关注人数
// 时间：post O(1), feed O(10 logK), follow/unfollow O(1)
// 空间：O(K) 堆空间
class Twitter {
    int ts;
    unordered_map<int, vector<pair<int,int>>> tweets;
    unordered_map<int, unordered_set<int>> follows;
public:
    Twitter() : ts(0) {}

    void postTweet(int userId, int tweetId) {
        tweets[userId].push_back({ts++, tweetId});
    }

    vector<int> getNewsFeed(int userId) {
        // 最大堆：(time, tweetId, userId, idx)
        priority_queue<tuple<int,int,int,int>> pq;

        // 辅助函数：将用户的最新推文加入堆
        auto pushLatest = [&](int uid) {
            if (!tweets[uid].empty()) {
                int idx = (int)tweets[uid].size() - 1;
                auto& [t, id] = tweets[uid][idx];
                pq.push({t, id, uid, idx});
            }
        };

        // 加入自己和所有关注者的最新推文
        pushLatest(userId);
        for (int fid : follows[userId]) {
            pushLatest(fid);
        }

        vector<int> res;
        while (!pq.empty() && (int)res.size() < 10) {
            auto [t, id, uid, idx] = pq.top();
            pq.pop();
            res.push_back(id);
            // 将该用户的前一条推文入堆
            if (idx > 0) {
                auto& [pt, pid] = tweets[uid][idx - 1];
                pq.push({pt, pid, uid, idx - 1});
            }
        }
        return res;
    }

    void follow(int followerId, int followeeId) {
        follows[followerId].insert(followeeId);
    }

    void unfollow(int followerId, int followeeId) {
        follows[followerId].erase(followeeId);
    }
};

/*
 * 【解法对比】
 * | 解法 | getNewsFeed | 适用场景 |
 * |------|-------------|---------|
 * | 收集排序 | O(N logN) | 推文总量小 |
 * | K路归并 | O(10 logK) | 生产级系统，推文量大 |
 *
 * 【易错点】
 * 1. 必须包含自己的推文：getNewsFeed 中 userId 自己也是信息源
 * 2. 不能用 tweetId 排序：tweetId 不保证单调递增，必须用全局时间戳
 * 3. follow/unfollow 幂等性：重复 follow 不报错，unfollow 不存在的不崩溃
 * 4. 题目说"用户不会 follow 自己"，但仍需在 getNewsFeed 中加入自己的推文
 *
 * 【面试追问】
 * Q1: 如何扩展到分布式系统？
 *     → Fan-out on write（写时分发）vs Fan-out on read（读时合并）的权衡
 *     → 大 V 用 Pull，普通用户用 Push 的混合架构
 * Q2: 如何支持推文删除？
 *     → 推文标记 deleted 标志，getNewsFeed 跳过已删除推文
 *     → 或维护 deleted set，合并时过滤
 * Q3: 如何优化 "取消关注后立即刷新 feed" 的体验？
 *     → Pull 模式天然支持：getNewsFeed 实时计算
 *     → Push 模式需异步清理粉丝 feed 缓存中的相关推文
 */
