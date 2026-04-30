import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';
import { LeetCodeService } from './leetcodeService';

interface ReviewProblem {
    id: string;
    title: string;
    difficulty: string;
    solutionPath: string;
    nextReviewTime: string;
}

export class SessionProvider implements vscode.TreeDataProvider<SessionItem> {
    private _onDidChangeTreeData: vscode.EventEmitter<SessionItem | undefined | null | void> = new vscode.EventEmitter<SessionItem | undefined | null | void>();
    readonly onDidChangeTreeData: vscode.Event<SessionItem | undefined | null | void> = this._onDidChangeTreeData.event;

    constructor(private service: LeetCodeService) {}

    refresh(): void {
        this.service.checkStatus().then(() => {
            this._onDidChangeTreeData.fire();
        });
    }

    getTreeItem(element: SessionItem): vscode.TreeItem {
        return element;
    }

    getChildren(element?: SessionItem): Thenable<SessionItem[]> {
        if (element) {
            // Expand "Today's Review" to show individual problems
            if (element.contextValue === 'reviewParent') {
                return Promise.resolve(this.getReviewChildren());
            }
            return Promise.resolve([]);
        }

        const items: SessionItem[] = [];

        // === Progress & Review section (at top) ===
        const stats = this.scanProgress();

        if (stats.total > 0) {
            const pct = ((stats.passed / stats.total) * 100).toFixed(1);
            const progressItem = new SessionItem(
                `Progress: ${stats.passed}/${stats.total} (${pct}%)`,
                `${stats.attempted} in progress`,
                vscode.TreeItemCollapsibleState.None
            );
            progressItem.iconPath = new vscode.ThemeIcon('graph');
            items.push(progressItem);
        }

        // Next problem recommendation
        const next = this.service.getNextProblem();
        if (next) {
            const nextItem = new SessionItem(
                `Next: #${next.id}. ${next.title}`,
                next.difficulty,
                vscode.TreeItemCollapsibleState.None
            );
            nextItem.iconPath = new vscode.ThemeIcon('play', new vscode.ThemeColor('charts.blue'));
            nextItem.command = {
                command: 'leetcode.nextProblem',
                title: 'Open Next Problem'
            };
            items.push(nextItem);
        }

        if (stats.reviewDue > 0) {
            const reviewItem = new SessionItem(
                `Review: ${stats.reviewDue} due today`,
                'Click to expand',
                vscode.TreeItemCollapsibleState.Collapsed
            );
            reviewItem.iconPath = new vscode.ThemeIcon('calendar', new vscode.ThemeColor('charts.orange'));
            reviewItem.contextValue = 'reviewParent';
            items.push(reviewItem);
        } else if (stats.passed > 0) {
            const reviewItem = new SessionItem(
                'Review: 0 due today',
                '',
                vscode.TreeItemCollapsibleState.None
            );
            reviewItem.iconPath = new vscode.ThemeIcon('calendar');
            items.push(reviewItem);
        }

        // === Separator ===
        if (stats.total > 0) {
            const sep = new SessionItem('', '', vscode.TreeItemCollapsibleState.None);
            items.push(sep);
        }

        // Site info
        const siteName = this.service.getSiteName();
        const siteItem = new SessionItem(
            `Site: ${siteName}`,
            '',
            vscode.TreeItemCollapsibleState.None
        );
        siteItem.iconPath = new vscode.ThemeIcon('globe');
        siteItem.command = {
            command: 'leetcode.switchSite',
            title: 'Switch Site'
        };
        items.push(siteItem);

        // Login status
        if (this.service.getIsLoggedIn()) {
            const userItem = new SessionItem(
                `User: ${this.service.getUsername()}`,
                'Logged in',
                vscode.TreeItemCollapsibleState.None
            );
            userItem.iconPath = new vscode.ThemeIcon('account');
            items.push(userItem);

            const logoutItem = new SessionItem(
                'Logout',
                '',
                vscode.TreeItemCollapsibleState.None
            );
            logoutItem.iconPath = new vscode.ThemeIcon('sign-out');
            logoutItem.command = {
                command: 'leetcode.logout',
                title: 'Logout'
            };
            items.push(logoutItem);
        } else {
            const loginItem = new SessionItem(
                'Click to Login',
                'Not logged in',
                vscode.TreeItemCollapsibleState.None
            );
            loginItem.iconPath = new vscode.ThemeIcon('sign-in');
            loginItem.command = {
                command: 'leetcode.login',
                title: 'Login'
            };
            items.push(loginItem);
        }

        // Quick actions
        const settingsItem = new SessionItem(
            'Settings',
            'Display language, proxy, etc.',
            vscode.TreeItemCollapsibleState.None
        );
        settingsItem.iconPath = new vscode.ThemeIcon('gear');
        settingsItem.command = {
            command: 'leetcode.settings',
            title: 'Settings'
        };
        items.push(settingsItem);

        const fetchItem = new SessionItem(
            'Fetch Problem',
            'Download a problem by ID or slug',
            vscode.TreeItemCollapsibleState.None
        );
        fetchItem.iconPath = new vscode.ThemeIcon('cloud-download');
        fetchItem.command = {
            command: 'leetcode.fetchProblem',
            title: 'Fetch Problem'
        };
        items.push(fetchItem);

        return Promise.resolve(items);
    }

