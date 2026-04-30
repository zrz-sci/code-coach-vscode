/*
 * LeetCode 3433: 统计用户被提及情况
 * Count Mentions Per User
 *
 * 【题目本质】
 * 模拟题。按时间戳排序事件(同时间 OFFLINE 优先), 维护在线状态,
 * 根据 ALL/HERE/id 三种提及方式逐个计数。
 * 关键在于规则细节: 排序优先级、离线时长、在线判断边界。
 *
 * 【解法总览】
 * 解法1: 排序 + 模拟 (istringstream) — O(E log E + E*U) / O(U) — 面试首选 ⭐
 * 解法2: 排序 + 模拟 (手动解析)     — O(E log E + E*U) / O(U) — 不用 sstream
 */


// ============================================================
// 解法1: 排序 + 模拟 (istringstream 解析 id) — 面试首选 ⭐
// 时间: O(E log E + E * U)  空间: O(U)
// E = 事件数(最多100), U = 用户数(最多100)
//
// 【思路】
// Step 1: 排序事件
//   - 时间戳升序
//   - 同时间戳: OFFLINE 优先于 MESSAGE
//     (题目要求状态变更在消息之前处理)
//
// Step 2: 逐事件处理
//   - OFFLINE: 设置 offlineUntil[uid] = t + 60
//   - MESSAGE + "ALL": 所有用户 +1 (包括离线)
//   - MESSAGE + "HERE": 仅在线用户 +1
//   - MESSAGE + "id0 id1 ...": 解析 id 逐个 +1 (包括离线, 可重复)
//
// 【排序规则详解】
//   同一时间戳的事件:
//     ["OFFLINE","10","0"]  →  先处理 (用户先离线)
//     ["MESSAGE","10","HERE"] →  后处理 (此时用户已离线, HERE 不提及他)
//
//   如果顺序反了:
//     先处理 MESSAGE "HERE" → 用户还在线, 会被提及 (错误!)
//     后处理 OFFLINE → 用户离线了, 但提及已经发生
//
// 【在线判断: offlineUntil[i] <= t】
//   用户在 timestamp 时离线, 在 timestamp+60 时上线
//   即 offlineUntil = timestamp + 60
//   当 t >= offlineUntil 时, 用户已经在线
//   所以判断条件是 offlineUntil[i] <= t (不是 < t)
//
// 【三种提及方式对比】
//   ALL:  所有用户+1 (离线也算)  → 不检查 offlineUntil
//   HERE: 仅在线用户+1           → 必须检查 offlineUntil
//   id:   指定用户+1 (离线也算)  → 不检查 offlineUntil
//
// 示例 1: numberOfUsers=2, events 排序后:
//   t=10, MESSAGE "id1 id0" → mentions=[1,1]
//   t=11, OFFLINE "0"       → offlineUntil[0]=71
//   t=71, MESSAGE "HERE"    → 71>=71, id0在线! → mentions=[2,2]
//
// 示例 3: numberOfUsers=2, events 排序后:
//   t=10, OFFLINE "0"       → offlineUntil[0]=70
//   t=12, MESSAGE "HERE"    → 12<70, id0离线 → 只有id1 → mentions=[0,1]
// ============================================================
class Solution1 {
public:
    vector<int> countMentions(int numberOfUsers, vector<vector<string>>& events) {
        // Step 1: 排序事件
        // 按时间戳升序; 同时间戳 OFFLINE 排在 MESSAGE 前面
        sort(events.begin(), events.end(),
            [](const vector<string>& a, const vector<string>& b) {
                int ta = stoi(a[1]), tb = stoi(b[1]);
                if (ta != tb) return ta < tb;
                // OFFLINE 优先于 MESSAGE
                // "OFFLINE" < "MESSAGE" 字典序不成立,
                // 所以需要显式判断
                return a[0] == "OFFLINE" && b[0] == "MESSAGE";
            }
        );

        // Step 2: 模拟
        vector<int> mentions(numberOfUsers, 0);     // 每个用户的提及次数
        vector<int> offlineUntil(numberOfUsers, 0);  // 用户上线时间点

        for (auto& e : events) {
            int t = stoi(e[1]);

            if (e[0] == "OFFLINE") {
                // 用户离线 60 个时间单位
                int uid = stoi(e[2]);
                offlineUntil[uid] = t + 60;
            } else {
                // MESSAGE 事件 — 三种情况
                const string& msg = e[2];

                if (msg == "ALL") {
                    // ALL: 所有用户 +1 (不管在线离线)
                    for (int i = 0; i < numberOfUsers; i++) {
                        mentions[i]++;
                    }
                } else if (msg == "HERE") {
                    // HERE: 仅在线用户 +1
                    for (int i = 0; i < numberOfUsers; i++) {
                        // offlineUntil[i] <= t 表示用户 i 已经在线
                        if (offlineUntil[i] <= t) {
                            mentions[i]++;
                        }
                    }
                } else {
                    // id 列表: "id0 id1 id2 ..."
                    // 用 istringstream 按空格分割
                    istringstream iss(msg);
                    string token;
                    while (iss >> token) {
                        // token = "id0", "id1", etc.
                        // 去掉前缀 "id", 取出数字部分
                        int uid = stoi(token.substr(2));
                        mentions[uid]++;
                    }
                }
            }
        }
        return mentions;
    }
};

