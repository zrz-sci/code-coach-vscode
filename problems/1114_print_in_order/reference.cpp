/*
 * LeetCode 1114: 按序打印 (Print in Order)
 *
 * 【题目本质】
 * 三个线程共享 Foo 实例，分别调用 first/second/third。
 * 线程调度顺序不确定，需要用同步原语确保输出顺序始终是
 * "firstsecondthird"。经典的线程顺序同步问题。
 *
 * 【解法总览】
 * 解法1: promise/future       — O(1) / O(1) — 一次性同步原语 ⭐代码最简
 * 解法2: mutex + cond_var     — O(1) / O(1) — OS标准阻塞同步 ⭐教科书做法
 * 解法3: atomic + 自旋等待    — O(1) / O(1) — 忙等方案
 */


// ============================================================
// 解法1: promise/future — 一次性事件同步
// 时间: O(1)  空间: O(1)
//
// 【思路】
// promise/future 是 C++ 的一次性同步机制:
//   - promise 是"写端": set_value() 发出信号
//   - future 是"读端": wait() 阻塞等待信号
//   - set/wait 各只能调用一次，完美匹配本题（每个函数只调用一次）
//
// 需要两个同步点:
//   p1: first → second 的同步（first 完成后 second 才能执行）
//   p2: second → third 的同步（second 完成后 third 才能执行）
//
// 执行时序:
//   Thread A: [printFirst] → p1.set_value()
//   Thread B: p1.wait() → [printSecond] → p2.set_value()
//   Thread C: p2.wait() → [printThird]
//
// 为什么推荐?
//   1. 代码极简洁（6行核心逻辑）
//   2. 阻塞等待，不消耗 CPU（内部基于 futex/条件变量实现）
//   3. 类型安全，不会忘记加锁
// ============================================================
class Solution1 {  // 实际提交时类名为 Foo
public:
    promise<void> p1, p2;  // 两个同步点

    Solution1() {}

    void first(function<void()> printFirst) {
        // first 不需要等待任何人
        printFirst();
        p1.set_value();  // 通知 second: "我完成了"
    }

    void second(function<void()> printSecond) {
        p1.get_future().wait();  // 阻塞等待 first 完成
        printSecond();
        p2.set_value();  // 通知 third: "我完成了"
    }

    void third(function<void()> printThird) {
        p2.get_future().wait();  // 阻塞等待 second 完成
        printThird();
    }
};

// ============================================================
// 解法2: mutex + condition_variable — OS 标准阻塞同步
// 时间: O(1)  空间: O(1)
//
// 【思路】
// 操作系统教科书标准做法:
//   - mutex 保护共享状态
//   - condition_variable 让不满足条件的线程休眠（让出CPU）
//   - notify_all() 唤醒等待的线程重新检查条件
//
// 共享变量 step:
//   step=1: 该执行 first
//   step=2: first 完成，该执行 second
//   step=3: second 完成，该执行 third
//
// 为什么用 notify_all 而不是 notify_one?
//   因为 second 和 third 可能都在等待。
//   notify_one 只唤醒一个，可能唤醒的是 third 而不是 second。
//   notify_all 唤醒所有，不满足条件的会重新休眠。
//
// 关键: cv.wait(lock, predicate) 自动处理 spurious wakeup
//   等价于: while (!predicate()) cv.wait(lock);
//   如果不带谓词，spurious wakeup 会导致线程提前执行。
// ============================================================
class Solution2 {
public:
    mutex mtx;
    condition_variable cv;
    int step = 1;

    Solution2() {}

    void first(function<void()> printFirst) {
        // first 不需要等待
        printFirst();
        {
            lock_guard<mutex> lock(mtx);
            step = 2;  // 更新状态: first 完成
        }
        cv.notify_all();  // 唤醒所有等待线程
    }

    void second(function<void()> printSecond) {
        {
            unique_lock<mutex> lock(mtx);
            // 等待条件: step >= 2 (即 first 已完成)
            // wait(lock, pred): 原子地释放锁并休眠
            // 被唤醒后重新加锁，检查谓词，不满足则继续休眠
            cv.wait(lock, [this]{ return step >= 2; });
        }
        printSecond();
        {
            lock_guard<mutex> lock(mtx);
            step = 3;
        }
        cv.notify_all();
    }

    void third(function<void()> printThird) {
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]{ return step >= 3; });
        }
        printThird();
    }
};

// ============================================================
// 解法3: atomic + 自旋等待 — 忙等方案
// 时间: O(1)  空间: O(1)
//
// 【思路】
// 最直觉的做法: 用原子变量 step，等待的线程不断循环检查。
//
// 为什么必须用 atomic 而不是普通 int?
//   普通 int 在多线程读写中是 undefined behavior!
//   编译器可能优化掉循环（认为 step 不会被其他线程改变），
//   或者 CPU 缓存导致一个线程看不到另一个线程的写入。
//   atomic 保证了可见性（visibility）和原子性（atomicity）。
//
// memory_order 说明:
//   memory_order_release: store 之前的所有写入对其他线程可见
//   memory_order_acquire: load 之后的读取能看到 release 之前的写入
//   这对 release-acquire 建立了 happens-before 关系。
//   不熟悉的话用默认 seq_cst（最严格最安全）。
//
// 优化: 加 this_thread::yield() 让出 CPU 时间片
//   减少自旋开销，但仍然是 busy waiting。
//   真正的无锁等待需要 OS 支持（futex）。
// ============================================================
class Solution3 {
public:
    atomic<int> step{1};

