# 3433. 统计用户被提及情况

## 核心思路

**排序 + 模拟**。按时间戳排序所有事件（同时间戳 OFFLINE 优先于 MESSAGE），维护每个用户的在线/离线状态，按规则逐个处理 MESSAGE 中的 ALL/HERE/id 提及。

## 思维链

1. **读完题第一反应**：这是一道模拟题，没有特别的算法技巧，但规则细节很多，需要仔细处理。关键要素有三个：事件排序规则、离线状态管理、三种提及方式的区别。

2. **事件排序是第一步**：题目说"状态变更在消息之前处理"。这意味着如果时间戳 t 同时有 OFFLINE 和 MESSAGE 事件，必须先处理 OFFLINE（用户先离线），然后再处理 MESSAGE（此时该用户已经离线，HERE 不会提及他）。

3. **排序规则**：
   - 首先按时间戳升序排列
   - 时间戳相同时，OFFLINE 排在 MESSAGE 前面
   
   ```
   事件列表 (排序前):
   ["MESSAGE","10","id1 id0"], ["OFFLINE","10","0"]
   
   事件列表 (排序后):
   ["OFFLINE","10","0"], ["MESSAGE","10","id1 id0"]
   ```

4. **离线状态管理**：用 `offlineUntil[uid]` 记录用户 uid 的上线时间点。如果当前时间 `t >= offlineUntil[uid]`，该用户在线。OFFLINE 事件设置 `offlineUntil[uid] = timestamp + 60`。

5. **三种提及方式**：
   - **ALL**：所有用户 +1（不管在线离线）
   - **HERE**：只有在线用户 +1（需要检查每个用户的 offlineUntil）
   - **id0 id1 ...**：空格分隔的用户 id，逐个解析并 +1（不管在线离线，可以重复）

6. **字符串解析**：id 格式是 `"id<number>"`，需要提取 `<number>` 部分（跳过前两个字符 "id"）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 排序 + 模拟 | 按规则排序，逐事件处理 | O(E log E + E * U) | O(U) | 唯一解法 ⭐ |

> E = 事件数（最多 100），U = 用户数（最多 100）

## 关键提示

1. **排序是重点中的重点**：同一时间戳的 OFFLINE 必须在 MESSAGE 之前处理。排序比较器中，时间戳相同时 OFFLINE < MESSAGE。这一条规则理解错误就会导致 HERE 的结果错误。

2. **离线持续 60 个时间单位**：`offlineUntil[uid] = timestamp + 60`。注意：用户在 `timestamp + 60` 时刻就已经在线了（不是 timestamp + 61）。因此判断在线的条件是 `t >= offlineUntil[uid]`（不是 `t > offlineUntil[uid]`）。

3. **ALL vs HERE vs id 的区别**：
   ```
   ALL  → 所有用户 +1（包括离线用户）
   HERE → 仅在线用户 +1（离线用户不计）
   id   → 指定用户 +1（不管在线离线，可重复提及）
   ```

4. **id 可以重复提及**：`"id0 id0 id0"` 意味着 user 0 被提及 3 次，不是 1 次。每次提及独立计数。

5. **手动模拟验证**（示例 1）：
   ```
   初始: 所有用户在线, mentions = [0, 0]
   
   t=10, MESSAGE "id1 id0":
     → id1 +1, id0 +1
     → mentions = [1, 1]
   
   t=11, OFFLINE "0":
     → offlineUntil[0] = 11 + 60 = 71
   
   t=71, MESSAGE "HERE":
     → t=71, offlineUntil[0]=71, 71 >= 71 → id0 在线!
     → id0 +1, id1 +1
     → mentions = [2, 2]
   ```

6. **排序规则的示例验证**（示例 3）：
   ```
   初始: 所有用户在线, mentions = [0, 0]
   
   t=10, OFFLINE "0":
     → offlineUntil[0] = 10 + 60 = 70
   
   t=12, MESSAGE "HERE":
     → t=12, offlineUntil[0]=70, 12 < 70 → id0 离线!
     → 只有 id1 在线 → id1 +1
     → mentions = [0, 1]
   ```

## 解法详解

### 排序 + 模拟 — O(E log E + E * U) / O(U) ⭐

**第一步：排序事件**

排序规则：
1. 时间戳升序
2. 时间戳相同时，OFFLINE 在 MESSAGE 之前

```
为什么 OFFLINE 要优先? 题目原文:
"if a user goes offline or comes back online, their status change
is processed before handling any message event that occurs at the
same timestamp"
```

**第二步：逐事件处理**

```
对于 OFFLINE 事件:
  → offlineUntil[uid] = timestamp + 60

对于 MESSAGE 事件:
  → 根据 mentions_string 的内容分三种情况:
    "ALL"       → 所有用户 mentions[i]++
    "HERE"      → 遍历用户, 在线的 mentions[i]++
    "id0 id1.." → 解析每个 id, mentions[uid]++
```

**第三步：字符串解析细节**

