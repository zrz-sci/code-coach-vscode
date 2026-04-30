#!/usr/bin/env python3
"""
Fetch all favorite problems and generate AI-powered hints.md + reference.cpp for each.

Usage:
    # Set API key first:
    export ANTHROPIC_API_KEY="sk-ant-..."

    # Run:
    python3 fetch_and_generate.py [--favorite SLUG] [--dry-run]

    # Default: fetches all favorites, then generates hints for problems missing them.
"""
import argparse
import json
import os
import sys
import time
from pathlib import Path

from api_client import LeetCodeClient
from config import load_config
from template_generator import generate_with_structures


def get_all_favorite_problems(client: LeetCodeClient) -> list:
    """Fetch all problems from all user favorites."""
    print("Fetching favorites list...")
    favorites = client.get_favorites()
    if not favorites:
        print("No favorites found.")
        return []

    all_problems = []
    seen_slugs = set()

    for fav in favorites:
        fav_id = fav.get('idHash', '')
        fav_name = fav.get('name', 'Unknown')
        print(f"\n  Favorite: {fav_name} ({fav_id})")

        problems = client.get_favorite_problems(fav_id)
        if not problems:
            print(f"    (empty or failed)")
            continue

        for p in problems:
            slug = p.get('titleSlug', '')
            if slug and slug not in seen_slugs:
                seen_slugs.add(slug)
                all_problems.append(p)

        print(f"    {len(problems)} problems")

    print(f"\nTotal unique problems across all favorites: {len(all_problems)}")
    return all_problems


def fetch_problem_if_needed(client: LeetCodeClient, problem: dict, problems_dir: Path) -> Path:
    """Fetch problem and update problem_info.json. Always re-fetches to update bilingual data.
    solution.cpp is preserved if it already exists (handled by template_generator)."""
    slug = problem.get('titleSlug', '')
    pid = str(problem.get('frontendQuestionId', '0'))

    dir_name = f"{pid.zfill(4)}_{slug.replace('-', '_')}"
    problem_dir = problems_dir / dir_name

    # Always fetch to update problem_info.json with bilingual data
    print(f"  Fetching {pid}. {slug}...")
    full_problem = client.get_problem(slug)
    if not full_problem:
        print(f"    Failed to fetch!")
        # Return existing dir if it exists, None otherwise
        return problem_dir if problem_dir.exists() else None

    # Generate/update solution template (solution.cpp won't be overwritten if exists)
    try:
        generate_with_structures(full_problem, str(problems_dir))
    except Exception as e:
        print(f"    Warning: template generation failed ({e}), skipping template")
        # Still return the dir — problem_info.json may have been saved
        problem_dir.mkdir(parents=True, exist_ok=True)
    return problem_dir


def load_teaching_skills() -> str:
    """Load the teaching skills knowledge base for AI generation prompts."""
    skills_path = Path(__file__).parent / "teaching_skills.md"
    if skills_path.exists():
        return skills_path.read_text(encoding='utf-8')
    return ""


