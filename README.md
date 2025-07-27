# Minishell

A lightweight shell implementation written in C, featuring command execution, environment variable management, and basic shell functionality.

## 📋 Description

Minishell is a command-line interpreter that replicates the basic functionality of bash. It provides an interactive shell environment where users can execute commands, manage environment variables, and use various shell features like pipes, redirections, and built-in commands.

## ✨ Features

### 🚀 Core Functionality
- **Interactive Command Line Interface** with prompt and command history
- **Command Execution** with PATH resolution
- **Environment Variable Management** (get, set, unset)
- **Signal Handling** (Ctrl+C, Ctrl+D, Ctrl+\)

### 🔧 Built-in Commands
- `echo` - Display text with optional `-n` flag
- `cd` - Change directory
- `pwd` - Print working directory
- `export` - Set environment variables
- `unset` - Remove environment variables
- `env` - Display environment variables
- `exit` - Exit the shell

### 🔗 Advanced Features
- **Pipe Support** (`|`) - Connect multiple commands
- **Input/Output Redirections**:
  - `<` - Input redirection
  - `>` - Output redirection (truncate)
  - `>>` - Output redirection (append)
  - `<<` - Here document (heredoc)
- **Quote Handling** - Support for single and double quotes
- **Variable Expansion** - Environment variable substitution
- **Error Handling** - Comprehensive error management

## 🛠️ Requirements

- **Operating System**: Linux/Unix-like system
- **Compiler**: GCC with C99 standard
- **Libraries**: 
  - `readline` - For command line editing
  - `ncurses` - Terminal manipulation library

### Installing Dependencies

#### Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install gcc make libreadline-dev libncurses5-dev
```

#### CentOS/RHEL/Fedora:
```bash
sudo yum install gcc make readline-devel ncurses-devel
# or for newer versions:
sudo dnf install gcc make readline-devel ncurses-devel
```

#### macOS:
```bash
brew install readline ncurses
```

## 🚀 Installation & Usage

### Building the Project

1. **Clone the repository**:
   ```bash
   git clone <your-repository-url>
   cd minishell
   ```

2. **Compile the project**:
   ```bash
   make
   ```

3. **Run minishell**:
   ```bash
   ./minishell
   ```

### Makefile Commands

- `make` - Build the project
- `make clean` - Remove object files
- `make fclean` - Remove object files and executable
- `make re` - Rebuild the project
- `make norm` - Check code style with norminette

## 📖 Usage Examples

### Basic Commands
```bash
$ pwd
/home/user

$ ls -la
total 40
drwxr-xr-x  2 user user 4096 Jan 15 10:30 .
drwxr-xr-x 20 user user 4096 Jan 15 10:30 ..

$ echo "Hello, World!"
Hello, World!
```

### Environment Variables
```bash
$ export MY_VAR="Hello World"
$ echo $MY_VAR
Hello World

$ env | grep MY_VAR
MY_VAR=Hello World
```

### Pipes and Redirections
```bash
$ ls | grep .c
main.c
utils.c

$ echo "Hello" > output.txt
$ cat output.txt
Hello

$ cat < input.txt
File contents here

$ echo "Appending" >> output.txt
```

### Here Documents
```bash
$ cat << EOF
> This is a here document
> It continues until EOF
> EOF
This is a here document
It continues until EOF
```

## 🏗️ Project Structure

```
minishell/
├── include/              # Header files
│   ├── minishell.h      # Main header
│   └── types.h          # Data structures
├── libft/               # Custom library functions
├── src/                 # Source code
│   ├── core/           # Main shell logic
│   ├── parser/         # Command parsing
│   ├── execution/      # Command execution
│   ├── builtins/       # Built-in commands
│   │   ├── cd/
│   │   ├── echo/
│   │   ├── env/
│   │   ├── export/
│   │   ├── pwd/
│   │   ├── unset/
│   │   └── exit/
│   ├── pipes/          # Pipe handling
│   ├── redirections/   # I/O redirections
│   ├── quotes/         # Quote processing
│   ├── signals/        # Signal handling
│   └── utils/          # Utility functions
├── Makefile            # Build configuration
└── README.md          # This file
```

## 🔧 Technical Details

### Architecture
- **Modular Design**: Each feature is implemented in separate modules
- **Memory Management**: Custom garbage collector for memory safety
- **Error Handling**: Comprehensive error checking and reporting
- **Signal Safety**: Proper signal handling for interactive shell

### Key Components
- **Lexer**: Tokenizes input into meaningful units
- **Parser**: Builds command structures from tokens
- **Executor**: Handles command execution and process management
- **Built-ins**: Native shell commands implemented in C

## 📝 License

This project is part of the 42 curriculum and follows the 42 coding standards.

⭐ If you find this project helpful, please give it a star! 
