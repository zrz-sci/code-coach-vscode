-- ============================================================
-- 584. Find Customer Referee
-- ============================================================
-- 难度: Easy | 标签: Database
--
-- 题意: 找出不是由 id=2 推荐的客户（包括没有推荐人的客户）
-- 核心考点: SQL 三值逻辑与 NULL 处理
-- ============================================================

-- ============================================================
-- 解法一：OR IS NULL（最标准，推荐）
-- ============================================================
-- 思路：
--   referee_id != 2 会丢失 NULL 行（NULL != 2 -> UNKNOWN，被WHERE过滤）
--   必须显式加上 OR referee_id IS NULL 来捕获无推荐人的客户
-- 执行逻辑:
--   Will  -> referee_id=NULL -> !=2=UNKNOWN, IS NULL=TRUE  -> OR=TRUE  (Y)
--   Jane  -> referee_id=NULL -> !=2=UNKNOWN, IS NULL=TRUE  -> OR=TRUE  (Y)
--   Alex  -> referee_id=2    -> !=2=FALSE,   IS NULL=FALSE -> OR=FALSE (N)
--   Bill  -> referee_id=NULL -> !=2=UNKNOWN, IS NULL=TRUE  -> OR=TRUE  (Y)
--   Zack  -> referee_id=1    -> !=2=TRUE,    IS NULL=FALSE -> OR=TRUE  (Y)
--   Mark  -> referee_id=2    -> !=2=FALSE,   IS NULL=FALSE -> OR=FALSE (N)
SELECT name
FROM Customer
WHERE referee_id != 2 OR referee_id IS NULL;

-- ============================================================
-- 解法二：IFNULL（MySQL）
-- ============================================================
-- 思路：
--   将 NULL 替换为一个确定不等于 2 的值（如 0）
--   然后正常比较即可
-- 注意: 替代值不能选 2，否则 NULL 行会被错误过滤
SELECT name
FROM Customer
WHERE IFNULL(referee_id, 0) != 2;

-- ============================================================
-- 解法三：COALESCE（标准 SQL，跨数据库兼容）
-- ============================================================
-- 思路：同 IFNULL，COALESCE 是 SQL 标准函数
-- COALESCE 返回参数列表中第一个非 NULL 的值
SELECT name
FROM Customer
WHERE COALESCE(referee_id, 0) != 2;

-- ============================================================
-- 解法四：MySQL NULL-safe 等于运算符 <=>
-- ============================================================
-- 思路：
--   <=> 是 MySQL 特有的 NULL-safe 比较运算符
--   NULL <=> 2  -> 0 (FALSE)，NOT 0 = 1 (TRUE) -> 保留
--   NULL <=> NULL -> 1 (TRUE)
--   2 <=> 2 -> 1 (TRUE)，NOT 1 = 0 (FALSE) -> 过滤
-- 优点: 简洁优雅
-- 缺点: MySQL 特有语法，不兼容其他数据库
SELECT name
FROM Customer
WHERE NOT (referee_id <=> 2);
