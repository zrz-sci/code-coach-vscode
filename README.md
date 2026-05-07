# Code Coach

A VS Code extension for practicing coding problems with AI-powered reference solutions, progress tracking, and bilingual support (English & Chinese).

Works with both LeetCode US (leetcode.com) and LeetCode CN (leetcode.cn).

<!-- ![Demo](docs/demo.gif) -->

## Highlights

- **AI-powered study aids** — generate reference solutions with your own Claude API key
- **Progress tracking** with spaced repetition (Ebbinghaus forgetting curve) review schedule
- **Smart recommendations** — follows your favorite folder order, skips completed problems
- **Bilingual** — problem descriptions in both English and Chinese
- **Local testing** — compile & run with g++ without leaving VS Code

## Features

### Browse & Solve
- Browse problems by difficulty, search by keyword
- Favorites and custom lists synced from your LeetCode account
- One-click fetch with auto-generated C++ solution template

### AI-Powered Study Aids
- **reference.cpp** — thought process, multiple solutions with analysis, common mistakes, interview follow-ups
- Powered by Claude API (bring your own key)

### Code & Submit
| Shortcut | Action |
|----------|--------|
| `Cmd+Alt+T` / `Ctrl+Alt+T` | Compile & test locally with g++ |
| `Cmd+Alt+R` / `Ctrl+Alt+R` | Run against LeetCode online test cases |
| `Cmd+Alt+S` / `Ctrl+Alt+S` | Submit solution (shows Runtime/Memory percentile) |

### Progress Tracking
- Automatic progress recording on every submit (pass/fail, attempt count, timestamps)
- Spaced repetition review schedule based on the Ebbinghaus forgetting curve
- Status icons in the solution tree: passed / attempted / untouched
- Daily review reminders in the Session panel

### Smart Recommendations
- **Next Problem** follows your My Favorites folder order (Array → LinkedList → ... → Dynamic Programming)
- Automatically skips completed problems
- Progress bar shows overall completion percentage

### Bilingual Support
- Problem descriptions available in both English and Chinese
- Toggle language with one click in the description panel
- Configurable display language (Settings > Display Language)

### Solution Management
- Organized tree view: each problem has `solution.cpp`, `reference.cpp`, `testcases.txt`
- Reset solution to re-practice from scratch
- Expandable file tree with syntax-appropriate icons

## Requirements

- **Python 3.8+** with `requests` and `html2text` packages
- **g++** (optional, for local compile & test)
- **Playwright** (optional, for browser-based login)

```bash
# Install Python dependencies
pip install requests html2text

# Optional: browser login support
pip install playwright && python3 -m playwright install chromium
```

## Installation from Source

```bash
# 1. Clone the repo
git clone https://github.com/zrz-sci/code-coach-vscode.git
cd code-coach-vscode

# 2. Install Node.js dependencies
npm install

# 3. Compile TypeScript
npm run compile

# 4. Install Python dependencies
pip install requests html2text

# 5. (Optional) Browser login support
pip install playwright && python3 -m playwright install chromium

# 6. Package as .vsix
npm install -g @vscode/vsce
vsce package

# 7. Install in VS Code
code --install-extension code-coach-1.1.0.vsix
```

After installation, restart VS Code. You'll see the **Code Coach** icon in the Activity Bar.

> **Tip:** Set `leetcode.problemsDir` in VS Code settings to your preferred directory for storing problems.

## Getting Started

1. Install the extension from VS Code Marketplace
2. Click the **Code Coach** icon in the Activity Bar (sidebar)
3. Click **Login** and choose your site (US or CN)
4. A browser window opens — log in normally, session is captured automatically
5. Browse problems, click to fetch, and start solving!

Problems are stored in `~/lc-practice/` by default (configurable in settings).

## Settings

Open VS Code Settings (`Cmd+,`) and search `leetcode`:

| Setting | Description | Default |
|---------|-------------|---------|
| `leetcode.site` | LeetCode site (us / cn) | `us` |
| `leetcode.pythonPath` | Path to Python 3 interpreter | `python3` |
| `leetcode.problemsDir` | Directory to store problems | `~/lc-practice` |
| `leetcode.proxy` | HTTP proxy URL (optional) | _(empty)_ |
| `leetcode.anthropicApiKey` | Claude API key for AI reference | _(empty)_ |
| `leetcode.hideProblemTitle` | Hide problem title in editor tab | `false` |

You can also access settings from the sidebar: **Session > Settings**.

## AI Reference Generation

To generate AI-powered reference solutions:

1. Set your API key: **Session > Settings > Set AI API Key**
2. Enter your Anthropic API key (`sk-ant-...`)
3. Use **Settings > Re-fetch All Favorites** to batch-generate references
4. Each problem gets a `reference.cpp` with hints, analysis, and code

Without an API key, problems are fetched without AI-generated content.

Get your API key at [console.anthropic.com](https://console.anthropic.com).

## How It Works

```
┌─ Session Panel ─────────────────────┐
│  Progress: 42/462 (9.1%)            │
│  Next: #34. Find First and Last...  │
│  Review: 3 due today                │
│  Site: LeetCode CN                  │
│  User: your_username                │
└─────────────────────────────────────┘

┌─ My Favorites ──────────────────────┐
│  ▶ 1_Array (16 problems)            │
│  ▶ 2_LinkedList (12 problems)       │
│  ▶ ...                              │
│  ▶ 10_Dynamic_Program (45 problems) │
└─────────────────────────────────────┘

┌─ My Solutions ──────────────────────┐
│  ✅ 704. Binary Search         Easy │
│  ✅ 35. Search Insert Position Easy │
│  ⬜ 34. Find First and Last... Med  │
│  ...                                │
└─────────────────────────────────────┘
```

## Disclaimer

- This extension uses unofficial LeetCode APIs and may break if endpoints change.
- Problem content is fetched on-demand and stored locally on your machine only.
- This extension is **not affiliated with or endorsed by LeetCode**.
- Use at your own risk. The authors are not responsible for any account-related issues.

## Contributing

Contributions are welcome! Please open an issue or pull request on GitHub.

## License

[MIT License](LICENSE) — see LICENSE file for details.
