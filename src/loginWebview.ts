import * as vscode from 'vscode';

export class LoginWebview {
    private static panel: vscode.WebviewPanel | undefined;

    static show(context: vscode.ExtensionContext, onLogin: (session: string, csrf: string, proxy: string) => Promise<boolean>, onBrowserLogin?: () => Promise<void>) {
        if (this.panel) {
            this.panel.reveal();
            return;
        }

        // Get current proxy setting
        const currentProxy = vscode.workspace.getConfiguration('leetcode').get('proxy', '');

        this.panel = vscode.window.createWebviewPanel(
            'leetcodeLogin',
            'LeetCode Login',
            vscode.ViewColumn.One,
            {
                enableScripts: true,
                retainContextWhenHidden: true
            }
        );

        this.panel.webview.html = this.getHtml(currentProxy as string);

        this.panel.webview.onDidReceiveMessage(async (message) => {
            if (message.command === 'login') {
                const success = await onLogin(message.session, message.csrf, message.proxy);
                if (success) {
                    vscode.window.showInformationMessage('Login successful! 登录成功!');
                    this.panel?.dispose();
                } else {
                    this.panel?.webview.postMessage({ command: 'error', message: 'Login failed. Please check your cookies and proxy settings.' });
                }
            } else if (message.command === 'browserLogin') {
                this.panel?.dispose();
                if (onBrowserLogin) {
                    await onBrowserLogin();
                }
            } else if (message.command === 'cancel') {
                this.panel?.dispose();
            }
        });

        this.panel.onDidDispose(() => {
            this.panel = undefined;
        });
    }

