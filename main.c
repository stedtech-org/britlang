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

// --- 既存の実行エンジン関数群 ---
int get_var_value(char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) return vars[i].value;
    }
    return atoi(name); 
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

void execute_line(char* line) {
    char name[64], op[10], target[64];
    int val;
    while(isspace(*line)) line++;
    if (*line == '\0' || *line == '#') return;

    if (sscanf(line, "constant %s = %d", name, &val) == 2) {
        set_var(name, val, 1);
    }
    else if (sscanf(line, "variable %s = %d", name, &val) == 2) {
        set_var(name, val, 0);
    }
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
    else if (sscanf(line, "%s = %s %s %d", name, target, op, &val) == 4) {
        int current = get_var_value(target);
        if (strcmp(op, "+") == 0) set_var(name, current + val, 0);
        if (strcmp(op, "-") == 0) set_var(name, current - val, 0);
    }
}

// --- 追加されたコマンド関数 ---
void show_help() {
    printf("--- BritLang Official CLI Help ---\n");
    printf("v-pre.1.3.0 \n");
    printf("Usage:\n");
    printf("execute -p <file> : Execute a .brit file\n");
    printf("america           : ...Don't even ask.\n");
    printf("tea               : Have a break.\n");
    printf("version           : Show BritLangClientVersion\n");
    printf("help              : Show this list\n");
}

void show_tea() {
    printf("\n");
    printf("       (  )   (  )\n");
    printf("        )  )   )  )\n");
    printf("      _(_)(_)(_)(_)_\n");
    printf("     |              |  __\n");
    printf("     |     TEA      | /  \\\n");
    printf("     |    TIME      ||    |\n");
    printf("     |______________| \\__/\n");
    printf("     \\______________/\n");
    printf("\nCheers! Relax and enjoy your tea.\n");
}

// --- メイン関数（コマンド分岐の統合） ---
int main(int argc, char *argv[]) {
    if (argc < 2) {
        show_help();
        return 0;
    }

    if (strcmp(argv[1], "help") == 0) {
        show_help();
    } 
    else if (strcmp(argv[1], "america") == 0) {
        printf("HELL NO\n");
    } 
    else if (strcmp(argv[1], "version") == 0) {
        printf("v-pre1.3.0\n");
    } 
    else if (strcmp(argv[1], "tea") == 0) {
        show_tea();
    } 
    else if (strcmp(argv[1], "execute") == 0) {
        if (argc < 4 || strcmp(argv[2], "-p") != 0) {
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
    } 
    else {
        printf("Unknown command: %s\n", argv[1]);
        show_help();
    }

    return 0;
}