// ============================================================
// 解法2: 排序 + 模拟 (手动解析, 不用 istringstream)
// 时间: O(E log E + E * U)  空间: O(U)
//
// 【思路】
// 与解法1 逻辑完全相同, 但字符串解析部分不依赖 <sstream>,
// 而是手动遍历字符串, 用 find 定位空格来分割 token。
// 有些面试环境可能不方便用 istringstream, 这个版本更通用。
//
// 手动解析示例:
//   msg = "id0 id1 id23"
//   start=0: 找到空格在 pos=3, token="id0" → uid=0
//   start=4: 找到空格在 pos=7, token="id1" → uid=1
//   start=8: 找不到空格(npos), token="id23" → uid=23
// ============================================================
class Solution2 {
public:
    vector<int> countMentions(int numberOfUsers, vector<vector<string>>& events) {
        // 排序 (同解法1)
        sort(events.begin(), events.end(),
            [](const vector<string>& a, const vector<string>& b) {
                int ta = stoi(a[1]), tb = stoi(b[1]);
                if (ta != tb) return ta < tb;
                return a[0] == "OFFLINE" && b[0] == "MESSAGE";
            }
        );

        vector<int> mentions(numberOfUsers, 0);
        vector<int> offlineUntil(numberOfUsers, 0);

        for (auto& e : events) {
            int t = stoi(e[1]);

            if (e[0] == "OFFLINE") {
                int uid = stoi(e[2]);
                offlineUntil[uid] = t + 60;
            } else {
                const string& msg = e[2];

                if (msg == "ALL") {
                    for (int i = 0; i < numberOfUsers; i++) mentions[i]++;
                } else if (msg == "HERE") {
                    for (int i = 0; i < numberOfUsers; i++) {
                        if (offlineUntil[i] <= t) mentions[i]++;
                    }
                } else {
                    // 手动解析 id 列表 (不用 istringstream)
                    size_t start = 0;
                    while (start < msg.size()) {
                        size_t spacePos = msg.find(' ', start);
                        string token;
                        if (spacePos == string::npos) {
                            token = msg.substr(start);
                            start = msg.size(); // 结束
                        } else {
                            token = msg.substr(start, spacePos - start);
                            start = spacePos + 1;
                        }
                        // token = "id<number>", 取 substr(2) 得到数字
                        int uid = stoi(token.substr(2));
                        mentions[uid]++;
                    }
                }
            }
        }
        return mentions;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 解析方式       | 依赖    | 时间            | 空间 |
// |---------------|---------------|---------|-----------------|------|
// | istringstream | C++ stream    | <sstream>| O(E log E+E*U) | O(U) |
// | 手动解析      | find+substr   | 无      | O(E log E+E*U) | O(U) |
//
// 两者性能一致, 选择取决于面试环境和个人偏好。
//
// ============================================================
// 【易错点】
//
// 1. 排序规则写错:
//    ✗ 只按时间戳排序, 同时间戳不区分 OFFLINE/MESSAGE
//    ✓ 同时间戳 OFFLINE 必须排在 MESSAGE 前面
//    反例: t=10 同时有 OFFLINE 和 MESSAGE "HERE"
//          如果先处理 MESSAGE, 用户还在线会被 HERE 提及(错误)
//
// 2. 在线判断用 < 而不是 <=:
//    ✗ if (offlineUntil[i] < t)  // t=70, offlineUntil=70 → 认为离线
//    ✓ if (offlineUntil[i] <= t) // t=70, offlineUntil=70 → 认为在线
//    用户在 timestamp+60 时"自动上线", 所以 = 时已经在线
//
// 3. ALL 也包含离线用户:
//    ✗ ALL 时检查在线状态, 只给在线用户+1
//    ✓ ALL → 全部用户+1 (题目: "mentions all users")
//
// 4. id 提及也包含离线用户:
//    ✗ 提及 "id0" 前检查 id0 是否在线
//    ✓ 直接+1 (题目: "This can mention even the offline users")
//
// 5. id 可重复:
//    ✗ 用 set 对 id 去重后再计数
//    ✓ "id0 id0" = 提及 2 次, 每次独立计数
//
// 6. stoi 解析错误:
//    ✗ int uid = stoi(token);       // token="id0", stoi 遇到 "id" 会报错
//    ✓ int uid = stoi(token.substr(2)); // 去掉 "id" 前缀再转数字
//
// ============================================================
// 【面试追问】
//
// Q1: "排序比较器的写法有什么注意事项?"
//     比较器必须满足严格弱序(strict weak ordering):
//     - 如果 a==b, 返回 false (不能 a<b 且 b<a)
//     - 传递性: a<b, b<c → a<c
//     我们的比较器: 时间不等按时间比; 时间相等且类型不同按规则;
//     时间相等且类型相同返回 false。满足严格弱序。
//
// Q2: "如果用户数很大(10^5), HERE 怎么优化?"
//     维护在线用户集合(set/bitset),
//     HERE 时只遍历在线用户而不是全部用户。
//     或者用差分数组: 记录每个用户在哪些时间段在线,
//     最后统一计算 HERE 提及次数。
//
// Q3: "如果事件流式到达(不能提前排序), 怎么做?"
//     用优先队列管理即将上线的事件,
//     每次处理新事件前, 先出队所有到期的上线事件。
//
// ============================================================
