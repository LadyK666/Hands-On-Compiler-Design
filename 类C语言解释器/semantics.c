#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "parser.tab.h"
#include "def.h"
/*******************begin********************************/
#include "math.h"
/* 日志与源码打印总开关：将 1 改为 0 可一键关闭所有日志与源码打印 */
#define ENABLE_LOG 0
#if defined(_WIN32)
#define DEV_NULL "NUL"
#else
#define DEV_NULL "/dev/null"
#endif
typedef struct {
    int type; /* 0:int, 1:float */
    int int_val;
    float float_val;
} Value;

static Symbol* symbol_table_head = NULL;

static Value evalExpression(TreeNode* node);
static void evalStatementList(TreeNode* node);

TreeNode* createNode(NodeKind kind, const char *name) {
    TreeNode* n = (TreeNode*)malloc(sizeof(TreeNode));
    n->kind = kind;
    if (name) {
        strncpy(n->name, name, sizeof(n->name) - 1);
        n->name[sizeof(n->name) - 1] = '\0';
    } else {
        n->name[0] = '\0';
    }
    n->lineno = 0;  /* 默认行号为0，需要在创建节点时设置 */
    n->int_val = 0;
    n->float_val = 0.0f;
    n->child[0] = n->child[1] = n->child[2] = NULL;
    n->next = NULL;
    return n;
}

TreeNode* makeOpNode(const char *op, TreeNode *left, TreeNode *right) {
    TreeNode* n = createNode(NODE_OP, op);
    n->child[0] = left;
    n->child[1] = right;
    return n;
}

TreeNode* makeAssignNode(TreeNode *id, TreeNode *exp) {
    TreeNode* n = createNode(NODE_OP, "=");
    n->child[0] = id;
    n->child[1] = exp;
    return n;
}

TreeNode* makeIfNode(TreeNode *cond, TreeNode *then_stmt, TreeNode *else_stmt) {
    TreeNode* n = createNode(NODE_OP, "IF");
    n->child[0] = cond;
    n->child[1] = then_stmt;
    n->child[2] = else_stmt;
    return n;
}

TreeNode* makeWhileNode(TreeNode *cond, TreeNode *body) {
    TreeNode* n = createNode(NODE_OP, "WHILE");
    n->child[0] = cond;
    n->child[1] = body;
    return n;
}

TreeNode* makePrintNode(TreeNode *exp) {
    TreeNode* n = createNode(NODE_OP, "PRINT");
    n->child[0] = exp;
    return n;
}

TreeNode* makeScanNode(TreeNode *id) {
    TreeNode* n = createNode(NODE_OP, "SCAN");
    n->child[0] = id;
    return n;
}

void printTree(TreeNode *node, int indent) {
    (void)node; (void)indent;
}

