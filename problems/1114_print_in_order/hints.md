# 1114. 按序打印 (Print in Order)

## 核心思路

三个线程共享一个 Foo 实例，分别调用 `first()`、`second()`、`third()`，但线程调度顺序不确定。我们需要用**同步原语**确保执行顺序始终是 first -> second -> third。这是一道经典的**线程同步/并发控制**题，考察你对操作系统原语（mutex、condition variable、semaphore、atomic、promise/future）的理解和运用。

## 思维链

1. **读完题第一反应**：三个线程并发启动，但 second 必须等 first 完成后才能执行，third 必须等 second 完成后才能执行。这就是一个**顺序依赖**的同步问题。

2. **最简单的思路 — 自旋等待**：用一个共享的 `step` 变量，初始为 1。first() 执行后把 step 设为 2；second() 在循环中不断检查 step 是否为 2，是就执行然后设为 3；third() 类似等 step 为 3。这就是 **busy waiting / spin lock**。

3. **自旋等待有什么问题？** 等待的线程在不断空转检查条件，消耗 CPU。在面试中可以先写出来展示理解，然后讨论改进。

4. **更好的方案 — 阻塞等待**：
   - **mutex + condition_variable**：让等待的线程休眠（让出 CPU），当条件满足时被唤醒。这是 OS 教科书的标准做法。
   - **promise/future**：C++ 特有的一次性同步原语，一个线程 set_value，另一个线程 wait。代码最简洁。
   - **信号量 (semaphore)**：C++20 引入了 `std::counting_semaphore`，更早的版本可以用 POSIX semaphore。

5. **面试最佳策略**：先说出自旋等待的思路（展示你理解问题），然后写 promise/future（代码最短），最后口头描述 mutex + condition_variable（展示你懂阻塞同步）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| promise/future | 两个一次性同步点 | O(1) | O(1) | ⭐ 代码最简洁 |
| mutex + condition_variable | 条件变量阻塞等待 | O(1) | O(1) | ⭐ OS标准做法 |
| atomic + 自旋 | 原子变量忙等 | O(1) | O(1) | 能说出即可 |
| 信号量 | 计数信号量控制顺序 | O(1) | O(1) | 加分项 |

## 关键提示

1. **promise 只能 set_value 一次**：如果多次调用 `set_value`，会抛异常。这正好匹配我们的需求 — first() 只执行一次。

2. **condition_variable 必须和 unique_lock 配合**：`cv.wait(lock, predicate)` 会原子地释放锁并休眠，被唤醒后重新获取锁。

3. **自旋等待需要 atomic**：如果用普通 int 作为共享变量，在多线程环境下会有数据竞争（undefined behavior）。必须用 `atomic<int>` 或者加锁保护。

4. **spurious wakeup（虚假唤醒）**：condition_variable 可能在没有被 notify 的情况下醒来。所以 `wait` 必须带谓词（predicate），用 while 循环检查条件，不能只用 if。

5. **LeetCode 的评测环境**：LeetCode 对并发题的评测方式是启动三个线程并检查输出顺序。你的代码不需要自己创建线程，只需要在 Foo 类中实现同步逻辑。

## 解法详解

### 解法1: promise/future — O(1) / O(1)

**思考过程**：promise/future 是 C++ 的一次性同步机制。promise 持有"写端"，future 持有"读端"。一个线程通过 `promise.set_value()` 发信号，另一个线程通过 `future.wait()` 等待。

**关键决策**：需要两个同步点：first→second 和 second→third，所以用两个 promise。

```
线程执行时序:

Thread A (first):   [执行 printFirst] → p1.set_value()
Thread B (second):  p1.wait() ←────────→ [执行 printSecond] → p2.set_value()
Thread C (third):   p2.wait() ←─────────────────────────────→ [执行 printThird]
```

```cpp
#include <future>
class Foo {
    promise<void> p1, p2;  // 两个同步点
public:
    Foo() {}

    void first(function<void()> printFirst) {
        printFirst();         // 执行打印
        p1.set_value();       // 通知 second 可以执行了
    }

    void second(function<void()> printSecond) {
        p1.get_future().wait();  // 等待 first 完成
        printSecond();            // 执行打印
        p2.set_value();           // 通知 third 可以执行了
    }

    void third(function<void()> printThird) {
        p2.get_future().wait();  // 等待 second 完成
        printThird();             // 执行打印
    }
};
```

**优势**：代码极其简洁，没有自旋开销，语义清晰。
**注意**：promise 只能 set_value 一次，get_future 也只能调用一次。但本题每个函数只调用一次，完美匹配。

### 解法2: mutex + condition_variable — O(1) / O(1)

**思考过程**：操作系统教科书的标准做法。用一个共享变量 `step` 记录当前应该执行第几步，condition_variable 让不满足条件的线程休眠。

**关键决策**：
- `step` 初始为 1，first() 执行后设为 2，second() 设为 3
- second() 等待 `step >= 2`，third() 等待 `step >= 3`
- `notify_all()` 唤醒所有等待的线程（因为可能 second 和 third 都在等）

```cpp
#include <mutex>
#include <condition_variable>

class Foo {
    mutex mtx;
    condition_variable cv;
    int step = 1;  // 当前应该执行第几步
    
public:
    Foo() {}
    
    void first(function<void()> printFirst) {
        // first 不需要等待，直接执行
        printFirst();
        {
            lock_guard<mutex> lock(mtx);
            step = 2;  // 更新状态
        }
        cv.notify_all();  // 唤醒等待的线程
    }
    
    void second(function<void()> printSecond) {
        {
            unique_lock<mutex> lock(mtx);
            // 等待 step >= 2，即 first 已完成
            // 带谓词的 wait 自动处理 spurious wakeup
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
```

