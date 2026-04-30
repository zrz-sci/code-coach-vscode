# 355. Design Twitter - 设计推特

## 核心思路
用 **HashMap + 链表/数组** 存储每个用户的推文列表（带时间戳），用 **HashSet** 维护关注关系。获取 NewsFeed 时，合并自己和所有关注者的推文列表，取最近 10 条 → 经典的 **K路归并** 问题，用 **最大堆** 高效解决。

## 思维链
1. 用户数据：每个 userId 对应一组推文（按时间排序）→ `map<int, vector<pair<int,int>>>` (timestamp, tweetId)
2. 关注关系：每个 userId 对应一组关注者 → `map<int, set<int>>`
3. 获取 Feed：收集自己 + 关注者的所有推文 → 取 top 10 最近的
4. 优化 Feed：不需要收集所有推文，用最大堆做 K 路归并，每个用户只需从最新推文开始

## 解法概览表⭐

| 解法 | postTweet | getNewsFeed | follow/unfollow | 特点 |
|------|----------|-------------|-----------------|------|
| 全收集排序 | O(1) | O(N logN) | O(1) | 简单暴力 |
| 最大堆 K路归并 | O(1) | O(K log K) | O(1) | 最优，K=关注人数 |

## 关键提示
1. **全局时间戳**：需要一个递增计数器来标记推文发送顺序
2. **自己的推文**：getNewsFeed 必须包含用户自己的推文
3. **取消关注自己**：不应该允许用户 unfollow 自己（但题目保证不会 follow 自己）
4. **堆的大小**：K 路归并中 K = 关注人数 + 1（包含自己），每路只需遍历最多 10 条

## 解法详解

### 解法一：收集所有推文排序

**思路**：getNewsFeed 时收集自己和所有关注者的推文到一个数组中，按时间戳降序排序，取前 10 条。

```cpp
class Twitter {
    int timestamp;
    unordered_map<int, vector<pair<int,int>>> tweets;  // userId -> [(time, tweetId)]
    unordered_map<int, unordered_set<int>> follows;    // userId -> {followeeIds}
public:
    Twitter() : timestamp(0) {}
    
    void postTweet(int userId, int tweetId) {
        tweets[userId].push_back({timestamp++, tweetId});
    }
    
    vector<int> getNewsFeed(int userId) {
        vector<pair<int,int>> all;
        // 收集自己的推文
        for (auto& t : tweets[userId]) all.push_back(t);
        // 收集关注者的推文
        for (int fid : follows[userId]) {
            for (auto& t : tweets[fid]) all.push_back(t);
        }
        sort(all.begin(), all.end(), greater<>());
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
```

**复杂度**：getNewsFeed O(N logN) N=所有相关推文数

### 解法二：最大堆 K路归并（推荐）

**思路**：每个用户的推文按时间有序。getNewsFeed 等价于合并 K 个有序列表取 Top10。用最大堆，每次弹出最新推文，再将该用户的下一条推文入堆。

```cpp
class Twitter {
    int timestamp;
    unordered_map<int, vector<pair<int,int>>> tweets;
    unordered_map<int, unordered_set<int>> follows;
public:
    Twitter() : timestamp(0) {}
    
    void postTweet(int userId, int tweetId) {
        tweets[userId].push_back({timestamp++, tweetId});
    }
    
    vector<int> getNewsFeed(int userId) {
        // 最大堆：(time, tweetId, userId, index_in_tweets)
        priority_queue<tuple<int,int,int,int>> pq;
        
        // 将自己和关注者的最新推文加入堆
        auto addUser = [&](int uid) {
            if (!tweets[uid].empty()) {
                int idx = tweets[uid].size() - 1;
                auto [t, id] = tweets[uid][idx];
                pq.push({t, id, uid, idx});
            }
        };
        
        addUser(userId);
        for (int fid : follows[userId]) addUser(fid);
        
        vector<int> res;
        while (!pq.empty() && res.size() < 10) {
            auto [t, id, uid, idx] = pq.top(); pq.pop();
            res.push_back(id);
            if (idx > 0) {
                auto [pt, pid] = tweets[uid][idx - 1];
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
```

**复杂度**：getNewsFeed O(10 * log K)，K=关注人数

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| getNewsFeed 不包含自己的推文 | 必须包含自己发的推文 | 题目明确要求 |
| 用 tweetId 排序时间顺序 | 用独立的全局时间戳排序 | tweetId 不保证递增 |
| follow 时不去重 | 用 set 存关注关系 | 可能重复 follow |
| unfollow 不存在的关注关系崩溃 | set.erase 安全删除 | erase 不存在的元素不报错 |

## 面试追问

**Q1: 如果用户推文量很大（数百万条），getNewsFeed 如何优化？**

A: 1) K路归并只需取 Top10，每路最多遍历 10 条 → O(10 * log K)。2) 可以对每个用户的推文用 "Fan-out on write" 策略：发推时直接写入所有粉丝的 Feed 缓存，getNewsFeed 直接读缓存 O(1)。3) 对大 V 用 "Fan-out on read"，普通用户用 "Fan-out on write"（混合策略）。

**Q2: 如何扩展支持 "点赞" 和 "转推" 功能？**

A: 推文结构扩展为 `struct Tweet { int id, authorId, timestamp; int likes; int retweetOf; }`。点赞直接更新 likes 计数。转推创建新 Tweet 指向原推。getNewsFeed 合并时需要去重（同一原推被多人转推）。

**Q3: 如何支持实时推送（Push 模式）而不是拉取（Pull 模式）？**

A: 维护每个用户的 Feed 列表。postTweet 时异步地将推文追加到所有粉丝的 Feed 中（Fan-out on write）。用消息队列解耦写入。对粉丝过多的用户（>10000）改用 Pull 模式避免写放大。

## 相关题型
- [23. Merge k Sorted Lists](../0023_merge_k_sorted_lists/) - K路归并经典题
- [295. Find Median from Data Stream](../0295_find_median_from_data_stream/) - 堆的设计题
- [380. Insert Delete GetRandom O(1)](../0380_insert_delete_getrandom_o1/) - 设计数据结构
