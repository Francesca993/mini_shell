# Minishell - Parsing e Costruzione Pipeline

## 🧠 Descrizione del Parsing

Il parsing della minishell è organizzato in **due fasi principali**:

---

## 1. Lexer (`lexer.c`, `lexer_utils.c`)

**Obiettivo:** suddividere la stringa di input in token e associare ciascuno a un tipo.

### Token riconosciuti

* `WORD` → comandi, argomenti
* `PIPE` → `|`
* `REDIR_IN` → `<`
* `REDIR_OUT` → `>`
* `APPEND` → `>>`
* `HEREDOC` → `<<`

### Funzionalità:

* **Quote:**

  * Singole `'...'` → letterali
  * Doppie `"..."` → permettono `$VAR`
* **Errori gestiti:** quote non chiuse, backslash isolati

### Output del Lexer

```c
int lexer(char *line, char ***tokens_out, t_token_type **types_out);
```

Restituisce:

* `tokens_out` → array di stringhe
* `types_out` → array di `t_token_type`

---

## 2. Parser (`parser.c`, `build_pipeline.c`, ...)

**Obiettivo:** costruire strutture dati `t_cmd` e `t_pipeline` a partire da token e tipi.

### Struttura `t_cmd`

```c
typedef struct s_cmd {
    char **args;
    char *infile;
    char *outfile;
    int append;
    int heredoc;
    int redir_in;
    int redir_out;
    int fd_in;
    int fd_out;
    int pipe;
    int quote_single;
    int quote_double;
    int dollar;
} t_cmd;
```

### Funzioni Chiave:

* `build_pipeline()`
* `populate_comands()`
* `expand_pipeline_variables()`
* `find_quotes()`

### Quote e variabili:

* Quote gestite con `quote_handling.c`
* Variabili gestite con `exp_var.c`

---

## Flusso completo

```text
readline()
  ↓
lexer() → tokens + types
  ↓
parser() → t_pipeline
  ↓
executor(t_pipeline, env)
```

---

## Struttura `t_pipeline`

```c
typedef struct s_pipeline {
    t_cmd **cmds;
    int n_cmds;
    char **tokens;
    t_token_type *types;
    char **my_env;
} t_pipeline;
```

---

## Prossimi Passi

* [ ] Executor con `fork()`, `execve()`, `dup2()`
* [ ] Heredoc (`<<`) con pipe temporanee
* [ ] Redirezioni `>`, `>>`, `<` con `open()` + `dup2()`

---

## Builtin Supportati

* `echo`, `cd`, `pwd`, `export`, `unset`, `exit`, `env`

---

## Stato

* Lexer: ✅
* Parser: ✅
* Quote/variabili: ✅
* Redirezioni: in sviluppo
* Executor: da implementare