    private scanProgress(): { total: number; passed: number; attempted: number; reviewDue: number; reviewProblems: ReviewProblem[] } {
        const result = { total: 0, passed: 0, attempted: 0, reviewDue: 0, reviewProblems: [] as ReviewProblem[] };
        const problemsDir = this.service.getProblemsDir();

        if (!fs.existsSync(problemsDir)) {
            return result;
        }

        const today = new Date();
        today.setHours(23, 59, 59, 999); // Include all of today

        try {
            const dirs = fs.readdirSync(problemsDir, { withFileTypes: true })
                .filter(d => d.isDirectory());

            for (const dir of dirs) {
                const solutionPath = path.join(problemsDir, dir.name, 'solution.cpp');
                const infoPath = path.join(problemsDir, dir.name, 'problem_info.json');

                if (!fs.existsSync(solutionPath)) continue;
                result.total++;

                if (!fs.existsSync(infoPath)) continue;

                try {
                    const info = JSON.parse(fs.readFileSync(infoPath, 'utf-8'));
                    if (!info.progress) continue;

                    if (info.progress.status === 'passed') {
                        result.passed++;
                        // Check if review is due
                        if (info.progress.nextReviewTime) {
                            const dueDate = new Date(info.progress.nextReviewTime);
                            if (dueDate <= today) {
                                result.reviewDue++;
                                result.reviewProblems.push({
                                    id: info.id || '',
                                    title: info.title || dir.name,
                                    difficulty: info.difficulty || '',
                                    solutionPath,
                                    nextReviewTime: info.progress.nextReviewTime
                                });
                            }
                        }
                    } else if (info.progress.status === 'attempted') {
                        result.attempted++;
                    }
                } catch {
                    // Skip unparseable files
                }
            }
        } catch {
            // Skip on error
        }

        // Sort review problems by due date (oldest first)
        result.reviewProblems.sort((a, b) =>
            new Date(a.nextReviewTime).getTime() - new Date(b.nextReviewTime).getTime()
        );

        return result;
    }

    private getReviewChildren(): SessionItem[] {
        const stats = this.scanProgress();
        return stats.reviewProblems.map(p => {
            const diffIcon = p.difficulty === 'Easy' ? 'charts.green' :
                             p.difficulty === 'Medium' ? 'charts.yellow' : 'charts.red';
            const item = new SessionItem(
                `${p.id}. ${p.title}`,
                p.difficulty,
                vscode.TreeItemCollapsibleState.None
            );
            item.iconPath = new vscode.ThemeIcon('pass-filled', new vscode.ThemeColor(diffIcon));
            item.command = {
                command: 'leetcode.openSolutionWithDescription',
                title: 'Open Problem',
                arguments: [p.solutionPath]
            };
            return item;
        });
    }
}

class SessionItem extends vscode.TreeItem {
    constructor(
        public readonly label: string,
        public readonly description: string,
        public readonly collapsibleState: vscode.TreeItemCollapsibleState
    ) {
        super(label, collapsibleState);
        this.tooltip = this.description;
    }
}
