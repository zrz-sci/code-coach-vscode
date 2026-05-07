import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';
import * as os from 'os';
import { LeetCodeService } from './leetcodeService';
import { SessionProvider } from './sessionProvider';
import { ProblemProvider } from './problemProvider';
import { SolutionProvider, SolutionItem } from './solutionProvider';
import { FavoritesProvider } from './favoritesProvider';
import { MyListsProvider } from './myListsProvider';
import { ProblemWebview } from './problemWebview';
import { LoginWebview } from './loginWebview';
import { ReferenceCodeLensProvider } from './referenceCodeLensProvider';

let leetcodeService: LeetCodeService;

// Shell argument escaping for terminal.sendText() commands
function shellEscape(arg: string): string {
    // Replace single quotes with escaped version, wrap in single quotes
    return "'" + arg.replace(/'/g, "'\\''") + "'";
}

// Validate problem slug format (only alphanumeric, hyphens, underscores, dots)
function isValidSlug(slug: string): boolean {
    return /^[a-zA-Z0-9][a-zA-Z0-9._-]{0,200}$/.test(slug);
}

// Validate file path is within expected problems directory
function isPathWithinDir(filePath: string, baseDir: string): boolean {
    const resolved = path.resolve(filePath);
    const resolvedBase = path.resolve(baseDir);
    return resolved.startsWith(resolvedBase + path.sep) || resolved === resolvedBase;
}
let sessionProvider: SessionProvider;
let problemProvider: ProblemProvider;
let solutionProvider: SolutionProvider;
let favoritesProvider: FavoritesProvider;
let myListsProvider: MyListsProvider;

// Mock Interview state
let mockState: {
    active: boolean;
    startTime: number;
    durationMs: number;
    problemId: string;
    problemTitle: string;
    solutionPath: string;
    timer: NodeJS.Timeout | null;
    statusBarItem: vscode.StatusBarItem | null;
} = {
    active: false, startTime: 0, durationMs: 0,
    problemId: '', problemTitle: '', solutionPath: '',
    timer: null, statusBarItem: null
};

// Helper: get API key from SecretStorage (preferred) or settings (legacy fallback)
async function getApiKey(context: vscode.ExtensionContext): Promise<string> {
    // 1. Try SecretStorage first
    const secret = await context.secrets.get('anthropicApiKey');
    if (secret) return secret;
    // 2. Fallback to legacy settings.json (auto-migrate if found)
    const legacy = vscode.workspace.getConfiguration('leetcode').get<string>('anthropicApiKey') || '';
    if (legacy) {
        // Migrate to SecretStorage and clear from settings
        await context.secrets.store('anthropicApiKey', legacy);
        await vscode.workspace.getConfiguration('leetcode').update('anthropicApiKey', undefined, true);
        return legacy;
    }
    return '';
}

// Helper: store API key in SecretStorage
async function setApiKey(context: vscode.ExtensionContext, key: string): Promise<void> {
    if (key) {
        await context.secrets.store('anthropicApiKey', key);
    } else {
        await context.secrets.delete('anthropicApiKey');
    }
    // Ensure settings.json is clean
    await vscode.workspace.getConfiguration('leetcode').update('anthropicApiKey', undefined, true);
}

export function activate(context: vscode.ExtensionContext) {
    console.log('Code Coach is now active!');

    // Initialize service
    leetcodeService = new LeetCodeService(context);

    // Check Python environment on first activation
    checkPythonEnvironment(context);

    // Initialize providers
    sessionProvider = new SessionProvider(leetcodeService);
    problemProvider = new ProblemProvider(leetcodeService);
    solutionProvider = new SolutionProvider(leetcodeService);
    favoritesProvider = new FavoritesProvider(leetcodeService);
    myListsProvider = new MyListsProvider(leetcodeService);

    // Register tree views
    vscode.window.registerTreeDataProvider('leetcode-session', sessionProvider);
    vscode.window.registerTreeDataProvider('leetcode-problems', problemProvider);
    vscode.window.registerTreeDataProvider('leetcode-favorites', favoritesProvider);
    vscode.window.registerTreeDataProvider('leetcode-my-lists', myListsProvider);
    vscode.window.registerTreeDataProvider('leetcode-my-solutions', solutionProvider);

    // Register CodeLens provider for reference links in solution.cpp
    const referenceLensProvider = new ReferenceCodeLensProvider();
    context.subscriptions.push(
        vscode.languages.registerCodeLensProvider(
            { pattern: '**/solution.cpp' },
            referenceLensProvider
        ),
        referenceLensProvider
    );

    // Register commands
    context.subscriptions.push(
        vscode.commands.registerCommand('leetcode.switchSite', async () => {
            const sites = [
                { label: '$(globe) LeetCode US', description: 'leetcode.com', value: 'us' },
                { label: '$(globe) 力扣中国', description: 'leetcode.cn', value: 'cn' }
            ];
            const selected = await vscode.window.showQuickPick(sites, {
                placeHolder: 'Select LeetCode Site / 选择网站'
            });
            if (selected) {
                await leetcodeService.switchSite(selected.value);
                sessionProvider.refresh();
                problemProvider.refresh();
                vscode.window.showInformationMessage(`Switched to ${selected.label}`);
            }
        }),

        vscode.commands.registerCommand('leetcode.login', async () => {
            // Step 1: Choose site
            const currentSite = leetcodeService.getCurrentSite();
            const siteChoice = await vscode.window.showQuickPick([
                { label: '$(globe) LeetCode US', description: 'leetcode.com', value: 'us',
                  detail: currentSite === 'us' ? '(current / 当前)' : undefined },
                { label: '$(globe) 力扣中国 (CN)', description: 'leetcode.cn', value: 'cn',
                  detail: currentSite === 'cn' ? '(current / 当前)' : undefined },
            ], { placeHolder: 'Step 1: Select site / 第一步: 选择站点' });

            if (!siteChoice) return;

            // Switch site if different
            if (siteChoice.value !== currentSite) {
                await leetcodeService.switchSite(siteChoice.value);
                sessionProvider.refresh();
            }

            // Step 2: Choose login method
            const methodChoice = await vscode.window.showQuickPick([
                { label: '$(sign-in) Login with Browser', description: 'Recommended / 推荐', value: 'browser' },
                { label: '$(key) Manual Cookie Login', description: 'Paste cookies from DevTools', value: 'cookie' },
            ], { placeHolder: 'Step 2: Select login method / 第二步: 选择登录方式' });

            if (!methodChoice) return;

            if (methodChoice.value === 'browser') {
                await vscode.commands.executeCommand('leetcode.browserLogin');
            } else {
                LoginWebview.show(context, async (session, csrf, proxy) => {
                    if (proxy) {
                        await vscode.workspace.getConfiguration('leetcode').update('proxy', proxy, true);
                    }
                    const success = await leetcodeService.login(session, csrf);
                    if (success) {
                        sessionProvider.refresh();
                        problemProvider.refresh();
                    }
                    return success;
                });
            }
        }),

        vscode.commands.registerCommand('leetcode.browserLogin', async () => {
            const pythonPath = vscode.workspace.getConfiguration('leetcode').get('pythonPath', 'python3');
            const toolPath = leetcodeService.getToolPath();
            const problemsDir = leetcodeService.getProblemsDir();
            const currentSite = leetcodeService.getCurrentSite();

            const siteName = currentSite === 'cn' ? '力扣 CN' : 'LeetCode US';
            const terminal = vscode.window.createTerminal({
                name: `Login (${siteName})`,
                cwd: path.dirname(toolPath)
            });
            terminal.show();
            terminal.sendText(`${pythonPath} "${toolPath}" --problems-dir "${problemsDir}" --site ${currentSite} login`);

            // Poll for login status every 3 seconds (up to 5 minutes)
            // This way user doesn't need to manually close the terminal.
            const maxAttempts = 100;  // 100 * 3s = 5 minutes
            let attempt = 0;
            const pollInterval = setInterval(async () => {
                attempt++;
                await leetcodeService.checkStatus();
                if (leetcodeService.getIsLoggedIn() || attempt >= maxAttempts) {
                    clearInterval(pollInterval);
                    if (leetcodeService.getIsLoggedIn()) {
                        sessionProvider.refresh();
                        problemProvider.refresh();
                        vscode.window.showInformationMessage('Login successful! 登录成功!');
                    }
                }
            }, 3000);

            // Also stop polling if terminal is closed
            const disposable = vscode.window.onDidCloseTerminal((closedTerminal) => {
                if (closedTerminal === terminal) {
                    disposable.dispose();
                    clearInterval(pollInterval);
                    // Final check
                    leetcodeService.checkStatus().then(() => {
                        sessionProvider.refresh();
                        problemProvider.refresh();
                    });
                }
            });
        }),

        vscode.commands.registerCommand('leetcode.settings', async () => {
            const currentHideTitle = vscode.workspace.getConfiguration('leetcode').get('hideProblemTitle', false);
            const settings = [
                { label: '$(globe) Display Language / 显示语言', description: 'Switch between Chinese and English', value: 'lang' },
                { label: currentHideTitle ? '$(eye) Show Problem Title / 显示题目标题' : '$(eye-closed) Hide Problem Title / 隐藏题目标题', description: currentHideTitle ? 'Currently hidden (blind practice mode)' : 'Hide title for blind practice', value: 'toggle-title' },
                { label: '$(cloud-download) Re-fetch All Favorites / 重新下载收藏夹', description: 'Fetch all favorite problems + generate AI reference if API key is set', value: 'refetch-favorites' },
                { label: '$(sync) Re-fetch Local Problems / 重新下载本地题目', description: 'Re-fetch all locally saved problems', value: 'refetch-local' },
                { label: '$(key) Set AI API Key / 设置 AI Key', description: 'Anthropic API key for generating reference solutions', value: 'apikey' },
            ];
            const choice = await vscode.window.showQuickPick(settings, {
                placeHolder: 'Select setting to change / 选择要修改的设置'
            });

            if (!choice) return;

            if (choice.value === 'toggle-title') {
                const newValue = !currentHideTitle;
                await vscode.workspace.getConfiguration('leetcode').update('hideProblemTitle', newValue, true);
                const msg = newValue ? 'Title hidden (blind practice mode) / 标题已隐藏（盲练模式）' : 'Title visible / 标题已显示';
                vscode.window.showInformationMessage(msg);
            } else if (choice.value === 'lang') {
                const langChoice = await vscode.window.showQuickPick([
                    { label: '$(symbol-string) 中文 (Chinese)', description: 'Show titles and descriptions in Chinese', value: 'cn' },
                    { label: '$(symbol-string) English', description: 'Show titles and descriptions in English', value: 'en' },
                ], { placeHolder: 'Select display language / 选择显示语言' });

                if (langChoice) {
                    await leetcodeService.setDisplayLanguage(langChoice.value);
                    const langName = langChoice.value === 'cn' ? '中文' : 'English';
                    vscode.window.showInformationMessage(`Display language set to ${langName}`);
                    problemProvider.forceRefresh();
                    sessionProvider.refresh();
                }
            } else if (choice.value === 'refetch-favorites') {
                const pythonPath = vscode.workspace.getConfiguration('leetcode').get('pythonPath', 'python3');
                const apiKey = await getApiKey(context);
                const scriptPath = path.join(path.dirname(leetcodeService.getToolPath()), 'fetch_and_generate.py');
                const problemsDir = leetcodeService.getProblemsDir();

                const terminalEnv: { [key: string]: string } = {};
                if (apiKey) {
                    terminalEnv['ANTHROPIC_API_KEY'] = apiKey;
                }

                const terminal = vscode.window.createTerminal({
                    name: 'Fetch Favorites',
                    cwd: path.dirname(scriptPath),
                    env: terminalEnv
                });
                terminal.show();

                if (apiKey) {
                    // Has API key: fetch + generate AI reference (key passed via env, not visible in terminal)
                    terminal.sendText(`${shellEscape(pythonPath)} ${shellEscape(scriptPath)}`);
                } else {
                    // No API key: fetch only, skip AI generation
                    terminal.sendText(`${shellEscape(pythonPath)} ${shellEscape(scriptPath)} --dry-run`);
                    vscode.window.showInformationMessage(
                        'No API key set. Fetching problems only (no AI reference). Set API key in Settings > Set AI API Key.'
                    );
                }

                const disposable = vscode.window.onDidCloseTerminal((closedTerminal) => {
                    if (closedTerminal === terminal) {
                        disposable.dispose();
                        solutionProvider.refresh();
                        problemProvider.forceRefresh();
                    }
                });
            } else if (choice.value === 'refetch-local') {
                // Show list of local problems for user to select
                const pythonPath = vscode.workspace.getConfiguration('leetcode').get('pythonPath', 'python3');
                const fs = require('fs');
                const problemsDir = leetcodeService.getProblemsDir();

                if (!fs.existsSync(problemsDir)) {
                    vscode.window.showInformationMessage('No local problems found.');
                    return;
                }

                // Read all local problems
                const dirs = fs.readdirSync(problemsDir, { withFileTypes: true })
                    .filter((d: any) => d.isDirectory())
                    .sort((a: any, b: any) => a.name.localeCompare(b.name));

                const items: { label: string; description: string; slug: string }[] = [];
                for (const dir of dirs) {
                    const infoPath = path.join(problemsDir, dir.name, 'problem_info.json');
                    if (fs.existsSync(infoPath)) {
                        try {
                            const info = JSON.parse(fs.readFileSync(infoPath, 'utf8'));
                            const hasBilingual = info.title_cn && info.title_en;
                            items.push({
                                label: `${info.id || '?'}. ${info.title || dir.name}`,
                                description: hasBilingual ? '$(check) bilingual' : '$(warning) needs re-fetch',
                                slug: info.titleSlug || '',
                            });
                        } catch (e) {
                            items.push({ label: dir.name, description: '$(warning) no info', slug: '' });
                        }
                    }
                }

                const selected = await vscode.window.showQuickPick(items, {
                    placeHolder: 'Select problems to re-fetch / 选择要重新下载的题目',
                    canPickMany: true,
                });

                if (!selected || selected.length === 0) return;

                // Fetch selected problems in terminal
                const slugs = selected.map(s => s.slug).filter(s => s && isValidSlug(s));
                if (slugs.length === 0) {
                    vscode.window.showErrorMessage('No valid problem slugs found.');
                    return;
                }
                const toolPath = leetcodeService.getToolPath();
                const terminal = vscode.window.createTerminal({
                    name: `Re-fetch ${selected.length} problems`,
                    cwd: path.dirname(toolPath)
                });
                terminal.show();

                const cmds = slugs.map(s => `${shellEscape(pythonPath)} ${shellEscape(toolPath)} --problems-dir ${shellEscape(problemsDir)} fetch ${shellEscape(s)}`).join(' && ');
                terminal.sendText(cmds);

                const disposable = vscode.window.onDidCloseTerminal((closedTerminal) => {
                    if (closedTerminal === terminal) {
                        disposable.dispose();
                        solutionProvider.refresh();
                        problemProvider.forceRefresh();
                    }
                });
            } else if (choice.value === 'apikey') {
                const currentKey = await getApiKey(context);
                const key = await vscode.window.showInputBox({
                    prompt: 'Enter Anthropic API Key (for AI-generated reference solutions)',
                    placeHolder: 'sk-ant-...',
                    value: currentKey,
                    password: true
                });
                if (key !== undefined) {
                    await setApiKey(context, key);
                    vscode.window.showInformationMessage(key ? 'API key saved! Re-fetch Favorites will now generate AI reference.' : 'API key cleared.');
                }
            }
        }),

        vscode.commands.registerCommand('leetcode.logout', async () => {
            await leetcodeService.logout();
            vscode.window.showInformationMessage('Logged out successfully.');
            sessionProvider.refresh();
        }),

        vscode.commands.registerCommand('leetcode.refresh', () => {
            problemProvider.forceRefresh();
            solutionProvider.refresh();
        }),

        vscode.commands.registerCommand('leetcode.fetchProblem', async () => {
            const input = await vscode.window.showInputBox({
                prompt: 'Enter problem ID or slug',
                placeHolder: 'e.g., 1 or two-sum'
            });
            if (input) {
                await vscode.window.withProgress({
                    location: vscode.ProgressLocation.Notification,
                    title: `Fetching problem: ${input}`,
                    cancellable: false
                }, async () => {
                    const result = await leetcodeService.fetchProblem(input);
                    if (result.success && result.filePath) {
                        const doc = await vscode.workspace.openTextDocument(result.filePath);
                        await vscode.window.showTextDocument(doc);
                        solutionProvider.refresh();
                        vscode.window.showInformationMessage(`Problem fetched: ${result.title}`);
                    } else {
                        vscode.window.showErrorMessage(result.error || 'Failed to fetch problem');
                    }
                });
            }
        }),

        vscode.commands.registerCommand('leetcode.searchProblem', async () => {
            const keyword = await vscode.window.showInputBox({
                prompt: 'Search problems',
                placeHolder: 'Enter keyword (e.g., binary tree, dynamic programming)'
            });
            if (keyword) {
                problemProvider.setSearchKeyword(keyword);
            }
        }),

        vscode.commands.registerCommand('leetcode.filterByDifficulty', async () => {
            const difficulties = [
                { label: '$(circle-filled) All', value: '' },
                { label: '$(check) Easy', value: 'easy' },
                { label: '$(warning) Medium', value: 'medium' },
                { label: '$(error) Hard', value: 'hard' }
            ];
            const selected = await vscode.window.showQuickPick(difficulties, {
                placeHolder: 'Filter by difficulty'
            });
            if (selected) {
                problemProvider.setDifficultyFilter(selected.value);
            }
        }),

        vscode.commands.registerCommand('leetcode.openProblem', async (item: any) => {
            console.log('[LeetCode] openProblem - item.id:', item?.id, 'item.slug:', item?.slug, 'item.label:', item?.label);
            if (item) {
                // Prefer slug over ID because slug-based API is more reliable
                const identifier = item.slug || item.id;
                console.log('[LeetCode] Using identifier:', identifier);
                if (!identifier) return;

                await vscode.window.withProgress({
                    location: vscode.ProgressLocation.Notification,
                    title: `Fetching problem: ${item.label}`,
                    cancellable: false
                }, async () => {
                    // Fetch and open solution file
                    const result = await leetcodeService.fetchProblem(identifier);
                    if (result.success && result.filePath) {
                        // First show problem description on the LEFT (Column One)
                        const problem = await leetcodeService.getProblemFromFile(result.filePath);
                        if (problem) {
                            ProblemWebview.show(context, problem, vscode.ViewColumn.One);
                        }

                        // Then open solution file on the RIGHT (Column Two)
                        const doc = await vscode.workspace.openTextDocument(result.filePath);
                        await vscode.window.showTextDocument(doc, vscode.ViewColumn.Two);
                        solutionProvider.refresh();
                    } else {
                        vscode.window.showErrorMessage(result.error || 'Failed to fetch problem');
                    }
                });
            }
        }),

        vscode.commands.registerCommand('leetcode.showProblemDescription', async (item: any) => {
            if (item && item.slug) {
                const problem = await leetcodeService.getProblemDetail(item.slug);
                if (problem) {
                    ProblemWebview.show(context, problem);
                }
            }
        }),

        vscode.commands.registerCommand('leetcode.testSolution', async () => {
            const editor = vscode.window.activeTextEditor;
            if (!editor) {
                vscode.window.showErrorMessage('No active editor');
                return;
            }

            const filePath = editor.document.fileName;
            if (!filePath.endsWith('.cpp')) {
                vscode.window.showErrorMessage('Please open a C++ solution file');
                return;
            }

            await editor.document.save();

            const pythonPath = vscode.workspace.getConfiguration('leetcode').get('pythonPath', 'python3');
            const toolPath = leetcodeService.getToolPath();
            const problemsDir = leetcodeService.getProblemsDir();
            const terminal = vscode.window.createTerminal('LeetCode Test');
            terminal.show();
            terminal.sendText(`${pythonPath} "${toolPath}" --problems-dir "${problemsDir}" test "${filePath}"`);
        }),

        vscode.commands.registerCommand('leetcode.runSolution', async () => {
            const editor = vscode.window.activeTextEditor;
            if (!editor) {
                vscode.window.showErrorMessage('No active editor');
                return;
            }

            const filePath = editor.document.fileName;
            await editor.document.save();

            const pythonPath = vscode.workspace.getConfiguration('leetcode').get('pythonPath', 'python3');
            const toolPath = leetcodeService.getToolPath();
            const problemsDir = leetcodeService.getProblemsDir();
            const terminal = vscode.window.createTerminal('LeetCode Run');
            terminal.show();
            terminal.sendText(`${pythonPath} "${toolPath}" --problems-dir "${problemsDir}" run "${filePath}"`);
        }),

        vscode.commands.registerCommand('leetcode.submitSolution', async () => {
            const editor = vscode.window.activeTextEditor;
            if (!editor) {
                vscode.window.showErrorMessage('No active editor');
                return;
            }

            const filePath = editor.document.fileName;
            await editor.document.save();

            const pythonPath = vscode.workspace.getConfiguration('leetcode').get('pythonPath', 'python3');
            const toolPath = leetcodeService.getToolPath();
            const problemsDir = leetcodeService.getProblemsDir();
            const terminal = vscode.window.createTerminal('LeetCode Submit');
            terminal.show();
            terminal.sendText(`${pythonPath} "${toolPath}" --problems-dir "${problemsDir}" submit "${filePath}"`);
        }),

        vscode.commands.registerCommand('leetcode.refreshFavorites', () => {
            favoritesProvider.forceRefresh();
        }),

        vscode.commands.registerCommand('leetcode.refreshMyLists', () => {
            myListsProvider.refresh();
        }),

        vscode.commands.registerCommand('leetcode.openProblemFromFavorite', async (item: any) => {
            console.log('[LeetCode] openProblemFromFavorite - item.problemId:', item?.problemId);
            console.log('[LeetCode] openProblemFromFavorite - item.label:', item?.label);
            if (item && item.problemId) {
                await vscode.window.withProgress({
                    location: vscode.ProgressLocation.Notification,
                    title: `Opening problem: ${item.label}`,
                    cancellable: false
                }, async () => {
                    const result = await leetcodeService.fetchProblem(item.problemId);
                    if (result.success && result.filePath) {
                        const doc = await vscode.workspace.openTextDocument(result.filePath);
                        await vscode.window.showTextDocument(doc, vscode.ViewColumn.One);
                        solutionProvider.refresh();

                        // Also show problem description in side panel
                        const problem = await leetcodeService.getProblemFromFile(result.filePath);
                        if (problem) {
                            ProblemWebview.show(context, problem);
                        }
                    } else {
                        vscode.window.showErrorMessage(result.error || 'Failed to fetch problem');
                    }
                });
            } else {
                vscode.window.showErrorMessage('No problem selected');
            }
        }),

        vscode.commands.registerCommand('leetcode.refreshSolutions', () => {
            solutionProvider.refresh();
        }),

        vscode.commands.registerCommand('leetcode.nextProblem', async () => {
            const next = leetcodeService.getNextProblem();
            if (!next) {
                vscode.window.showInformationMessage('All problems completed! 🎉');
                return;
            }
            const doc = await vscode.workspace.openTextDocument(next.solutionPath);
            await vscode.window.showTextDocument(doc, vscode.ViewColumn.Two);

            // Also show problem description
            const problem = await leetcodeService.getProblemFromFile(next.solutionPath);
            if (problem) {
                ProblemWebview.show(context, problem, vscode.ViewColumn.One);
            }
        }),

        vscode.commands.registerCommand('leetcode.openSolutionWithDescription', async (solutionPath: string) => {
            if (!solutionPath) { return; }
            const doc = await vscode.workspace.openTextDocument(solutionPath);
            await vscode.window.showTextDocument(doc, vscode.ViewColumn.Two);
            const problem = await leetcodeService.getProblemFromFile(solutionPath);
            if (problem) {
                ProblemWebview.show(context, problem, vscode.ViewColumn.One);
            }
        }),

        vscode.commands.registerCommand('leetcode.startMock', async () => {
            if (mockState.active) {
                vscode.window.showWarningMessage('Mock interview already in progress. End it first.');
                return;
            }

            // Step 1: Choose difficulty
            const diffChoice = await vscode.window.showQuickPick([
                { label: '$(symbol-event) Random', description: 'Any difficulty', value: '' },
                { label: '$(check) Easy', value: 'Easy' },
                { label: '$(warning) Medium', value: 'Medium' },
                { label: '$(error) Hard', value: 'Hard' },
            ], { placeHolder: 'Select difficulty / 选择难度' });
            if (!diffChoice) return;

            // Step 2: Choose time limit
            const timeChoice = await vscode.window.showQuickPick([
                { label: '20 minutes', value: 20 },
                { label: '30 minutes', value: 30 },
                { label: '45 minutes (Recommended)', value: 45 },
                { label: '60 minutes', value: 60 },
            ], { placeHolder: 'Select time limit / 选择时间' });
            if (!timeChoice) return;

            // Step 3: Pick random problem
            const problem = leetcodeService.getRandomProblem(diffChoice.value || undefined);
            if (!problem) {
                vscode.window.showWarningMessage('No matching problems found. Try a different difficulty.');
                return;
            }

            // Step 4: Start mock
            mockState.active = true;
            mockState.startTime = Date.now();
            mockState.durationMs = timeChoice.value * 60 * 1000;
            mockState.problemId = problem.id;
            mockState.problemTitle = problem.title;
            mockState.solutionPath = problem.solutionPath;

            // Hide reference during mock
            solutionProvider.setMockMode(true);

            // Create status bar timer
            mockState.statusBarItem = vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Left, 100);
            mockState.statusBarItem.command = 'leetcode.endMock';
            mockState.statusBarItem.tooltip = 'Click to end mock interview';
            mockState.statusBarItem.show();

            // Update timer every second
            const updateTimer = () => {
                if (!mockState.active || !mockState.statusBarItem) return;
                const elapsed = Date.now() - mockState.startTime;
                const remaining = Math.max(0, mockState.durationMs - elapsed);
                const mins = Math.floor(remaining / 60000);
                const secs = Math.floor((remaining % 60000) / 1000);
                mockState.statusBarItem.text = `$(clock) Mock: ${mins}:${secs.toString().padStart(2, '0')}  #${problem.id}`;

                if (remaining <= 0) {
                    vscode.window.showWarningMessage(`Time's up! Mock interview for #${mockState.problemId} ended.`, 'View Results').then(choice => {
                        if (choice) {
                            vscode.commands.executeCommand('leetcode.endMock');
                        }
                    });
                    if (mockState.timer) clearInterval(mockState.timer);
                }
            };
            updateTimer();
            mockState.timer = setInterval(updateTimer, 1000);

            // Open the problem
            const doc = await vscode.workspace.openTextDocument(problem.solutionPath);
            await vscode.window.showTextDocument(doc, vscode.ViewColumn.Two);
            const problemDetail = await leetcodeService.getProblemFromFile(problem.solutionPath);
            if (problemDetail) {
                ProblemWebview.show(context, problemDetail, vscode.ViewColumn.One);
            }

            vscode.window.showInformationMessage(
                `Mock started: #${problem.id}. ${problem.title} [${problem.difficulty}] — ${timeChoice.value} min. Reference hidden.`
            );
        }),

        vscode.commands.registerCommand('leetcode.endMock', async () => {
            if (!mockState.active) {
                vscode.window.showInformationMessage('No mock interview in progress.');
                return;
            }

            // Stop timer
            if (mockState.timer) {
                clearInterval(mockState.timer);
                mockState.timer = null;
            }
            if (mockState.statusBarItem) {
                mockState.statusBarItem.dispose();
                mockState.statusBarItem = null;
            }

            // Calculate elapsed time
            const elapsedMs = Date.now() - mockState.startTime;
            const mins = Math.floor(elapsedMs / 60000);
            const secs = Math.floor((elapsedMs % 60000) / 1000);

            // Check if problem was passed
            const infoPath = path.join(path.dirname(mockState.solutionPath), 'problem_info.json');
            let passed = false;
            try {
                const info = JSON.parse(fs.readFileSync(infoPath, 'utf-8'));
                passed = info.progress?.status === 'passed';
            } catch {
                // ignore
            }

            const result = passed ? '✅ Accepted' : '❌ Not submitted / Not accepted';

            // Restore reference
            mockState.active = false;
            solutionProvider.setMockMode(false);
            sessionProvider.refresh();

            // Show results
            vscode.window.showInformationMessage(
                `Mock Result: #${mockState.problemId}. ${mockState.problemTitle}\n` +
                `Time: ${mins}m ${secs}s | ${result}`,
                'Open Reference'
            ).then(choice => {
                if (choice === 'Open Reference') {
                    const dir = path.dirname(mockState.solutionPath);
                    const refNewPath = path.join(dir, 'reference_new.cpp');
                    const refOldPath = path.join(dir, 'reference.cpp');
                    const refPath = fs.existsSync(refNewPath) ? refNewPath : refOldPath;
                    if (fs.existsSync(refPath)) {
                        vscode.workspace.openTextDocument(refPath).then(doc => {
                            vscode.window.showTextDocument(doc, vscode.ViewColumn.Beside);
                        });
                    }
                }
            });
        }),

        vscode.commands.registerCommand('leetcode.resetSolution', async (item: any) => {
            // Support both SolutionItem (has filePath) and FavoriteItem (has id/slug)
            let filePath = item?.filePath;
            let label = item?.label || '';

            if (!filePath && item?.id) {
                // From Favorites - find the solution file by ID
                const found = leetcodeService.findExistingProblem(item.id);
                if (!found) {
                    vscode.window.showErrorMessage('Solution not found locally. Fetch the problem first.');
                    return;
                }
                filePath = found;
            }

            if (!filePath) {
                vscode.window.showErrorMessage('No solution selected');
                return;
            }

            const confirm = await vscode.window.showWarningMessage(
                `Reset solution: ${label}? Your code will be cleared back to the initial template. Reference will be kept.`,
                'Yes, Reset', 'Cancel'
            );

            if (confirm === 'Yes, Reset') {
                const result = await leetcodeService.resetSolution(filePath);
                if (result.success && result.filePath) {
                    vscode.window.showInformationMessage(`Solution reset: ${label}`);
                    solutionProvider.refresh();
                    // Open the fresh template
                    const doc = await vscode.workspace.openTextDocument(result.filePath);
                    await vscode.window.showTextDocument(doc);
                } else {
                    vscode.window.showErrorMessage(`Failed to reset: ${result.error || 'Unknown error'}`);
                }
            }
        }),

        vscode.commands.registerCommand('leetcode.clearSolution', async (item: SolutionItem) => {
            if (!item || !item.filePath) {
                vscode.window.showErrorMessage('No solution selected');
                return;
            }

            const confirm = await vscode.window.showWarningMessage(
                `Delete solution: ${item.label}? This action cannot be undone.`,
                'Yes, Delete', 'Cancel'
            );

            if (confirm === 'Yes, Delete') {
                const success = leetcodeService.clearSolution(item.filePath);
                if (success) {
                    vscode.window.showInformationMessage(`Deleted: ${item.label}`);
                    solutionProvider.refresh();
                } else {
                    vscode.window.showErrorMessage('Failed to delete solution');
                }
            }
        }),

        vscode.commands.registerCommand('leetcode.clearAllSolutions', async () => {
            const confirm = await vscode.window.showWarningMessage(
                'Delete ALL solutions? This action cannot be undone!',
                { modal: true },
                'Yes, Delete All'
            );

            if (confirm === 'Yes, Delete All') {
                const count = leetcodeService.clearAllSolutions();
                vscode.window.showInformationMessage(`Deleted ${count} solutions`);
                solutionProvider.refresh();
            }
        }),

        vscode.commands.registerCommand('leetcode.generateHints', async (item: any) => {
            const apiKey = await getApiKey(context);
            if (!apiKey) {
                const choice = await vscode.window.showWarningMessage(
                    'Anthropic API key required for AI reference. Set it now?',
                    'Set API Key', 'Cancel'
                );
                if (choice === 'Set API Key') {
                    await vscode.commands.executeCommand('leetcode.settings');
                }
                return;
            }

            // Determine the target path: from tree item, or from active editor
            let targetPath = item?.filePath;
            if (!targetPath && item?.id) {
                // From Favorites - find local solution by ID
                targetPath = leetcodeService.findExistingProblem(item.id);
            }
            if (!targetPath) {
                const editor = vscode.window.activeTextEditor;
                if (editor && editor.document.fileName.endsWith('.cpp')) {
                    targetPath = editor.document.fileName;
                }
            }
            if (!targetPath) {
                vscode.window.showErrorMessage('No solution selected. Open a solution file or click from My Solutions.');
                return;
            }

            const pythonPath = vscode.workspace.getConfiguration('leetcode').get('pythonPath', 'python3');
            const toolPath = leetcodeService.getToolPath();
            const problemsDir = leetcodeService.getProblemsDir();

            const terminal = vscode.window.createTerminal({
                name: 'Generate Reference',
                cwd: path.dirname(toolPath),
                env: { 'ANTHROPIC_API_KEY': apiKey }
            });
            terminal.show();
            terminal.sendText(`${shellEscape(pythonPath)} ${shellEscape(toolPath)} --problems-dir ${shellEscape(problemsDir)} generate-reference ${shellEscape(targetPath)}`);

            // Refresh solution tree when terminal closes
            const disposable = vscode.window.onDidCloseTerminal((closedTerminal) => {
                if (closedTerminal === terminal) {
                    disposable.dispose();
                    solutionProvider.refresh();
                }
            });
        }),

        vscode.commands.registerCommand('leetcode.addToFavorite', async (item: any) => {
            if (!item || !item.id) {
                vscode.window.showErrorMessage('No problem selected.');
                return;
            }

            const problemId = item.id;
            const problemLabel = item.label || `Problem #${problemId}`;

            // Fetch favorite folders
            const favorites = await leetcodeService.getFavorites();
            if (!favorites || favorites.length === 0) {
                const create = await vscode.window.showWarningMessage(
                    'No favorite folders found. Create one?',
                    'Create New', 'Cancel'
                );
                if (create !== 'Create New') return;

                const name = await vscode.window.showInputBox({
                    prompt: 'Enter name for new favorite folder / 输入收藏夹名称',
                    placeHolder: 'e.g., Interview Prep'
                });
                if (!name) return;

                const createResult = await leetcodeService.createFavorite(name);
                if (!createResult.success || !createResult.id) {
                    vscode.window.showErrorMessage(`Failed to create favorite: ${createResult.error}`);
                    return;
                }

                await vscode.window.withProgress({
                    location: vscode.ProgressLocation.Notification,
                    title: `Adding ${problemLabel} to ${name}...`
                }, async () => {
                    const result = await leetcodeService.addToFavorite(createResult.id!, problemId);
                    if (result.success) {
                        vscode.window.showInformationMessage(`Added ${problemLabel} to "${name}"`);
                        favoritesProvider.forceRefresh();
                    } else {
                        vscode.window.showErrorMessage(`Failed: ${result.error}`);
                    }
                });
                return;
            }

            // Build QuickPick options
            const options = [
                ...favorites.map(f => ({
                    label: `$(folder) ${f.name}`,
                    description: f.id,
                    value: f.id
                })),
                {
                    label: '$(add) Create New Favorite / 新建收藏夹',
                    description: '',
                    value: '__create_new__'
                }
            ];

            const selected = await vscode.window.showQuickPick(options, {
                placeHolder: `Add "${problemLabel}" to which favorite? / 加入哪个收藏夹？`
            });

            if (!selected) return;

            let targetId = selected.value;
            let targetName = selected.label;

            if (targetId === '__create_new__') {
                const name = await vscode.window.showInputBox({
                    prompt: 'Enter name for new favorite folder / 输入收藏夹名称',
                    placeHolder: 'e.g., Interview Prep'
                });
                if (!name) return;

                const createResult = await leetcodeService.createFavorite(name);
                if (!createResult.success || !createResult.id) {
                    vscode.window.showErrorMessage(`Failed to create favorite: ${createResult.error}`);
                    return;
                }
                targetId = createResult.id;
                targetName = name;
            }

            await vscode.window.withProgress({
                location: vscode.ProgressLocation.Notification,
                title: `Adding ${problemLabel}...`
            }, async () => {
                const result = await leetcodeService.addToFavorite(targetId, problemId);
                if (result.success) {
                    vscode.window.showInformationMessage(`Added ${problemLabel} to "${targetName}"`);
                    favoritesProvider.forceRefresh();
                } else {
                    vscode.window.showErrorMessage(`Failed: ${result.error}`);
                }
            });
        })
    );

    // Watch for problem_info.json changes → auto-refresh UI after submit
    const problemsDir = leetcodeService.getProblemsDir();
    // Ensure problemsDir exists so watcher can be created for new users
    if (!fs.existsSync(problemsDir)) {
        fs.mkdirSync(problemsDir, { recursive: true });
    }
    const watcher = vscode.workspace.createFileSystemWatcher(
        new vscode.RelativePattern(problemsDir, '**/problem_info.json')
    );
    let debounceTimer: NodeJS.Timeout | undefined;
    const debouncedRefresh = () => {
        if (debounceTimer) clearTimeout(debounceTimer);
        debounceTimer = setTimeout(() => {
            solutionProvider.refresh();
            sessionProvider.refresh();
        }, 500);
    };
    watcher.onDidChange(debouncedRefresh);
    watcher.onDidCreate(debouncedRefresh);
    context.subscriptions.push(watcher);

    // Initial refresh
    sessionProvider.refresh();
}

