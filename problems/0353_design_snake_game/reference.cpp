/*
 * 【题目本质】
 * 模拟贪吃蛇游戏：用数据结构高效维护蛇身状态，支持 O(1) 移动、碰撞检测和吃食物
 *
 * 【解法总览】
 * 解法1: Deque + Set — 标准做法，set 支持 O(logN) 碰撞检测
 * 解法2: Deque + unordered_set + 1D编码 — 优化至 O(1) 碰撞检测
 */

// ===================== Solution 1: Deque + Set =====================
// 思路：deque 维护蛇身有序序列，set 维护蛇身坐标集合
// 移动时：计算新头 → 判越界 → 判吃食物（决定是否删尾）→ 先删尾 → 判自碰 → 加头
// 时间：O(logN) per move   空间：O(N)
class SnakeGame_v1 {
    int w, h, foodIdx, score;
    deque<pair<int,int>> snake;
    set<pair<int,int>> body;
    vector<vector<int>> food;
public:
    SnakeGame_v1(int width, int height, vector<vector<int>>& food)
        : w(width), h(height), foodIdx(0), score(0), food(food) {
        snake.push_back({0, 0});
        body.insert({0, 0});
    }

    int move(string direction) {
        auto [r, c] = snake.front();
        if (direction == "U") r--;
        else if (direction == "D") r++;
        else if (direction == "L") c--;
        else c++;  // "R"

        // 1. 越界检查
        if (r < 0 || r >= h || c < 0 || c >= w) return -1;

        // 2. 判断是否吃到食物
        if (foodIdx < (int)food.size() && r == food[foodIdx][0] && c == food[foodIdx][1]) {
            foodIdx++;
            score++;
        } else {
            // 不吃食物：先删除尾部（释放空间）
            body.erase(snake.back());
            snake.pop_back();
        }

        // 3. 检查自碰（必须在删尾之后）
        if (body.count({r, c})) return -1;

        // 4. 添加新头部
        snake.push_front({r, c});
        body.insert({r, c});
        return score;
    }
};

// ===================== Solution 2: Deque + unordered_set + 1D编码 =====================
// 思路：将 (row, col) 编码为 row * width + col 存入 unordered_set
// 避免 pair 哈希问题，实现真正 O(1) 碰撞检测
// 时间：O(1) per move 均摊   空间：O(N)
class SnakeGame {
    int w, h, foodIdx, score;
    deque<int> snake;            // 存 1D 编码
    unordered_set<int> body;    // O(1) 查找
    vector<vector<int>> food;

    int encode(int r, int c) { return r * w + c; }
public:
    SnakeGame(int width, int height, vector<vector<int>>& food)
        : w(width), h(height), foodIdx(0), score(0), food(food) {
        snake.push_back(0);     // 起点 (0,0) 编码为 0
        body.insert(0);
    }

    int move(string direction) {
        int head = snake.front();
        int r = head / w, c = head % w;

        if (direction == "U") r--;
        else if (direction == "D") r++;
        else if (direction == "L") c--;
        else c++;

        // 越界
        if (r < 0 || r >= h || c < 0 || c >= w) return -1;

        // 吃食物判定
        if (foodIdx < (int)food.size() && r == food[foodIdx][0] && c == food[foodIdx][1]) {
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

/*
 * 【解法对比】
 * | 解法 | move 时间 | 空间 | 优势 |
 * |------|----------|------|------|
 * | Deque + Set | O(logN) | O(N) | 简洁，pair 直接存 |
 * | Deque + unordered_set | O(1)均摊 | O(N) | 更快碰撞检测 |
 *
 * 【易错点】
 * 1. 先删尾再判自碰：蛇尾移走后头可以进入该位置（关键顺序）
 * 2. width vs height：构造函数参数是 (width, height)，width 是列数
 * 3. food 越界：foodIdx 可能超出 food 数组范围，需要判断
 * 4. 起始分数为 0，不是 body.size()-1
 *
 * 【面试追问】
 * Q1: 高频调用下如何保证性能？
 *     → unordered_set + 1D 编码保证 O(1)，deque 两端操作均 O(1)
 * Q2: 如何支持多人贪吃蛇？
 *     → 每条蛇独立 deque+set，全局 occupied set 做跨蛇碰撞
 * Q3: 如何增加"穿墙"功能？
 *     → 取模替代越界判断：r = (r + h) % h, c = (c + w) % w
 */
