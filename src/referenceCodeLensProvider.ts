import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';

export class ReferenceCodeLensProvider implements vscode.CodeLensProvider {
    private _onDidChangeCodeLenses: vscode.EventEmitter<void> = new vscode.EventEmitter<void>();
    readonly onDidChangeCodeLenses: vscode.Event<void> = this._onDidChangeCodeLenses.event;

    private watcher1: vscode.FileSystemWatcher;
    private watcher2: vscode.FileSystemWatcher;

    constructor() {
        this.watcher1 = vscode.workspace.createFileSystemWatcher('**/reference_new.cpp');
        this.watcher2 = vscode.workspace.createFileSystemWatcher('**/reference.cpp');
        this.watcher1.onDidCreate(() => this._onDidChangeCodeLenses.fire());
        this.watcher1.onDidDelete(() => this._onDidChangeCodeLenses.fire());
        this.watcher2.onDidCreate(() => this._onDidChangeCodeLenses.fire());
        this.watcher2.onDidDelete(() => this._onDidChangeCodeLenses.fire());
    }

    provideCodeLenses(document: vscode.TextDocument): vscode.CodeLens[] {
        if (!document.fileName.endsWith('solution.cpp')) {
            return [];
        }

        const dir = path.dirname(document.fileName);
        // Prefer reference_new.cpp, fallback to reference.cpp
        const refNewPath = path.join(dir, 'reference_new.cpp');
        const refOldPath = path.join(dir, 'reference.cpp');
        const refPath = fs.existsSync(refNewPath) ? refNewPath : refOldPath;
        const hasRef = fs.existsSync(refPath);

        const lenses: vscode.CodeLens[] = [];
        const range = new vscode.Range(0, 0, 0, 0);

        if (hasRef) {
            lenses.push(new vscode.CodeLens(range, {
                title: '$(code) View Reference',
                command: 'vscode.open',
                arguments: [vscode.Uri.file(refPath)]
            }));
        } else {
            const apiKey = vscode.workspace.getConfiguration('leetcode').get<string>('anthropicApiKey');
            if (apiKey) {
                lenses.push(new vscode.CodeLens(range, {
                    title: '$(sparkle) Generate Reference',
                    command: 'leetcode.generateHints'
                }));
            }
        }

        return lenses;
    }

    dispose(): void {
        this.watcher1.dispose();
        this.watcher2.dispose();
    }
}