def generate_hints_with_ai(problem_dir: Path, problem_info: dict, api_key: str) -> bool:
    """Generate hints.md and reference.cpp in a single API call with optimized prompt."""
    import anthropic

    title = problem_info.get('title', '')
    title_cn = problem_info.get('title_cn', '') or title
    pid = problem_info.get('id', '')
    difficulty = problem_info.get('difficulty', '')
    tags = problem_info.get('tags', '')
    content = problem_info.get('content', '')
    content_cn = problem_info.get('content_cn', '') or content

    # Read the solution template to get the function signature
    solution_file = problem_dir / "solution.cpp"
    solution_code = ''
    if solution_file.exists():
        solution_code = solution_file.read_text()

    # Extract function signature only (skip includes and main)
    func_signature = ''
    for line in solution_code.split('\n'):
        stripped = line.strip()
        if stripped and not stripped.startswith('//') and not stripped.startswith('#') \
                and not stripped.startswith('using') and 'main()' not in stripped \
                and stripped not in ('', '{', '}'):
            func_signature += line + '\n'
    func_signature = func_signature[:500]

    # Load teaching methodology
    teaching_skills = load_teaching_skills()

    client = anthropic.Anthropic(api_key=api_key)

    system_prompt = f"""你是一个 LeetCode 教学专家。你的目标是让读者真正学会每一道题，而不是给答案。

以下是你的教学方法论，严格遵守：

{teaching_skills}"""

    # Single unified prompt — generates both hints.md and reference.cpp
    unified_prompt = f"""为这道题生成完整的教学文档。输出分两部分，用分隔符隔开。

题目 {pid}: {title_cn} ({title})
难度: {difficulty}
标签: {tags}

题目描述（中文）:
{content_cn[:3000]}

题目描述（英文）:
{content[:2000]}

函数签名:
{func_signature}

===== 第一部分: hints.md =====

生成一份完整的教学 markdown，可以直接贴到 LeetCode 题解。结构如下：

# {pid}. {title_cn}

## 核心思路
一句话说清楚这道题本质上在问什么。

## 思维链
用 4-6 步展示"从零开始怎么想到最优解"的推导过程：
1. 读完题第一反应 → 最朴素的做法是什么？
2. 暴力解的瓶颈在哪？→ 哪个操作最耗时？
3. 怎么突破瓶颈？→ 什么数据结构/算法能优化这个操作？
4. 具体怎么用？→ 核心逻辑
（这是整个文档最重要的部分，必须让人看完觉得"原来是这么想到的"）

## 解法概览
| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
表格列出 2-4 种解法。"面试"列用以下标注：
- ⭐ 必须写出（面试官 80% 期望这个解法）
- 能说出即可（暴力解，展示理解）
- 加分项（最优解或特殊技巧）

## 关键提示
- 3-5 个提示，引导思考方向，不给完整解法
- 如果是树/图/链表/矩阵题，画 ASCII 示意图

## 解法详解

### 解法1: 名称 — O(?) / O(?)
**思考过程**: 为什么想到这个方法？从什么观察出发？
```cpp
class Solution {{
public:
    // 完整可编译代码，关键行有注释
}};
```
**关键点**: 这个解法最容易写错的地方。

### 解法2: 名称 — O(?) / O(?) ⭐ 面试首选
**从解法1优化**: 解法1的瓶颈是X，通过Y来突破。
```cpp
class Solution {{
public:
    // 完整可编译代码
}};
```

（每种解法都要有完整的思考过程和可运行代码。如果是系列题，显式标注和上一题的代码差异。）

## 解法对比
| | 解法1 | 解法2 | ... |
关键区别 + 什么时候选哪个。

## 易错点
- 必须是具体的代码 bug，不是"注意边界条件"
- 格式：✗ 错误写法 → ✓ 正确写法，说明为什么

## 面试追问
递进链：Q1(基础理解) → Q2(优化) → Q3(变体)，每个含简短回答。

## 相关题型
- 说明复用什么技巧/模板。如果是系列题，标注代码改了哪一行。

===== 第二部分: reference.cpp =====

然后生成纯 C++ 代码版本，格式要求：
1. 开头注释包含【题目本质】【解法总览】
2. 每种解法用 class Solution1, Solution2 等区分
3. 每种解法的注释包含【思路】（思考过程，不是代码描述）
4. 如果是树/链表/图题，注释中画 ASCII 示意图
5. 如果是 DP 题，注释画填表方向
6. 如果是回溯题，注释画决策树
7. 末尾注释包含【解法对比】【易错点】【面试追问】
8. 不要 #include、using namespace、main()
9. 注释用中文，变量名用英文
10. 面试追问是递进链
11. 易错点是具体代码 bug

===== 输出格式 =====

你的输出必须严格按以下格式，用分隔符隔开两部分：

---HINTS_MD---
（这里输出 hints.md 的完整 markdown 内容）
---REFERENCE_CPP---
（这里输出 reference.cpp 的纯 C++ 代码，不要用 markdown 代码块包裹）"""

    print(f"    Generating (unified)...")
    response = client.messages.create(
        model="claude-opus-4-6",
        max_tokens=12000,
        system=system_prompt,
        messages=[{"role": "user", "content": unified_prompt}]
    )
    output = response.content[0].text

    # Parse output into two files
    if '---HINTS_MD---' not in output or '---REFERENCE_CPP---' not in output:
        # Fallback: try to split intelligently
        print(f"    Warning: missing separators, attempting fallback parse")
        if '---REFERENCE_CPP---' in output:
            hints_content = output.split('---REFERENCE_CPP---')[0].strip()
            ref_content = output.split('---REFERENCE_CPP---')[1].strip()
        else:
            # Last resort: everything is hints, no separate reference
            hints_content = output
            ref_content = ''
    else:
        parts = output.split('---REFERENCE_CPP---')
        hints_content = parts[0].replace('---HINTS_MD---', '').strip()
        ref_content = parts[1].strip()

    # Clean up reference.cpp
    if ref_content:
        if '```cpp' in ref_content:
            ref_content = ref_content.split('```cpp')[1].split('```')[0].strip()
        elif '```c++' in ref_content:
            ref_content = ref_content.split('```c++')[1].split('```')[0].strip()
        elif '```' in ref_content:
            # Only strip if it looks like a code block wrapper
            lines = ref_content.split('\n')
            if lines[0].strip() == '```':
                ref_content = '\n'.join(lines[1:]).split('```')[0].strip()

    # Write files
    (problem_dir / "hints.md").write_text(hints_content, encoding='utf-8')
    if ref_content:
        (problem_dir / "reference.cpp").write_text(ref_content + '\n', encoding='utf-8')

    return True


