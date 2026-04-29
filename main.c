#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARS 100
#define MAX_LINE 256

typedef struct {
    char name[32];
    int value;
    int is_const;
} Variable;

Variable vars[MAX_VARS];
int var_count = 0;

// 変数の取得・設定
int get_var_value(char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) return vars[i].value;
    }
    return atoi(name); // 変数名でなければ数値として扱う
}

void set_var(char* name, int val, int is_const) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            if (vars[i].is_const) {
                printf("Error: My apologies, but '%s' is a constant and cannot be changed.\n", name);
                exit(1);
            }
            vars[i].value = val;
            return;
        }
    }
    strcpy(vars[var_count].name, name);
    vars[var_count].value = val;
    vars[var_count].is_const = is_const;
    var_count++;
}

// 簡易実行エンジン
void execute_line(char* line) {
    char cmd[64], name[64], op[10], target[64];
    int val;

    // 前後の空白削除
    while(isspace(*line)) line++;
    if (*line == '\0' || *line == '#') return;

    // constant定義: constant x = 10
    if (sscanf(line, "constant %s = %d", name, &val) == 2) {
        set_var(name, val, 1);
    }
    // variable定義: variable y = 20
    else if (sscanf(line, "variable %s = %d", name, &val) == 2) {
        set_var(name, val, 0);
    }
    // 出力: Iwouldliketosaythat "Hello" もしくは 変数名
    else if (strncmp(line, "Iwouldliketosaythat", 19) == 0) {
        char *content = line + 20;
        while(isspace(*content)) content++;
        if (*content == '"') {
            char msg[MAX_LINE];
            sscanf(content, "\"%[^\"]\"", msg);
            printf("%s\n", msg);
        } else {
            char var_name[64];
            sscanf(content, "%s", var_name);
            printf("%d\n", get_var_value(var_name));
        }
    }
    // 計算: variable = variable + value
    else if (sscanf(line, "%s = %s %s %d", name, target, op, &val) == 4) {
        int current = get_var_value(target);
        if (strcmp(op, "+") == 0) set_var(name, current + val, 0);
        if (strcmp(op, "-") == 0) set_var(name, current - val, 0);
    }
}

int main(int argc, char *argv[]) {
    // 引数チェック: britlang execute -p "path"
    if (argc < 4 || strcmp(argv[1], "execute") != 0 || strcmp(argv[2], "-p") != 0) {
        printf("Usage: britlang execute -p \"filename.brit\"\n");
        return 1;
    }

    FILE *file = fopen(argv[3], "r");
    if (!file) {
        printf("Error: I'm dreadfully sorry, but the file '%s' could not be found.\n", argv[3]);
        return 1;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        execute_line(line);
    }

    fclose(file);
    return 0;
}