Symbol* lookupSymbol(const char *name) {
    Symbol* cur = symbol_table_head;
    while (cur) {
        if (strcmp(cur->name, name) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

void insertSymbol(const char *name, int type) {
    Symbol* s = (Symbol*)malloc(sizeof(Symbol));
    strncpy(s->name, name, sizeof(s->name) - 1);
    s->name[sizeof(s->name) - 1] = '\0';
    s->type = type;
    s->value.int_val = 0;
    s->value.float_val = 0.0f;
    s->next = symbol_table_head;
    symbol_table_head = s;
}

void setSymbolValue(const char *name, int type, void *value) {
    Symbol* s = lookupSymbol(name);
    if (!s) {
        insertSymbol(name, type);
        s = symbol_table_head;
    }
    s->type = type;
    if (type == 0) {
        s->value.int_val = *(int*)value;
    } else {
        s->value.float_val = *(float*)value;
    }
    /* 如需关闭赋值日志，请注释本 if-else 块内的 fprintf(stderr, ...) 行 */
    if (type == 0) {
        fprintf(stderr, "[LOG] set %s = %d\n", name, s->value.int_val);
    } else {
        fprintf(stderr, "[LOG] set %s = %.6f\n", name, s->value.float_val);
    }
}

void printSymbolTable() {
    Symbol* cur = symbol_table_head;
    while (cur) {
        if (cur->type == 0) printf("%s = %d\n", cur->name, cur->value.int_val);
        else printf("%s = %.6f\n", cur->name, cur->value.float_val);
        cur = cur->next;
    }
}

static Value makeInt(int x) { Value v; v.type = 0; v.int_val = x; v.float_val = (float)x; return v; }
static Value makeFloat(float x) { Value v; v.type = 1; v.float_val = x; v.int_val = (int)x; return v; }
static Value toFloat(Value v) { return (v.type == 1) ? v : makeFloat((float)v.int_val); }

static Value evalBinaryArith(const char* op, Value a, Value b) {
    if (strcmp(op, "/") == 0 && a.type == 0 && b.type == 0) {
        if (b.int_val == 0) return makeInt(0);
        return makeInt(a.int_val / b.int_val);
    }
    /* BUGFIX: 对于 +, -, * 若两操作数均为整型，应保持整型结果。
       之前实现无条件提升为浮点，导致 sum=0; sum=sum+i; 最终变为浮点 55.000000。 */
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "*") == 0) {
        if (a.type == 0 && b.type == 0) {
            if (strcmp(op, "+") == 0) return makeInt(a.int_val + b.int_val);
            if (strcmp(op, "-") == 0) return makeInt(a.int_val - b.int_val);
            /* op == "*" */ return makeInt(a.int_val * b.int_val);
        } else {
            Value fa = toFloat(a), fb = toFloat(b);
            if (strcmp(op, "+") == 0) return makeFloat(fa.float_val + fb.float_val);
            if (strcmp(op, "-") == 0) return makeFloat(fa.float_val - fb.float_val);
            /* op == "*" */ return makeFloat(fa.float_val * fb.float_val);
        }
    }
    /* 原实现（已修正，保留作参考）：
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "*") == 0 || strcmp(op, "/") == 0) {
        Value fa = toFloat(a), fb = toFloat(b);
        if (strcmp(op, "+") == 0) return makeFloat(fa.float_val + fb.float_val);
        if (strcmp(op, "-") == 0) return makeFloat(fa.float_val - fb.float_val);
        if (strcmp(op, "*") == 0) return makeFloat(fa.float_val * fb.float_val);
        if (strcmp(op, "/") == 0) {
            if (fb.float_val == 0.0f) return makeFloat(0.0f);
            return makeFloat(fa.float_val / fb.float_val);
        }
    }
    */
    if (strcmp(op, "/") == 0) {
        Value fa = toFloat(a), fb = toFloat(b);
        if (fb.float_val == 0.0f) return makeFloat(0.0f);
        return makeFloat(fa.float_val / fb.float_val);
    }
    Value fa = toFloat(a), fb = toFloat(b);
    int res = 0;
    if (strcmp(op, "==") == 0) res = fabsf(fa.float_val - fb.float_val) < 1e-7f;
    else if (strcmp(op, "!=") == 0) res = !(fabsf(fa.float_val - fb.float_val) < 1e-7f);
    else if (strcmp(op, ">") == 0) res = fa.float_val > fb.float_val;
    else if (strcmp(op, ">=") == 0) res = fa.float_val >= fb.float_val;
    else if (strcmp(op, "<") == 0) res = fa.float_val < fb.float_val;
    else if (strcmp(op, "<=") == 0) res = fa.float_val <= fb.float_val;
    return makeInt(res);
}

static Value evalExpression(TreeNode* node) {
    if (node == NULL) return makeInt(0);
    switch (node->kind) {
        case NODE_INT: return makeInt(node->int_val);
        case NODE_FLOAT: return makeFloat(node->float_val);
        case NODE_ID: {
            Symbol* s = lookupSymbol(node->name);
            /* 原实现（已修正）：未定义变量时返回0，未报错
            if (!s) return makeInt(0);
            */
            if (!s) {
                printf("第%d行的符号%s未定义值\n", node->lineno, node->name);
                exit(1);
            }
            return (s->type == 0) ? makeInt(s->value.int_val) : makeFloat(s->value.float_val);
        }
        case NODE_OP: {
            const char* op = node->name;
            if (strcmp(op, "=") == 0) {
                TreeNode* idn = node->child[0];
                Value v = evalExpression(node->child[1]);
                if (idn && idn->kind == NODE_ID) {
                    if (v.type == 0) { int t = v.int_val; setSymbolValue(idn->name, 0, &t); }
                    else { float f = v.float_val; setSymbolValue(idn->name, 1, &f); }
                }
                return v;
            }
            if (strcmp(op, "NEG") == 0) {
                Value v = evalExpression(node->child[0]);
                return (v.type == 0) ? makeInt(-v.int_val) : makeFloat(-v.float_val);
            }
            if (
                strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "*") == 0 || strcmp(op, "/") == 0 ||
                strcmp(op, "==") == 0 || strcmp(op, "!=") == 0 || strcmp(op, ">") == 0 || strcmp(op, ">=") == 0 || strcmp(op, "<") == 0 || strcmp(op, "<=") == 0
            ) {
                Value a = evalExpression(node->child[0]);
                Value b = evalExpression(node->child[1]);
                return evalBinaryArith(op, a, b);
            }
            if (strcmp(op, "PRINT") == 0) {
                Value v = evalExpression(node->child[0]);
                /* 如需关闭 print 前的日志，请注释下面两行 fprintf(stderr, ...) */
                if (v.type == 0) fprintf(stderr, "[LOG] print int %d\n", v.int_val);
                else fprintf(stderr, "[LOG] print float %.6f\n", v.float_val);
                if (v.type == 0) printf("%d\n", v.int_val);
                else printf("%.6f\n", v.float_val);
                return v;
            }
            if (strcmp(op, "SCAN") == 0) {
                TreeNode* idn = node->child[0];
                char buf[128];
                if (scanf("%127s", buf) == 1) {
                    if (strchr(buf, '.')) { float f = (float)atof(buf); setSymbolValue(idn->name, 1, &f); return makeFloat(f); }
                    else { int t = atoi(buf); setSymbolValue(idn->name, 0, &t); return makeInt(t); }
                }
                /* 如需关闭 scan 失败日志，请注释本行 */
                fprintf(stderr, "[LOG] scan input failed, default 0\n");
                return makeInt(0);
            }
            if (strcmp(op, "IF") == 0) {
                Value c = evalExpression(node->child[0]);
                int truth = (c.type == 0) ? (c.int_val != 0) : (c.float_val != 0.0f);
                /* 如需关闭 if 条件日志，请注释本行 */
                fprintf(stderr, "[LOG] if cond = %s\n", truth ? "true" : "false");
                if (truth) evalStatementList(node->child[1]);
                else evalStatementList(node->child[2]);
                return makeInt(0);
            }
            if (strcmp(op, "WHILE") == 0) {
                int iter = 0;
                while (1) {
                    Value c = evalExpression(node->child[0]);
                    int truth = (c.type == 0) ? (c.int_val != 0) : (c.float_val != 0.0f);
                    if (!truth) {
                        /* 如需关闭 while 迭代日志，请注释本行及下方迭代计数日志 */
                        fprintf(stderr, "[LOG] while exit after %d iteration(s)\n", iter);
                        break;
                    }
                    /* 如需关闭 while 迭代日志，请注释本行 */
                    fprintf(stderr, "[LOG] while iteration %d\n", iter + 1);
                    evalStatementList(node->child[1]);
                    iter++;
                }
                return makeInt(0);
            }
            return makeInt(0);
        }
        default:
            return makeInt(0);
    }
}