`"id0 id1 id2"` → 用 `istringstream` 按空格分割 → 每个 token 去掉前两个字符 "id" → 得到用户编号。

```cpp
class Solution {
public:
    vector<int> countMentions(int numberOfUsers, vector<vector<string>>& events) {
        // 第一步: 排序事件
        sort(events.begin(), events.end(), 
            [](const vector<string>& a, const vector<string>& b) {
                int ta = stoi(a[1]), tb = stoi(b[1]);
                if (ta != tb) return ta < tb;
                // 同时间戳: OFFLINE 优先于 MESSAGE
                return a[0] == "OFFLINE" && b[0] == "MESSAGE";
            }
        );
        
        // 第二步: 模拟处理
        vector<int> mentions(numberOfUsers, 0);
        vector<int> offlineUntil(numberOfUsers, 0);
        
        for (auto& e : events) {
            int t = stoi(e[1]);
            
            if (e[0] == "OFFLINE") {
                int uid = stoi(e[2]);
                offlineUntil[uid] = t + 60;
            } else {
                // MESSAGE
                string& msg = e[2];
                if (msg == "ALL") {
                    for (int i = 0; i < numberOfUsers; i++) {
                        mentions[i]++;
                    }
                } else if (msg == "HERE") {
                    for (int i = 0; i < numberOfUsers; i++) {
                        if (offlineUntil[i] <= t) {
                            mentions[i]++;
                        }
                    }
                } else {
                    // 解析 "id0 id1 id2 ..."
                    istringstream iss(msg);
                    string token;
                    while (iss >> token) {
                        int uid = stoi(token.substr(2));
                        mentions[uid]++;
                    }
                }
            }
        }
        return mentions;
    }
};
```

## 易错点

1. **排序规则写错 —— 同时间戳时 OFFLINE 没有优先**：
   ```
   ✗ sort(events, [](a, b) { return stoi(a[1]) < stoi(b[1]); });
   ✓ 还需要: 时间戳相同时 OFFLINE 排在 MESSAGE 前面
   ```
   这会导致 HERE 提及时用户状态不正确（应该先离线再处理消息）。

2. **在线判断的边界条件 —— >= vs >**：
   ```
   ✗ if (offlineUntil[i] < t) mentions[i]++;  // 在 t=60+timestamp 时仍认为离线
   ✓ if (offlineUntil[i] <= t) mentions[i]++;  // 在 t=60+timestamp 时已经在线
   ```
   题目说"用户在 timestamp + 60 时自动上线"，所以 `offlineUntil <= t` 才表示在线。

3. **ALL 也包含离线用户**：
   ```
   ✗ ALL 时只给在线用户 +1
   ✓ ALL 时给所有用户 +1（题目明确: "mentions all users"）
   ```

4. **id 提及也包含离线用户**：
   ```
   ✗ 提及 "id0" 时先检查 id0 是否在线
   ✓ 直接 +1（题目明确: "This can mention even the offline users"）
   ```

5. **id 可以重复 —— 不能去重**：
   ```
   ✗ 用 set 去重 id 后再计数
   ✓ "id0 id0" 就是提及 2 次, 每次独立计数
   ```

6. **字符串解析出错**：
   ```
   ✗ int uid = stoi(token);           // token = "id0", stoi 会失败
   ✓ int uid = stoi(token.substr(2)); // 去掉 "id" 前缀
   ```

## 面试追问

**Q1: 排序的时间复杂度是多少？**
O(E log E)，其中 E 是事件数。本题 E 最大 100，排序几乎是常数时间。瓶颈在模拟阶段 O(E * U)，其中 U 是用户数（最大 100），HERE 消息需要遍历所有用户检查在线状态。

**Q2: 如果用户数和事件数都很大（10^5 级别），怎么优化 HERE 消息的处理？**
可以用一个计数器 `onlineCount` 记录当前在线用户数。OFFLINE 时 onlineCount--，到期时 onlineCount++。但 HERE 仍然需要知道具体哪些用户在线才能给他们 +1。可以用差分数组/事件驱动的方式：记录每个用户有多少次 HERE 消息时在线，最后统一累加。

**Q3: 如果 OFFLINE 事件可以嵌套（用户还没上线又被再次 OFFLINE），怎么处理？**
题目保证 OFFLINE 引用的用户在事件发生时处于在线状态，所以不会嵌套。但如果允许嵌套，可以用 `offlineUntil[uid] = max(offlineUntil[uid], t + 60)` 取最远的上线时间。

**Q4: 如果要求实时处理（事件流式到达，不能排序），怎么做？**
需要维护一个优先队列管理即将上线的用户，每次收到 MESSAGE 事件时先处理所有到期的上线事件。复杂度类似，但实现更复杂。

## 相关题型

- **253. 会议室 II** — 同样是事件排序 + 模拟的问题，按时间处理进出事件。

- **1834. 单线程 CPU** — 事件排序 + 模拟，按时间戳处理任务。

- **2349. 设计数字容器系统** — 需要维护在线/离线状态的模拟题。