def get_local_problems(client: LeetCodeClient, problems_dir: Path) -> list:
    """Get all locally saved problems by reading problem_info.json files."""
    local_problems = []
    for info_file in sorted(problems_dir.glob('*/problem_info.json')):
        try:
            info = json.loads(info_file.read_text())
            slug = info.get('titleSlug', '')
            pid = info.get('id', '0')
            if slug:
                local_problems.append({
                    'titleSlug': slug,
                    'frontendQuestionId': pid,
                    'local_dir': str(info_file.parent),
                })
        except (json.JSONDecodeError, IOError):
            continue
    return local_problems


def main():
    parser = argparse.ArgumentParser(description='Fetch favorites and generate AI hints')
    parser.add_argument('--favorite', help='Specific favorite slug to process')
    parser.add_argument('--local', action='store_true',
                        help='Re-fetch only locally existing problems (update problem_info.json)')
    parser.add_argument('--dry-run', action='store_true', help='Only fetch, skip AI generation')
    parser.add_argument('--force', action='store_true', help='Regenerate even if hints already exist')
    parser.add_argument('--api-key', help='Anthropic API key (or set ANTHROPIC_API_KEY env var)')
    args = parser.parse_args()

    api_key = args.api_key or os.environ.get('ANTHROPIC_API_KEY', '')
    if not api_key and not args.dry_run:
        print("Error: Anthropic API key required for AI generation.")
        print("Set ANTHROPIC_API_KEY env var or use --api-key")
        print("Use --dry-run to only fetch problems without generating hints.")
        sys.exit(1)

    config = load_config()
    problems_dir = Path(config.get('problems_dir', 'problems'))
    problems_dir.mkdir(parents=True, exist_ok=True)

    client = LeetCodeClient()

    if not client.is_logged_in():
        print("Error: Not logged in. Run 'python3 leetcode_tool.py login' first.")
        sys.exit(1)

    # Get problems to process
    if args.local:
        # Re-fetch locally existing problems (no need to query favorites)
        problems = get_local_problems(client, problems_dir)
        print(f"Found {len(problems)} local problems to re-fetch.")
    elif args.favorite:
        print(f"Fetching favorite: {args.favorite}")
        problems = client.get_favorite_problems(args.favorite)
        if not problems:
            print("No problems found in this favorite.")
            sys.exit(1)
    else:
        problems = get_all_favorite_problems(client)

    if not problems:
        sys.exit(1)

    # Phase 1: Fetch all problems
    print(f"\n{'='*50}")
    print(f"  Phase 1: Fetching {len(problems)} problems")
    print(f"{'='*50}\n")

    fetched_dirs = []
    for i, p in enumerate(problems):
        slug = p.get('titleSlug', '')
        pid = str(p.get('frontendQuestionId', '0'))
        print(f"[{i+1}/{len(problems)}] {pid}. {slug}")

        problem_dir = fetch_problem_if_needed(client, p, problems_dir)
        if problem_dir:
            fetched_dirs.append(problem_dir)
        else:
            print(f"  Skipped (fetch failed)")

        # Rate limit
        time.sleep(0.5)

    print(f"\nFetched {len(fetched_dirs)} problems.")

    if args.dry_run:
        print("\nDry run complete. Skipping AI generation.")
        return

    # Phase 2: Generate hints + reference
    print(f"\n{'='*50}")
    print(f"  Phase 2: Generating AI hints & references")
    print(f"{'='*50}\n")

    generated = 0
    skipped = 0
    failed = 0

    for i, problem_dir in enumerate(fetched_dirs):
        info_file = problem_dir / "problem_info.json"
        if not info_file.exists():
            continue

        info = json.loads(info_file.read_text())
        pid = info.get('id', '?')
        title = info.get('title', '?')

        # Skip if already has hints (unless --force)
        if not args.force and (problem_dir / "hints.md").exists() and (problem_dir / "reference.cpp").exists():
            print(f"[{i+1}/{len(fetched_dirs)}] {pid}. {title} — already has hints, skipping")
            skipped += 1
            continue

        print(f"[{i+1}/{len(fetched_dirs)}] {pid}. {title}")

        try:
            success = generate_hints_with_ai(problem_dir, info, api_key)
            if success:
                generated += 1
                print(f"    ✓ Done")
            else:
                failed += 1
                print(f"    ✗ Failed")
        except Exception as e:
            failed += 1
            print(f"    ✗ Error: {e}")

        # Rate limit for API
        time.sleep(1)

    print(f"\n{'='*50}")
    print(f"  Complete!")
    print(f"  Generated: {generated}")
    print(f"  Skipped (already existed): {skipped}")
    print(f"  Failed: {failed}")
    print(f"{'='*50}")


if __name__ == '__main__':
    main()