**优势**：线程在等待时休眠，不消耗 CPU（和自旋的关键区别）。
**注意**：`wait` 必须带谓词 lambda，否则 spurious wakeup 会导致提前执行。

### 解法3: atomic + 自旋等待 — O(1) / O(1)

**思考过程**：最直觉的做法 — 用一个原子变量，等待的线程不断循环检查条件。

```cpp
#include <atomic>

class Foo {
    atomic<int> step{1};  // 原子变量，线程安全
    
public:
    Foo() {}
    
    void first(function<void()> printFirst) {
        printFirst();
        step.store(2, memory_order_release);  // 设为2
    }
    
    void second(function<void()> printSecond) {
        // 自旋等待: 不断检查 step 是否为 2
        while (step.load(memory_order_acquire) < 2) {
            // 空转等待（忙等）
            // this_thread::yield(); // 可以加 yield 减少 CPU 消耗
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
```

**优势**：代码直观，容易理解。
**劣势**：忙等浪费 CPU。在实际生产环境中不推荐。面试中可以加 `this_thread::yield()` 表示你知道这个问题。

## 解法对比

| 解法 | 等待方式 | CPU开销 | 代码复杂度 | 面试推荐 |
|------|---------|---------|-----------|---------|
| promise/future | 阻塞 | 低 | 简单 | ⭐⭐⭐ |
| mutex+cv | 阻塞 | 低 | 中等 | ⭐⭐⭐ |
| atomic+自旋 | 忙等 | 高 | 简单 | ⭐ |

**从自旋到阻塞的演进逻辑**：
- 自旋等待（解法3）最直觉，但浪费 CPU
- condition_variable（解法2）让等待线程休眠，OS 标准做法
- promise/future（解法1）是 C++ 对"一次性事件通知"的语法糖，最简洁

**面试策略**：
1. 先说自旋思路（展示理解问题本质）
2. 写 promise/future（代码最短，体现 C++ 功底）
3. 口头描述 mutex+cv（展示你懂 OS 同步原语）

## 易错点

1. **promise 多次 set_value**：
   ```cpp
   // ✗ 错误: promise 只能 set_value 一次
   p1.set_value();
   p1.set_value();  // 抛出 std::future_error!
   
   // ✓ 本题每个函数只调一次，所以没问题
   ```

2. **condition_variable 不带谓词**：
   ```cpp
   // ✗ 错误: 没有谓词，spurious wakeup 会导致提前执行
   cv.wait(lock);  // 可能在 step 未更新时就醒来
   
   // ✓ 正确: 带谓词
   cv.wait(lock, [this]{ return step >= 2; });
   ```

3. **普通 int 做自旋变量（数据竞争）**：
   ```cpp
   // ✗ 严重错误: 普通 int 在多线程读写中是 undefined behavior
   int step = 1;
   while (step < 2) {}  // UB! 编译器可能优化掉这个循环
   
   // ✓ 正确: 用 atomic
   atomic<int> step{1};
   while (step.load() < 2) {}
   ```

4. **condition_variable 用 lock_guard 而不是 unique_lock**：
   ```cpp
   // ✗ 编译错误: wait 需要 unique_lock（能 unlock/lock）
   lock_guard<mutex> lock(mtx);
   cv.wait(lock, ...);  // lock_guard 没有 unlock 方法
   
   // ✓ 正确: 用 unique_lock
   unique_lock<mutex> lock(mtx);
   cv.wait(lock, ...);
   ```

5. **忘记 notify**：
   ```cpp
   // ✗ 错误: first() 完成后没有通知
   void first(...) {
       printFirst();
       step = 2;
       // cv.notify_all(); ← 忘了这行！
       // second() 会永远等待
   }
   ```

## 面试追问

**Q1: promise/future 和 condition_variable 的底层区别？**
A1: promise/future 是一次性的（set 一次，wait 一次），内部通常基于 mutex+cv 实现但封装更简洁。condition_variable 更通用，可以反复 wait/notify，适合需要多次同步的场景（如生产者-消费者）。

**Q2: 自旋等待什么时候比 condition_variable 更好？**
A2: 当等待时间极短（纳秒级）且线程数不超过 CPU 核数时，自旋避免了线程切换的开销（上下文切换约需微秒级）。Linux 内核中的 spinlock 就是这个场景。但如果等待时间不确定，阻塞等待更好。

**Q3: 如果改成 N 个线程按顺序执行怎么设计？**
A3: 用一个 atomic<int> step 和 N-1 个同步点。或者用一个 condition_variable + step 变量，第 k 个线程等待 `step >= k`。通用写法可以用数组 `promise<void> sync[N-1]`。

**Q4: 如果 first/second/third 需要反复调用（而不是只调一次）呢？**
A4: promise/future 不再适用（只能用一次）。需要改用 condition_variable + 循环状态机，或者用信号量。这就是 LC 1115 "Print FooBar Alternately" 的模式。

**Q5: memory_order 在自旋方案中的作用？**
A5: `memory_order_release` 确保 set 之前的所有写入对其他线程可见（happens-before）。`memory_order_acquire` 确保 load 之后的读取能看到 release 之前的写入。如果不指定，默认 `seq_cst` 最严格，性能略差但最安全。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 1115 | Print FooBar Alternately | 两线程交替执行 |
| 1116 | Print Zero Even Odd | 三线程协调打印 |
| 1117 | Building H2O | 信号量控制资源 |
| 1188 | Design Bounded Blocking Queue | 生产者-消费者模式 |
| 1226 | The Dining Philosophers | 经典哲学家就餐问题 |
