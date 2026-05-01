#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// 拡張機能のインストール
void install_extension() {
    char* userprofile = getenv("USERPROFILE");
    char ext_path[MAX_PATH];
    char cmd[2048];

    sprintf(ext_path, "%s\\.vscode\\extensions\\britlang-syntax", userprofile);
    
    // フォルダ作成
    sprintf(cmd, "mkdir \"%s\" 2>nul", ext_path);
    system(cmd);

    // コピー実行（/Q オプションで画面出力を抑制）
    sprintf(cmd, "xcopy /y /e /q .\\britlang-syntax \"%s\\\"", ext_path);
    system(cmd);
}

// 環境変数の設定
void set_environment_variable() {
    system("mkdir C:\\BritLang 2>nul");
    system("copy /y britlang.exe C:\\BritLang\\");

    // PowerShellでPath更新
    system("powershell -WindowStyle Hidden -Command \"[System.Environment]::SetEnvironmentVariable('Path', [System.Environment]::GetEnvironmentVariable('Path', 'User') + ';C:\\BritLang', 'User')\"");
}

// メイン関数 (WinMainにすることでコンソールが出なくなります)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
    // 1. 開始確認
    int msgboxID = MessageBox(
        NULL,
        "Would you like to install BritLang Support?\n(StedTechOrganization(TM) official installer)",
        "BritLang Setup",
        MB_ICONINFORMATION | MB_YESNO | MB_DEFBUTTON1
    );

    if (msgboxID == IDYES) {
        // インストール実行
        install_extension();
        set_environment_variable();

        // 2. 完了通知
        MessageBox(
            NULL,
            "Installation Complete!\n\nBritLang has been successfully integrated into your system.\nPlease restart VS Code and your terminal.\n\nCheerio!",
            "Success",
            MB_ICONINFORMATION | MB_OK
        );
    } else {
        // キャンセル時
        MessageBox(NULL, "Installation cancelled.", "BritLang Setup", MB_OK);
    }

    return 0;
}