    Solution3() {}

    void first(function<void()> printFirst) {
        printFirst();
        step.store(2, memory_order_release);
    }

    void second(function<void()> printSecond) {
        // 自旋等待: 不断检查 step 是否 >= 2
        while (step.load(memory_order_acquire) < 2) {
            // this_thread::yield();  // 可选: 让出CPU时间片
        }
        printSecond();
        step.store(3, memory_order_release);
    }

    void third(function<void()> printThird) {
        while (step.load(memory_order_acquire) < 3) {
            // this_thread::yield();
        }
        printThird();
    }
};

// ============================================================
// 【解法对比】
//
// | 解法              | 等待方式 | CPU开销 | 代码量 | 面试推荐 |
// |------------------|---------|---------|-------|---------|
// | promise/future   | 阻塞    | 低      | 最少   | ⭐⭐⭐   |
// | mutex+cond_var   | 阻塞    | 低      | 中等   | ⭐⭐⭐   |
// | atomic+自旋       | 忙等    | 高      | 少     | ⭐       |
//
// 演进关系:
//   自旋(解法3): 最直觉，但浪费CPU
//   → mutex+cv(解法2): 让等待线程休眠，OS标准做法
//   → promise/future(解法1): C++ 对一次性同步的语法糖，最简洁
//
// 面试策略:
//   1. 先说自旋思路（展示理解问题本质，30秒）
//   2. 写 promise/future（代码最短，体现C++功底）
//   3. 口述 mutex+cv（展示你懂OS同步原语）
//   4. 讨论忙等 vs 阻塞的 trade-off
//
// ============================================================
// 【易错点】
//
// 1. promise 多次 set_value:
//    ✗ p1.set_value(); p1.set_value();
//      第二次 set_value 抛出 std::future_error
//    ✓ 本题每个函数只调一次，无此问题
//
// 2. condition_variable 不带谓词:
//    ✗ cv.wait(lock);  // spurious wakeup → 提前执行
//    ✓ cv.wait(lock, [this]{ return step >= 2; });
//
// 3. 用普通 int 做自旋变量:
//    ✗ int step = 1; while(step < 2) {}
//      数据竞争 UB! 编译器可能优化掉循环
//    ✓ atomic<int> step{1}; while(step.load() < 2) {}
//
// 4. condition_variable 配 lock_guard:
//    ✗ lock_guard<mutex> lk(mtx); cv.wait(lk, ...);
//      编译错误! wait 需要 unique_lock (能 unlock/lock)
//    ✓ unique_lock<mutex> lk(mtx); cv.wait(lk, ...);
//
// 5. 忘记 notify:
//    ✗ first() 中只写 step=2 没有 cv.notify_all()
//      second() 永远等不到通知
//    ✓ 每次更新状态后都 notify_all()
//
// 6. 在持有锁时执行 printXxx():
//    ✗ lock_guard lk(mtx); printSecond(); // 持锁执行耗时操作
//    ✓ 先 wait 获得锁确认条件，释放锁后再 print
//      （减少锁的持有时间，提高并发性）
//
// ============================================================
// 【面试追问 Interview Follow-ups】
//
// Q1: promise/future 和 condition_variable 底层区别？
// A1: promise/future 是一次性的（set 一次，wait 一次），
//     内部通常基于 mutex+cv 实现，但封装更简洁。
//     cv 更通用，可反复 wait/notify，适合生产者-消费者。
//
// Q2: 自旋等待什么时候比阻塞更好？
// A2: 等待时间极短（纳秒级）且线程数 <= CPU核数时。
//     上下文切换约需微秒级开销，自旋避免了这个开销。
//     Linux 内核 spinlock 就用于极短临界区。
//
// Q3: 改成 N 个线程按序执行怎么设计？
// A3: 数组 promise<void> sync[N-1]; 第 k 个线程等 sync[k-1]。
//     或 condition_variable + atomic<int> step，
//     第 k 个线程 wait(lock, [&]{ return step >= k; });
//
// Q4: 如果 first/second/third 需要反复调用？
// A4: promise 不再适用（只能用一次）。改用 cv + 循环状态机。
//     参考 LC 1115 "Print FooBar Alternately"。
//
// Q5: memory_order 在自旋方案中的作用？
// A5: release 保证 store 之前的写入对其他线程可见。
//     acquire 保证 load 之后能看到 release 之前的写入。
//     这对建立 happens-before 关系，防止指令重排。
//
// 相关题: 1115 Print FooBar Alternately,
//         1116 Print Zero Even Odd,
//         1117 Building H2O,
//         1226 The Dining Philosophers
// ============================================================
