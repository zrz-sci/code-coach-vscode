import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';
import * as os from 'os';
import { LeetCodeService } from './leetcodeService';

export class SolutionProvider implements vscode.TreeDataProvider<SolutionItem> {
    private _onDidChangeTreeData: vscode.EventEmitter<SolutionItem | undefined | null | void> = new vscode.EventEmitter<SolutionItem | undefined | null | void>();
    readonly onDidChangeTreeData: vscode.Event<SolutionItem | undefined | null | void> = this._onDidChangeTreeData.event;
    private mockMode: boolean = false;

    constructor(private service: LeetCodeService) {}

    refresh(): void {
        this._onDidChangeTreeData.fire();
    }

    setMockMode(active: boolean): void {
        this.mockMode = active;
        this.refresh();
    }

    isMockMode(): boolean {
        return this.mockMode;
    }

    getTreeItem(element: SolutionItem): vscode.TreeItem {
        return element;
    }

    async getChildren(element?: SolutionItem): Promise<SolutionItem[]> {
        if (element && element.filePath) {
            // Show child files for an expanded solution
            const dir = path.dirname(element.filePath);
            const children: SolutionItem[] = [];
            // Prefer reference_new.cpp, fallback to reference.cpp
            const refFile = fs.existsSync(path.join(dir, 'reference_new.cpp')) ? 'reference_new.cpp' : 'reference.cpp';
            const childFiles = this.mockMode
                ? ['testcases.txt']
                : [refFile, 'testcases.txt'];

            for (const name of childFiles) {
                const filePath = path.join(dir, name);
                if (fs.existsSync(filePath)) {
                    const icon = name.endsWith('.md') ? 'book' :
                                 name.endsWith('.cpp') ? 'code' : 'list-unordered';
                    const child = new SolutionItem(name, '', filePath, vscode.TreeItemCollapsibleState.None);
                    child.iconPath = new vscode.ThemeIcon(icon);
                    child.command = {
                        command: 'vscode.open',
                        title: 'Open',
                        arguments: [vscode.Uri.file(filePath)]
                    };
                    children.push(child);
                }
            }
            return children;
        }

        if (element) {
            return [];
        }

        const problemsDir = this.service.getProblemsDir();
        if (!fs.existsSync(problemsDir)) {
            return [new SolutionItem('No solutions yet', '', '', vscode.TreeItemCollapsibleState.None)];
        }

        const items: SolutionItem[] = [];

        try {
            const dirs = fs.readdirSync(problemsDir, { withFileTypes: true })
                .filter(d => d.isDirectory())
                .sort((a, b) => {
                    // Sort by problem number
                    const numA = parseInt(a.name.split('_')[0]) || 0;
                    const numB = parseInt(b.name.split('_')[0]) || 0;
                    return numA - numB;
                });

            for (const dir of dirs) {
                const solutionPath = path.join(problemsDir, dir.name, 'solution.cpp');
                const infoPath = path.join(problemsDir, dir.name, 'problem_info.json');

                if (fs.existsSync(solutionPath)) {
                    let title = dir.name;
                    let difficulty = '';
                    let progressStatus = 'untouched';
                    let nextReviewTime = '';

                    // Try to read problem info
                    if (fs.existsSync(infoPath)) {
                        try {
                            const info = JSON.parse(fs.readFileSync(infoPath, 'utf-8'));
                            title = `${info.id}. ${info.title}`;
                            difficulty = info.difficulty;
                            if (info.progress) {
                                progressStatus = info.progress.status || 'untouched';
                                nextReviewTime = info.progress.nextReviewTime || '';
                            }
                        } catch {
                            // Use directory name if can't parse info
                        }
                    }

                    // Build description: difficulty + review status
                    let desc = difficulty;
                    if (progressStatus === 'passed' && nextReviewTime) {
                        const dueDate = new Date(nextReviewTime);
                        const today = new Date();
                        today.setHours(0, 0, 0, 0);
                        if (dueDate <= today) {
                            desc = `${difficulty} · Review due`;
                        }
                    }

                    const item = new SolutionItem(
                        title,
                        desc,
                        solutionPath,
                        vscode.TreeItemCollapsibleState.Collapsed
                    );

                    // Set icon based on progress status (overrides difficulty icon)
                    switch (progressStatus) {
                        case 'passed':
                            item.iconPath = new vscode.ThemeIcon('pass-filled', new vscode.ThemeColor('charts.green'));
                            break;
                        case 'attempted':
                            item.iconPath = new vscode.ThemeIcon('warning', new vscode.ThemeColor('charts.yellow'));
                            break;
                        default:
                            // Untouched: show difficulty-based icon
                            switch (difficulty) {
                                case 'Easy':
                                    item.iconPath = new vscode.ThemeIcon('circle-outline', new vscode.ThemeColor('charts.green'));
                                    break;
                                case 'Medium':
                                    item.iconPath = new vscode.ThemeIcon('circle-outline', new vscode.ThemeColor('charts.yellow'));
                                    break;
                                case 'Hard':
                                    item.iconPath = new vscode.ThemeIcon('circle-outline', new vscode.ThemeColor('charts.red'));
                                    break;
                                default:
                                    item.iconPath = new vscode.ThemeIcon('circle-outline');
                            }
                    }

                    item.command = {
                        command: 'vscode.open',
                        title: 'Open Solution',
                        arguments: [vscode.Uri.file(solutionPath)]
                    };

                    item.contextValue = 'solution';
                    items.push(item);
                }
            }
        } catch (e) {
            return [new SolutionItem('Error reading solutions', '', '', vscode.TreeItemCollapsibleState.None)];
        }

        if (items.length === 0) {
            return [new SolutionItem('No solutions yet. Fetch a problem to start!', '', '', vscode.TreeItemCollapsibleState.None)];
        }

        return items;
    }
}

export class SolutionItem extends vscode.TreeItem {
    constructor(
        public readonly label: string,
        public readonly description: string,
        public readonly filePath: string,
        public readonly collapsibleState: vscode.TreeItemCollapsibleState
    ) {
        super(label, collapsibleState);
        this.tooltip = filePath || label;
    }
}
