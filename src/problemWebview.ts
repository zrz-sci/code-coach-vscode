import * as vscode from 'vscode';
import { Problem } from './leetcodeService';

export class ProblemWebview {
    private static panel: vscode.WebviewPanel | undefined;

    static show(context: vscode.ExtensionContext, problem: Problem, column: vscode.ViewColumn = vscode.ViewColumn.One): void {
        const hideTitle = vscode.workspace.getConfiguration('leetcode').get('hideProblemTitle', false);
        const panelTitle = hideTitle ? `#${problem.id}` : `${problem.id}. ${problem.title}`;

        if (ProblemWebview.panel) {
            ProblemWebview.panel.title = panelTitle;
            ProblemWebview.panel.reveal(column);
            ProblemWebview.panel.webview.html = ProblemWebview.getWebviewContent(problem, hideTitle);
            return;
        }

        ProblemWebview.panel = vscode.window.createWebviewPanel(
            'leetcodeProblem',
            panelTitle,
            column,
            {
                enableScripts: true,
                retainContextWhenHidden: true
            }
        );

        ProblemWebview.panel.webview.html = ProblemWebview.getWebviewContent(problem, hideTitle);

        ProblemWebview.panel.onDidDispose(() => {
            ProblemWebview.panel = undefined;
        }, null, context.subscriptions);
    }

    private static getWebviewContent(problem: Problem, hideTitle: boolean = false): string {
        const difficultyColor = {
            'Easy': '#00b8a3',
            'Medium': '#ffc01e',
            'Hard': '#ff375f'
        }[problem.difficulty] || '#fff';

        const hasBilingual = problem.content_cn && problem.content_en
            && problem.content_cn !== problem.content_en;

        const titleEn = problem.title_en || problem.title || '';
        const titleCn = problem.title_cn || problem.title || '';
        const contentEn = problem.content_en || problem.content || '';
        const contentCn = problem.content_cn || problem.content || '';

        const langToggle = hasBilingual ? `
    <div class="lang-toggle">
        <button id="btn-en" class="lang-btn active" onclick="switchLang('en')">English</button>
        <button id="btn-cn" class="lang-btn" onclick="switchLang('cn')">中文</button>
    </div>` : '';

        const contentSection = hasBilingual ? `
    <div id="content-en" class="content">${contentEn}</div>
    <div id="content-cn" class="content" style="display:none">${contentCn}</div>` : `
    <div class="content">${problem.content || '<p>Problem description not available.</p>'}</div>`;

        const titleSection = hideTitle
            ? ''
            : hasBilingual
                ? `<h1><span id="title-en">${problem.id}. ${titleEn}</span><span id="title-cn" style="display:none">${problem.id}. ${titleCn}</span></h1>`
                : `<h1>${problem.id}. ${problem.title}</h1>`;

        return `<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="Content-Security-Policy" content="default-src 'none'; style-src 'unsafe-inline'; script-src 'unsafe-inline'; img-src https: data:;">
    <title>${problem.title}</title>
    <style>
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
            padding: 20px;
            line-height: 1.6;
            color: var(--vscode-foreground);
            background-color: var(--vscode-editor-background);
        }
        h1 {
            font-size: 1.5em;
            margin-bottom: 10px;
            border-bottom: 1px solid var(--vscode-panel-border);
            padding-bottom: 10px;
        }
        .difficulty {
            display: inline-block;
            padding: 2px 10px;
            border-radius: 12px;
            font-size: 12px;
            font-weight: bold;
            color: white;
            background-color: ${difficultyColor};
        }
        .meta {
            margin: 15px 0;
            color: var(--vscode-descriptionForeground);
        }
        .lang-toggle {
            display: inline-flex;
            margin-left: 16px;
            border: 1px solid var(--vscode-panel-border);
            border-radius: 6px;
            overflow: hidden;
        }
        .lang-btn {
            padding: 3px 12px;
            border: none;
            cursor: pointer;
            font-size: 12px;
            background: transparent;
            color: var(--vscode-descriptionForeground);
        }
        .lang-btn.active {
            background: var(--vscode-button-background);
            color: var(--vscode-button-foreground);
        }
        .content {
            margin-top: 20px;
        }
        pre {
            background-color: var(--vscode-textCodeBlock-background);
            padding: 10px;
            border-radius: 5px;
            overflow-x: auto;
        }
        code {
            font-family: 'SF Mono', Consolas, 'Liberation Mono', Menlo, monospace;
        }
    </style>
</head>
<body>
    ${titleSection}
    <div class="meta">
        <span class="difficulty">${problem.difficulty}</span>
        ${problem.acRate ? `<span style="margin-left: 10px;">Acceptance: ${problem.acRate}%</span>` : ''}
        ${langToggle}
    </div>
    ${contentSection}
    ${problem.codeSnippet ? `
    <h2>Code Template</h2>
    <pre><code>${problem.codeSnippet}</code></pre>
    ` : ''}
    ${hasBilingual ? `
    <script>
        function switchLang(lang) {
            document.getElementById('content-en').style.display = lang === 'en' ? '' : 'none';
            document.getElementById('content-cn').style.display = lang === 'cn' ? '' : 'none';
            document.getElementById('title-en').style.display = lang === 'en' ? '' : 'none';
            document.getElementById('title-cn').style.display = lang === 'cn' ? '' : 'none';
            document.getElementById('btn-en').className = 'lang-btn' + (lang === 'en' ? ' active' : '');
            document.getElementById('btn-cn').className = 'lang-btn' + (lang === 'cn' ? ' active' : '');
        }
    </script>
    ` : ''}
</body>
</html>`;
    }
}