static void evalStatementList(TreeNode* node) {
    TreeNode* cur = node;
    while (cur) {
        (void)evalExpression(cur);
        cur = cur->next;
    }
}

void eval(TreeNode *node) {
    /* 如需完全关闭执行过程日志，请注释本行与下方 end eval 行 */
    fprintf(stderr, "[LOG] begin eval\n");
    evalStatementList(node);
    fprintf(stderr, "[LOG] end eval\n");
}

int yyparse(void);
extern FILE* yyin;

int main(int argc, char** argv) {
    /* 单点关闭：当 ENABLE_LOG 为 0 时，重定向 stderr 到空设备，
       从而屏蔽所有日志与源码回显（它们全部输出到 stderr）。 */
#if !ENABLE_LOG
    freopen(DEV_NULL, "w", stderr);
#endif
    if (argc > 1 && argv[1]) {
        const char* path = argv[1];
        FILE* fp = fopen(path, "rb");
        if (!fp) {
            return yyparse();
        }
        if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); return yyparse(); }
        long sz = ftell(fp);
        if (sz < 0) { fclose(fp); return yyparse(); }
        rewind(fp);
        char* buf = (char*)malloc((size_t)sz + 1);
        if (!buf) { fclose(fp); return yyparse(); }
        size_t nread = fread(buf, 1, (size_t)sz, fp);
        buf[nread] = '\0';
        /* 如需关闭“源代码回显”，请注释本段中所有 fprintf/fwrite 行（[SRC BEGIN]/内容/[SRC END]） */
        fprintf(stderr, "[SRC BEGIN]\n");
        if (nread > 0) {
            fwrite(buf, 1, nread, stderr);
            if (buf[nread-1] != '\n') fputc('\n', stderr);
        }
        fprintf(stderr, "[SRC END]\n");
        free(buf);
        rewind(fp);
        yyin = fp;
        int rc = yyparse();
        fclose(fp);
        return rc;
    } else {
        FILE* tmp = tmpfile();
        if (!tmp) {
            return yyparse();
        }
        size_t cap = 4096;
        size_t len = 0;
        char* buf = (char*)malloc(cap);
        if (!buf) {
            return yyparse();
        }
        int ch;
        while ((ch = fgetc(stdin)) != EOF) {
            if (len + 1 >= cap) {
                cap *= 2;
                char* nb = (char*)realloc(buf, cap);
                if (!nb) { free(buf); fclose(tmp); return yyparse(); }
                buf = nb;
            }
            buf[len++] = (char)ch;
            fputc(ch, tmp);
        }
        buf[len] = '\0';
        /* 如需关闭“源代码回显”，请注释本段中所有 fprintf/fwrite 行（[SRC BEGIN]/内容/[SRC END]） */
        fprintf(stderr, "[SRC BEGIN]%s[SRC END]\n", (len > 0) ? "\n" : "");
        if (len > 0) {
            fwrite(buf, 1, len, stderr);
            if (buf[len-1] != '\n') fputc('\n', stderr);
        }
        fprintf(stderr, "[SRC END]\n");
        free(buf);
        rewind(tmp);
        yyin = tmp;
        int rc = yyparse();
        fclose(tmp);
        return rc;
    }
}

/*******************end********************************/