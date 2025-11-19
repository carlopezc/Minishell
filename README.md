# 🚀 Minishell

> A custom-built UNIX shell that replicates many of the core features of `bash`.

---

## 🎯 Project Goals

This is a major project that combines many core concepts of **system programming**. The goal is to build a shell that can read user input, parse it, and execute commands. It's the culmination of everything learned in `Pipex`, requiring robust **process management**, complex **parsing**, and handling of `PATH`, `env`, signals, and more.

## 🛠️ Technical Details

* **Parsing:** Implements a full lexer and parser to interpret the user's command line. It splits the input into tokens, handles quotes (`'` and `"`), and builds a command structure.
* **Command Execution:** Uses `execve()` to run external commands, searching for them in the `PATH` environment variable.
* **Built-ins:** Implements a set of commands *within* the shell itself, such as `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`, which do not run as separate processes.
* **Pipes & Redirections:** Managages complex I/O. It can handle multiple pipes (`|`) and all standard redirections (`<`, `>`, `<<`, `>>`), correctly linking the `stdin` and `stdout` of all processes.
* **Signal Handling:** Uses `sigaction()` to correctly manage `Ctrl-C` (which should show a new prompt, not exit) and `Ctrl-\` (which should do nothing), just like in `bash`.
* **Environment Variables:** Manages a list of environment variables, supporting expansion (e.g., `echo $PATH`) and modification (using `export` and `unset`).
* **Challenge:** The parser is notoriously complex, especially when handling edge cases with quotes and special characters. Managing all file descriptors correctly in a long chain of pipes (`cmd1 | cmd2 | cmd3`) is another huge challenge.

## ⚙️ How to Compile & Run

1.  Clone the repository:
    ```bash
    git clone [https://github.com/carlopezc/Minishell.git](https://github.com/carlopezc/Minishell.git)
    cd Minishell
    ```
2.  Compile the project:
    ```bash
    make
    ```
    This will create the main executable `minishell`.

3.  Run the program:
    ```bash
    ./minishell
    ```
    This will launch your interactive shell, ready to receive commands.
