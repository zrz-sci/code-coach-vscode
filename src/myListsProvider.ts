import * as vscode from 'vscode';
import { LeetCodeService, Problem } from './leetcodeService';

export class MyListsProvider implements vscode.TreeDataProvider<ListItem> {
    private _onDidChangeTreeData: vscode.EventEmitter<ListItem | undefined | null | void> = new vscode.EventEmitter<ListItem | undefined | null | void>();
    readonly onDidChangeTreeData: vscode.Event<ListItem | undefined | null | void> = this._onDidChangeTreeData.event;

    constructor(private service: LeetCodeService) {}

    refresh(): void {
        this._onDidChangeTreeData.fire();
    }

    getTreeItem(element: ListItem): vscode.TreeItem {
        return element;
    }

    async getChildren(element?: ListItem): Promise<ListItem[]> {
        if (!this.service.getIsLoggedIn()) {
            return [new ListItem('Please login first', '', vscode.TreeItemCollapsibleState.None, 'info')];
        }

        if (!element) {
            // Root level - show lists
            try {
                const lists = await this.service.getMyLists();
                if (lists.length === 0) {
                    return [new ListItem('No lists found', '', vscode.TreeItemCollapsibleState.None, 'info')];
                }
                return lists.map(lst => {
                    const item = new ListItem(
                        lst.name,
                        lst.id,
                        vscode.TreeItemCollapsibleState.Collapsed,
                        'folder'
                    );
                    item.listId = lst.id;
                    return item;
                });
            } catch (e) {
                return [new ListItem('Failed to load lists', '', vscode.TreeItemCollapsibleState.None, 'error')];
            }
        } else if (element.listId) {
            // Show problems in this list
            try {
                const problems = await this.service.getFavoriteProblems(element.listId);
                return problems.map(p => {
                    const item = new ListItem(
                        `${p.id}. ${p.title}`,
                        p.difficulty,
                        vscode.TreeItemCollapsibleState.None,
                        'problem'
                    );
                    item.slug = p.titleSlug;
                    item.problemId = p.id;
                    item.status = p.status;
                    item.difficulty = p.difficulty;

                    // Set icon based on status
                    if (p.status === 'ac') {
                        item.iconPath = new vscode.ThemeIcon('check', new vscode.ThemeColor('testing.iconPassed'));
                    } else if (p.status === 'notac') {
                        item.iconPath = new vscode.ThemeIcon('circle-outline', new vscode.ThemeColor('testing.iconQueued'));
                    } else {
                        item.iconPath = new vscode.ThemeIcon('circle-outline');
                    }

                    item.description = p.difficulty;

                    item.command = {
                        command: 'leetcode.openProblemFromFavorite',
                        title: 'Open Problem',
                        arguments: [item]
                    };

                    return item;
                });
            } catch (e) {
                return [new ListItem('Failed to load problems', '', vscode.TreeItemCollapsibleState.None, 'error')];
            }
        }

        return [];
    }
}

export class ListItem extends vscode.TreeItem {
    listId?: string;
    slug?: string;
    problemId?: string;
    status?: string;
    difficulty?: string;

    constructor(
        public readonly label: string,
        public description: string,
        public readonly collapsibleState: vscode.TreeItemCollapsibleState,
        public readonly type: 'folder' | 'problem' | 'info' | 'error'
    ) {
        super(label, collapsibleState);

        if (type === 'folder') {
            this.iconPath = new vscode.ThemeIcon('library');
            this.contextValue = 'listFolder';
        } else if (type === 'info') {
            this.iconPath = new vscode.ThemeIcon('info');
        } else if (type === 'error') {
            this.iconPath = new vscode.ThemeIcon('error');
        }
    }
}
