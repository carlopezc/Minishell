# 🚀 Minishell

> A custom-built UNIX shell that replicates many of the core features of `bash`.

---

## 🎯 Project Goals

This is a major project that combines many core concepts of **system programming**. The goal is to build a shell that can read user input, parse it, and execute commands. It's the culmination of everything learned in `Pipex`, requiring robust **process management**, complex **parsing**, and handling of `PATH`, `env`, signals, and more.

## 🛠️ Technical Details

* **Command Parsing:** Developed a lexer and parser to correctly interpret user input, handling single and double quotes, special characters, and environment variable expansion (e.g., `$USER`).
* **Process Management:** The shell manages the lifecycle of processes, allowing it to execute external commands found in the system's `PATH`.
* **Pipes & Redirections:** Implements full support for input/output redirection (`<`, `>`, `>>`, `<<`) and pipelines (`|`), allowing data to flow seamlessly between commands.
* **Built-in Commands:** Recreated core shell utilities internally, including `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.
* **Signal Handling:** Mimics standard terminal behavior, correctly processing interrupt signals (like `Ctrl-C`) without crashing the shell.
* **Challenge:** The main challenge was coordinating the logic between the parser and the execution engine, ensuring that memory is managed perfectly during long sessions and that multiple processes communicate without errors.

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
