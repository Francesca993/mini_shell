# 🧠 Parsing in `minishell`

Il parsing in `minishell` è progettato in **due fasi principali**: *tokenizzazione* della linea di input e *costruzione della pipeline di comandi*.
Questa fase prepara i dati per l’esecuzione dei comandi tramite strutture più comode da gestire rispetto a semplici array di stringhe.

---

## 🔹 1. Lexer – Analisi Lessicale

**Obiettivo:**
Dividere la linea in **token** e assegnare a ciascuno un tipo semantico (`t_token_type`).

### Token riconosciuti:

* `WORD` – parola generica (comandi, argomenti, nomi di file)
* `PIPE` – `|`
* `REDIR_IN` – `<`
* `REDIR_OUT` – `>`
* `APPEND` – `>>`
* `HEREDOC` – `<<`

### Gestione quote:

* Quote **singole** `'...'`: il contenuto è considerato letterale.
* Quote **doppie** `"..."`: ancora senza espansione di variabili.
* Se ci sono quote **non chiuse**, viene stampato un errore e il lexer restituisce `-1`.

### Funzioni principali:

* `count_token()` – Conta i token totali presenti nella linea.
* `fill_tokens()` – Riempie gli array `tokens[]` e `types[]`.
* `handle_word()` / `handle_redirection()` – Gestiscono parole e metacaratteri.
* `free_partial_tokens()` – Libera la memoria parzialmente allocata in caso di errore.
* `print_tokens()` – Debug: stampa token e tipi.

### Output del lexer:

```c
int lexer(char *line, char ***tokens_out, t_token_type **types_out);
```

* `tokens_out`: array di stringhe (es. `["grep", "<<", "eof", "|", "wc", NULL]`)
* `types_out`: array parallelo di tipi (es. `[WORD, HEREDOC, WORD, PIPE, WORD]`)

---

## 🔹 2. Parser – Costruzione Pipeline

La funzione principale è:

```c
t_pipeline *parse_line(char *line, char **env);
```

**Cosa fa:**

1. Chiama il `lexer()` per ottenere `tokens[]` e `types[]`.
2. Stampa i token per debug (`print_tokens()`).
3. Costruisce una struttura `t_pipeline` tramite `build_pipeline()`.
4. Chiama `populate_comands()` per creare un array di comandi `t_cmd **`.

---

## 📦 Strutture Dati Coinvolte

### `t_cmd` – Rappresenta un singolo comando:

```c
typedef struct s_cmd {
    char **args;        // ["grep", "ciao", NULL]
    char *infile;       // nome file per input redirection
    char *outfile;      // nome file per output redirection
    int append;         // 1 se >>
    int heredoc;        // 1 se <<
    int redir_in;       // 1 se <
    int redir_out;      // 1 se >
    int pipe;           // 1 se il comando ha una pipe in uscita
    int quote_single;   // 1 se contiene quote singole
    int quote_double;   // 1 se contiene quote doppie
    int dollar;         // 1 se contiene $
} t_cmd;
```

### `t_pipeline` – Rappresenta l’intera linea:

```c
typedef struct s_pipeline {
    t_cmd **cmds;           // array di comandi
    int n_cmds;             // numero comandi
    char **tokens;          // token grezzi
    t_token_type *types;    // tipi associati ai token
    int n_tokens;           // numero totale di token
} t_pipeline;
```

---

## 🧹 Funzionamento della pipeline

### `build_pipeline(char **tokens, t_token_type *types, int num_tokens)`

1. Conta i comandi separati da pipe.
2. Alloca una `t_pipeline` e un array di `t_cmd*`.
3. Per ogni comando:

   * Conta i token `WORD` (usati come `args`)
   * Gestisce le redirezioni e pipe.
   * Alloca e popola i campi di `t_cmd`.

### `populate_comands(t_pipeline *pipeline)`

* Legge i token e i tipi, uno a uno.
* Costruisce `cmd->args`, `cmd->infile`, `cmd->outfile`, `cmd->append`, ecc.
* Riconosce quando termina un comando (PIPE) e passa al successivo.

---

## 🔀 Flusso attuale:

```
readline()
    ↓
lexer() → [tokens[], types[]]
    ↓
parse_line()
    ↓
build_pipeline() → t_pipeline* (con t_cmd** cmds)
    ↓
executor()
```

---

## ✅ Prossimi Passi

* **Gestione quote e variabili** in modo più completo:

  * Espansione di `$VAR`, `$?`, ecc.
  * Quote doppie permettono espansione, singole no.
* **Costruzione definitiva della pipeline**:

  * Supporto a tutte le forme di redirezione.
* **Implementazione executor**:

  * `execute_cmd(t_cmd **pipeline, char **env)` con fork, execve, pipe.
  * Gestione redirezioni prima della `execve()`.
