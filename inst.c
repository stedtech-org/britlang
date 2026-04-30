#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void install_extension() {
    // ユーザーのホームディレクトリを取得
    char* userprofile = getenv("USERPROFILE");
    char ext_path[MAX_PATH];
    
    // VS Code拡張機能フォルダのパスを作成
    sprintf(ext_path, "%s\\.vscode\\extensions\\britlang-syntax", userprofile);
    
    // フォルダ作成コマンド（既にあったら無視）
    char cmd[1024];
    sprintf(cmd, "mkdir \"%s\" 2>nul", ext_path);
    system(cmd);

    // 20行目の修正：一度 sprintf でコマンドを完成させてから system に渡す
    sprintf(cmd, "xcopy /y /e .\\britlang-syntax \"%s\\\"", ext_path);
    system(cmd);

    printf("[1/3] VS Code Extension installed.\n");
}

void set_environment_variable() {
    // 実行ファイルを C:\BritLang にコピーして Path を通す
    system("mkdir C:\\BritLang 2>nul");
    system("copy /y britlang.exe C:\\BritLang\\");

    // ユーザー環境変数 Path に追加（PowerShellを利用するのが確実）
    system("powershell -Command \"[System.Environment]::SetEnvironmentVariable('Path', [System.Environment]::GetEnvironmentVariable('Path', 'User') + ';C:\\BritLang', 'User')\"");
    printf("[2/3] Environment variable 'Path' updated (C:\\BritLang).\n");
}

int main() {
    printf("--- BritLang Official Installer by StedTechOrganization(TM) ---\n\n");

    install_extension();
    set_environment_variable();
    
    printf("\n[3/3] Installation Complete! Please restart your terminal/VS Code.\n");
    printf("Cheerio!\n");
    system("pause");
    return 0;
}