export function deactivate() {}

async function checkPythonEnvironment(context: vscode.ExtensionContext): Promise<void> {
    const cp = require('child_process');
    const pythonPath = vscode.workspace.getConfiguration('leetcode').get('pythonPath', 'python3');

    // Check if Python is available
    try {
        cp.execFileSync(pythonPath, ['--version'], { timeout: 5000 });
    } catch {
        vscode.window.showWarningMessage(
            'Python 3 is required for Code Coach. Please install Python 3 and set the path in settings.',
            'Open Settings'
        ).then(choice => {
            if (choice === 'Open Settings') {
                vscode.commands.executeCommand('workbench.action.openSettings', 'leetcode.pythonPath');
            }
        });
        return;
    }

    // Check if required pip packages are installed
    try {
        cp.execFileSync(pythonPath, ['-c', 'import requests; import html2text'], { timeout: 5000 });
    } catch {
        const requirementsPath = path.join(context.extensionPath, 'python', 'requirements.txt');
        vscode.window.showWarningMessage(
            'Missing Python dependencies. Install required packages?',
            'Install Now'
        ).then(choice => {
            if (choice === 'Install Now') {
                const terminal = vscode.window.createTerminal('Install Dependencies');
                terminal.show();
                terminal.sendText(`${pythonPath} -m pip install -r "${requirementsPath}"`);
            }
        });
    }
}
