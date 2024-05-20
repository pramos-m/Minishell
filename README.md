# MINISHELL
`minishell` is a project where you create your own shell based on specific requirements.

## Contents

1. [Algorithm/pseudocode](#Algorithm-pseudocode)
2. [Initializing](#Initializing)
3. [Parsing](#Parsing)
   - [Tokenizer](#Tokenizer)
   - [Re-order command](#Re-order-command)
   - [Build the tree](#Build-the-tree)
4. [Executing](#Executing)
   - [Builtins](#Builtins)
   - [Executor](#Executor)
   - [and/or](#and-or)
   - [pipes](#pipes)
   - [redirections](#redirections)
   - [command](#command)
   - [Expanding](#Expanding)
   - [Wildcard](#Wildcard)
   - [executing the command after forking](#execution)
   - [exit status](#exit-status)
5. [Tests](#Tests)
6. [Resources](#Resources)

### Algorithm pseudocode
---

1. Initializing: 
      - Set up global variables: status code and environment variables. 
      - Save stdin/stdout using dup. 
      - Read user input command using readline function.
2. Parsing:
      - Build a doubly linked list to hold the command tokens using shell grammar.
      - Check for syntax errors while tokenizing.
      - Re-order the command using the Shunting Yard algorithm.
      - Build the syntax tree.
3. Executing:
      - Execute the syntax tree recursively from bottom-up and left to right.
      - Handle tokens (and/or/pipe) and commands, including expansion, splitting, wildcard handling, and redirections.
      - Check for built-in commands or fork and execute using execve.

## Initializing

Replace environment variables into a linked list for easy addition or deletion using export and unset builtins. Display them using env or export builtins.

## Parsing
### Tokenizer
---

Types of tokens:
```bash
typedef enum e_token
{
    PIPE,     // |
    HEREDOC,  // <<
    LPR,      // (
    RPR,      // )
    AND,      // &&
    OR,       // ||
    APPEND,   // >>
    OUT,      // >
    IN,       // <
    NOT,      // string
    END       // end of cmd
}t_token;
```
## Grammar used:
```sh
conditional ::=  pipeline
          |   conditional "&&" pipeline
          |   conditional "||" pipeline

pipeline ::=  command
          |   pipeline "|" command

command  ::=  word
          |   redirection
          |   command word
          |   command redirection

redirection  ::=  redirectionop filename
redirectionop  ::=  "<"  |  ">"  |  "2>"
```
- A conditional is a series of pipelines, concatenated by && or ||.
- A pipeline is a series of commands, concatenated by |.
- A redirection is one of <, >, >> or <<, followed by a filename.

Check for syntax errors while tokenizing:

- Ensure quotes and parentheses are closed.
- Verify token sequences for correctness (e.g., "|" / "&&" / "||" should be followed by NOT / redirection / "(").

## Redirections:

```sh
struct s_redirect
{
	int		type;
	t_token	*file_tk;
	char	*file;
	int		warn;
	int		fd[2];
	int		tmp_fd[2];
} t_redir;

typedef struct s_node
{
    char            *pre_cmd;
    char            **cmd;
    t_token         tok;
    int             precedence;
    t_redir         *redirections;
    int             fd[2];
    struct s_node   *lchild;
    struct s_node   *rchild;
}t_node;
```
Handle redirections by saving the type and filename, replacing command characters with ASCII 127.

- Re-order command
- Re-order commands using the Shunting Yard algorithm for easier tree building.

### Build the tree
Build the syntax tree after re-ordering the command:

```sh
t_lstt	*tk_to_lstt(t_token **tk)
{
	t_lstt	*node;
	t_token	*tmp;
	t_token	*tmp2;

	if (!tk || !*tk)
		return (NULL);
	node = lstt_create(tk);
	if (!node)
		return (NULL);
	if (node->type == STAIR)
	{
		tmp = tk_get_in_parenthesis(tk);
		if (!tmp)
			return (lstt_clean(&node));
		tmp2 = tmp;
		node->content = st_generate(tmp);
		tk_clean(&tmp2, NEXT);
	}
	else
		node->content = tk_to_cmd(tk);
	if (!node->content)
		return (ft_free((char **)&node, 2));
	return (node);
}
```
## Executing
### Builtins
1. Export:
- Display environment variables sorted with declare -x.
2. Add/update variables in the environment list.
- Handle variable identifiers correctly.
3. unset:
- Remove variables from the environment list.
4. env:
- Display environment variables.
5. pwd:
- Show the current working directory.
6. exit:
 -  xit the shell with the appropriate status code.
7. echo with -n:
- Display arguments with optional newline suppression.
8. cd:
- Change the current directory and update environment variables.
Executor
and/or
1. AND: Run the right side if the left side's exit status is 0.
2. OR: Run the right side if the left side's exit status is non-zero.
pipes
Handle pipes by chaining commands.

## redirections:
Handle redirections by opening the appropriate files and managing heredocs.

### command
Expanding
Expand variables except inside single quotes and heredoc delimiters.
Use a custom getenv function.
Wildcard
Use directory access functions to match patterns with files, avoiding expansion inside quotes and hidden files unless explicitly requested.
execution
After expanding and splitting the command, check for built-ins or fork and execute with execve.
exit status
Commands exit with a status code indicating success (0) or failure (non-zero). Handle specific error statuses for different cases.

## Tests
```sh
$ ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls 
$ ls" | cat"
$ ls """
$ ls "don't"
$ l"s"""
$ l"'s'"
$ .
$ ..
$ export var0="head -"
$ export var1="n 1"
$ export var2="0"
$ echo $var0$var1$var2
$ head -n 10
$ $var0$var1$var2 //it should work
$ <<$x cat
$ <<   l"" cat
$ <<l cat
$ cat >$x
$ <file1
$ <<delim
$ cd
$ /bin/ls
// test this in bash
1. $ mkdir test
2. $ cd test
3. $ rm -rf ../test
4. $ pwd
5. $ cd . //this is gonna display an error

$ ls >file1>file2>>file3
$ << l << delim << dl cat
1. $ touch file1
2. $ chmod 000 file1
3. $ cat >file0>file1>file3
//error
4. cd file1
//
1. $ unset path
2. $ ls
3. $ /bin/ls
//
$ env -i ./minishell
//
1. $ grop
2. $ echo $?
//
1. $ cc test.c
2. $ chmod 000 a.out
3. $ ./a.out
4. $ echo $?
//
1. $ cat
CTR ^C
2. $ echo $?
//
$ echo $HOME
$ echo $HOMEsdjhfk$HOME
$ echo $HOME.sdjhfk$HOME
```
## Resources:
https://en.wikipedia.org/wiki/Shunting_yard_algorithm
https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm
https://en.wikipedia.org/wiki/Reverse_Polish_notation
https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form
https://cs61.seas.harvard.edu/site/2019/Section7/
https://brilliantorg-infra-prod.brilliant.org/wiki/shunting-yard-algorithm/
https://www.codequoi.com/en/sending-and-intercepting-a-signal-in-c/
https://www.codequoi.com/en/errno-and-error-management-in-c/
https://www.codequoi.com/en/pipe-an-inter-process-communication