    private static getHtml(currentProxy: string): string {
        return `<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <style>
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            padding: 20px;
            max-width: 600px;
            margin: 0 auto;
        }
        h2 {
            color: var(--vscode-foreground);
            margin-bottom: 10px;
        }
        .browser-login-section {
            background: var(--vscode-button-background);
            padding: 24px;
            border-radius: 8px;
            margin-bottom: 24px;
            text-align: center;
        }
        .browser-login-section p {
            color: var(--vscode-button-foreground);
            margin: 0 0 8px 0;
            font-size: 13px;
            line-height: 1.5;
        }
        .browser-login-section .title {
            font-size: 16px;
            font-weight: 600;
            margin-bottom: 12px;
        }
        .btn-browser {
            padding: 14px 40px;
            font-size: 16px;
            background: var(--vscode-button-foreground);
            color: var(--vscode-button-background);
            border: none;
            border-radius: 6px;
            cursor: pointer;
            margin-top: 12px;
            font-weight: 600;
        }
        .btn-browser:hover {
            opacity: 0.9;
        }
        .recommended {
            display: inline-block;
            background: var(--vscode-badge-background);
            color: var(--vscode-badge-foreground);
            padding: 2px 8px;
            border-radius: 10px;
            font-size: 11px;
            margin-left: 6px;
        }
        details {
            margin-top: 16px;
            border: 1px solid var(--vscode-panel-border);
            border-radius: 6px;
            padding: 0;
        }
        summary {
            padding: 12px 16px;
            cursor: pointer;
            color: var(--vscode-descriptionForeground);
            font-size: 13px;
        }
        summary:hover {
            color: var(--vscode-foreground);
        }
        .manual-content {
            padding: 0 16px 16px 16px;
        }
        .instructions {
            background: var(--vscode-textBlockQuote-background);
            border-left: 3px solid var(--vscode-textLink-foreground);
            padding: 12px 16px;
            margin: 12px 0;
            font-size: 13px;
            line-height: 1.6;
        }
        .instructions ol {
            margin: 8px 0;
            padding-left: 20px;
        }
        .instructions code {
            background: var(--vscode-textCodeBlock-background);
            padding: 2px 6px;
            border-radius: 3px;
            font-family: monospace;
        }
        .form-group {
            margin: 16px 0;
        }
        label {
            display: block;
            margin-bottom: 6px;
            font-weight: 500;
            color: var(--vscode-foreground);
        }
        input {
            width: 100%;
            padding: 10px;
            border: 1px solid var(--vscode-input-border);
            background: var(--vscode-input-background);
            color: var(--vscode-input-foreground);
            border-radius: 4px;
            font-size: 14px;
            box-sizing: border-box;
        }
        input:focus {
            outline: 1px solid var(--vscode-focusBorder);
        }
        .buttons {
            margin-top: 16px;
            display: flex;
            gap: 12px;
        }
        button {
            padding: 10px 24px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            font-size: 14px;
        }
        .btn-primary {
            background: var(--vscode-button-background);
            color: var(--vscode-button-foreground);
        }
        .btn-primary:hover {
            background: var(--vscode-button-hoverBackground);
        }
        .btn-secondary {
            background: var(--vscode-button-secondaryBackground);
            color: var(--vscode-button-secondaryForeground);
        }
        .error {
            color: var(--vscode-errorForeground);
            background: var(--vscode-inputValidation-errorBackground);
            border: 1px solid var(--vscode-inputValidation-errorBorder);
            padding: 10px;
            border-radius: 4px;
            margin-top: 16px;
            display: none;
        }
        .tip {
            font-size: 12px;
            color: var(--vscode-descriptionForeground);
            margin-top: 4px;
        }
    </style>
</head>
<body>
    <h2>Login to LeetCode</h2>

    <div class="browser-login-section">
        <p class="title">Browser Login <span class="recommended">Recommended</span></p>
        <p>A browser window will open automatically.</p>
        <p>Log in to LeetCode normally, and the tool will capture your session.</p>
        <button class="btn-browser" onclick="browserLogin()">
            Open Browser & Login
        </button>
    </div>

    <details>
        <summary>Advanced: Manual Cookie Login (if browser login doesn't work)</summary>
        <div class="manual-content">
            <div class="instructions">
                <strong>Steps:</strong>
                <ol>
                    <li>Open LeetCode in your browser and log in</li>
                    <li>Press <code>F12</code> (or <code>Cmd+Option+I</code>) to open DevTools</li>
                    <li>Click <code>Application</code> tab &rarr; <code>Cookies</code> &rarr; select leetcode site</li>
                    <li>Copy the cookie values below</li>
                </ol>
            </div>

            <div class="form-group">
                <label for="session">LEETCODE_SESSION</label>
                <input type="text" id="session" placeholder="Paste LEETCODE_SESSION cookie value here">
                <div class="tip">Usually starts with "eyJ..."</div>
            </div>

            <div class="form-group">
                <label for="csrf">csrftoken</label>
                <input type="text" id="csrf" placeholder="Paste csrftoken cookie value here">
                <div class="tip">A shorter string</div>
            </div>

            <div class="form-group">
                <label for="proxy">Proxy (Optional)</label>
                <input type="text" id="proxy" placeholder="e.g., http://127.0.0.1:7890" value="${currentProxy}">
                <div class="tip">Required if you're behind a firewall or using VPN</div>
            </div>

            <div class="buttons">
                <button class="btn-primary" onclick="login()">Login with Cookies</button>
                <button class="btn-secondary" onclick="cancel()">Cancel</button>
            </div>
        </div>
    </details>

    <div id="error" class="error"></div>

    <script>
        const vscode = acquireVsCodeApi();

        function login() {
            const session = document.getElementById('session').value.trim();
            const csrf = document.getElementById('csrf').value.trim();
            const proxy = document.getElementById('proxy').value.trim();

            if (!session || !csrf) {
                showError('Please fill in both cookie fields');
                return;
            }

            vscode.postMessage({ command: 'login', session, csrf, proxy });
        }

        function browserLogin() {
            vscode.postMessage({ command: 'browserLogin' });
        }

        function cancel() {
            vscode.postMessage({ command: 'cancel' });
        }

        function showError(msg) {
            const el = document.getElementById('error');
            el.textContent = msg;
            el.style.display = 'block';
        }

        window.addEventListener('message', event => {
            if (event.data.command === 'error') {
                showError(event.data.message);
            }
        });

        // Allow Enter to submit in manual mode
        document.addEventListener('keypress', (e) => {
            if (e.key === 'Enter' && document.querySelector('details[open]')) {
                login();
            }
        });
    </script>
</body>
</html>`;
    }
}
