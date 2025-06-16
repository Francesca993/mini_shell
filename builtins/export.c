/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:03:02 by francesca         #+#    #+#             */
/*   Updated: 2025/06/16 18:21:31 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
Testa questi casi:
export
export VAR1=ciao
export VAR1=nuovo
export VAR2
export 1INVALID=fail
export VAR3=ok VAR4 VAR5=123
*/

/*
export VAR=VAL	Aggiunge alla fine se non esiste
export VAR=NUOVO	Sovrascrive valore, mantiene posizione
export	Mostra tutte, nell'ordine corrente
env         # stampa tutte le variabili "vive" (quelle con =valore)
export      # stampa tutte, anche quelle senza valore
✅ In sintesi
Sintassi VAR=VAL	OK
Aggiunta nuovi	OK
Sovrascrittura esistenti	OK
Nome valido solo	OK
Comando senza argomenti (export)
*/

/*
** Restituisce l'indice della variabile in my_env se esiste (VAR= o VAR),
** altrimenti -1
*/
static int find_env_index(char **my_env, const char *key)
{
    int i = 0;
    size_t len = ft_strlen(key);

    while (my_env[i])
    {
        if (ft_strncmp(my_env[i], key, len) == 0 &&
            (my_env[i][len] == '=' || my_env[i][len] == '\0'))
            return i;
        i++;
    }
    return -1;
}

/*
** Verifica se una stringa è un identificatore valido per export/unset.
** Regole (come in Bash):
** - Deve iniziare con una lettera (a-z, A-Z) o underscore '_'
** - Può contenere lettere, numeri e underscore
** - Il controllo si ferma prima del primo '=' (se presente)
** Esempi validi:   VAR, _HOME, VAR123
** Esempi invalidi: 1ABC, VAR-TEST, =INVALID
** Ritorna:
** - 1 se valido
** - 0 se invalido
** ============================================
*/
int is_valid_identifier(const char *str)
{
    int i = 1;

    // Primo carattere: deve essere una lettera o '_'
    if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
        return (0);

    // Scorri il resto finché non trovi '=' o la fine
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0); // carattere non valido
        i++;
    }

    return (1); // tutto valido
}

/*
** Aggiunge una nuova variabile a my_env (in fondo)
*/
static void add_new_var(char **my_env, const char *entry)
{
    int len = 0;
    while (my_env[len])
        len++;

    my_env[len] = ft_strdup(entry);
    my_env[len + 1] = NULL;
}

/*
** Stampa tutte le variabili come `declare -x` (come Bash)
*/
void print_export_buildin(char **my_env)
{
    int i = 0;

    while (my_env[i])
    {
        char *equal = ft_strchr(my_env[i], '=');
        if (equal)
        {
            size_t key_len = equal - my_env[i];
            printf("declare -x ");
            write(1, my_env[i], key_len + 1); // "VAR="
            printf("\"%s\"\n", equal + 1);    // "valore"
        }
        else
            printf("declare -x %s\n", my_env[i]); // caso VAR senza '='
        i++;
    }
}

/*
** Trova la chiave (prima di '=' se presente)
*/
char *find_key(char *str)
{
        char *equal = ft_strchr(str, '=');
        char *key;

        // ricava il nome della variabile (prima di '=' se presente)
        if (equal)
            key = ft_substr(str, 0, equal - str);
        else
            key = ft_strdup(str);

    return (key);
}

void export_variable(char **my_env, char **args)
{
    int i;
    char *key;
    char *equal;
    int index;
    //perche il primo argomento è il comando export  
    i = 1;
    g_exit_status = 0; 
    if (!args[1]) // solo "export" → stampa l'ambiente
    {
        print_export_buildin(my_env);
        return;
    }
    while (args[i])
    {
        equal = ft_strchr(args[i], '=');
        if (equal)
            key = ft_substr(args[i], 0, equal - args[i]);
        else
            key = ft_strdup(args[i]);
        if (!is_valid_identifier(key))
        {
            fprintf(stderr, "minishell: export: `%s`: not a valid identifier\n", args[i]);
            g_exit_status = 1;
            free(key);
            i++;
            continue;
        }
        index = find_env_index(my_env, key);
        // Esiste già
        if (index >= 0)
        {
            if (equal) // aggiorna solo se c'è '='
            {
                free(my_env[index]);
                my_env[index] = ft_strdup(args[i]);
            }
        }
        else
        {
            if (equal)
                add_new_var(my_env, args[i]); // VAR=VAL
            else
            {
                char *tmp = ft_strjoin(args[i], "="); // aggiunge VAR=
                add_new_var(my_env, tmp);
                free(tmp);
            }
        }
        free(key);
        i++;
    }
}

