# 353. Design Snake Game - 贪吃蛇

## 核心思路
用 **deque（双端队列）** 维护蛇身坐标，用 **set** 快速判断蛇身碰撞。每次移动时计算新头部位置，判断越界和自碰，若吃到食物则不删尾部（蛇身增长），否则删除尾部。

## 思维链
1. 蛇身是一个有序序列 → 用 deque 存储（头在前，尾在后）
2. 移动 = 在头部添加新位置 + 删除尾部（除非吃到食物）
3. 碰撞检测需要 O(1) 查找 → 用 set 存储蛇身坐标
4. 关键：先删尾部再判碰撞（蛇尾离开后的位置可以被头部占据）

## 解法概览表⭐

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| Deque + Set | O(1)/move | O(N) | 标准解法，高效碰撞检测 |
| Deque + 线性搜索 | O(N)/move | O(N) | 简化版，无需额外 set |

## 关键提示
1. **方向映射**：用 map 将 "U/D/L/R" 映射为 (dr, dc) 偏移量
2. **碰撞检测顺序**：必须先移除尾部再检查新头部是否与蛇身冲突
3. **食物序列**：用索引 foodIdx 追踪当前应出现的食物
4. **坐标编码**：可以用 `row * width + col` 将 2D 坐标编码为 1D 整数，方便存入 set

## 解法详解

### 解法一：Deque + HashSet（推荐）

**思路**：deque 存蛇身坐标序列，set 存蛇身坐标集合用于 O(1) 碰撞检测。

```cpp
class SnakeGame {
    int w, h, foodIdx, score;
    deque<pair<int,int>> snake;
    set<pair<int,int>> body;
    vector<vector<int>> food;
public:
    SnakeGame(int width, int height, vector<vector<int>>& food)
        : w(width), h(height), foodIdx(0), score(0), food(food) {
        snake.push_back({0, 0});
        body.insert({0, 0});
    }
    
    int move(string direction) {
        auto [r, c] = snake.front();
        if (direction == "U") r--;
        else if (direction == "D") r++;
        else if (direction == "L") c--;
        else c++;
        
        // 越界检查
        if (r < 0 || r >= h || c < 0 || c >= w) return -1;
        
        // 吃到食物则不删尾
        if (foodIdx < food.size() && r == food[foodIdx][0] && c == food[foodIdx][1]) {
            foodIdx++;
            score++;
        } else {
            // 先删尾部
            body.erase(snake.back());
            snake.pop_back();
        }
        
        // 检查自碰
        if (body.count({r, c})) return -1;
        
        snake.push_front({r, c});
        body.insert({r, c});
        return score;
    }
};
```

**复杂度**：每次 move O(log N)（set 操作），空间 O(N)

### 解法二：Deque + 1D编码 + unordered_set

**思路**：将坐标编码为整数，使用 unordered_set 实现 O(1) 查找。

```cpp
class SnakeGame {
    int w, h, foodIdx, score;
    deque<int> snake;
    unordered_set<int> body;
    vector<vector<int>> food;
    
    int encode(int r, int c) { return r * w + c; }
public:
    SnakeGame(int width, int height, vector<vector<int>>& food)
        : w(width), h(height), foodIdx(0), score(0), food(food) {
        snake.push_back(0);
        body.insert(0);
    }
    
    int move(string direction) {
        int r = snake.front() / w, c = snake.front() % w;
        if (direction == "U") r--;
        else if (direction == "D") r++;
        else if (direction == "L") c--;
        else c++;
        
        if (r < 0 || r >= h || c < 0 || c >= w) return -1;
        
        if (foodIdx < food.size() && r == food[foodIdx][0] && c == food[foodIdx][1]) {
            foodIdx++;
            score++;
        } else {
            body.erase(snake.back());
            snake.pop_back();
        }
        
        int code = encode(r, c);
        if (body.count(code)) return -1;
        
        snake.push_front(code);
        body.insert(code);
        return score;
    }
};
```

**复杂度**：每次 move O(1) 均摊，空间 O(N)

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 先检查自碰再删尾 | 先删尾再检查自碰 | 蛇尾移开后头部可以进入原尾部位置 |
| `width` 当行数 | `height` 是行数, `width` 是列数 | 题目构造参数顺序是 (width, height, food) |
| 不处理 k=0 边界 | k=0 时直接返回 s | 距离0意味着无约束 |
| 用 `body.size()` 当分数 | 用独立变量 `score` 计分 | body 初始包含起点，size 从1开始 |

## 面试追问

**Q1: 如果蛇身很长（比如 10^4），如何优化碰撞检测？**

A: 使用 unordered_set 存储蛇身坐标的 1D 编码（`row * width + col`），实现 O(1) 碰撞检测。相比 set 的 O(log N) 更快。坐标编码避免了用 pair 做 key 的哈希问题。

**Q2: 如果要支持多条蛇同时游戏，数据结构如何扩展？**

A: 每条蛇独立维护一个 deque 和 body set。额外需要一个全局 set 存储所有蛇身占据的位置，碰撞检测时检查是否撞到任何蛇（包括自己）。食物需要全局管理，任何蛇吃到后都消失。

**Q3: 如何支持"穿越边界"（从一侧出去另一侧进入）？**

A: 不再判断越界返回 -1，而是对新坐标取模：`r = (r + h) % h; c = (c + w) % w;`。其他逻辑不变。

## 相关题型
- [362. Design Hit Counter](../0362_design_hit_counter/) - 设计类 + 队列
- [346. Moving Average from Data Stream](../0346_moving_average_from_data_stream/) - 队列维护窗口
- [359. Logger Rate Limiter](../0359_logger_rate_limiter/) - 设计类 + 时间窗口
