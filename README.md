*This project has been created as part of the 42 curriculum by melmbaz.*

---

# get_next_line

## Description

`get_next_line` is a C function that reads a single line at a time from a file descriptor, returning it as a null-terminated string (including the trailing `\n` when present). Repeated calls to the function allow the caller to traverse an entire file or input stream line by line, without loading it all into memory at once.

The goal of the project is to deepen understanding of static variables, dynamic memory allocation, and low-level I/O in C — core competencies required for systems programming at 42 and beyond.

The implementation is split across three files:

- **`get_next_line.c`** — the main function and its internal helpers
- **`get_next_line_utils.c`** — auxiliary string utilities (`ft_strlen`, `ft_strdup`, `ft_calloc`, `ft_strchr`, `join_blocks`)
- **`get_next_line.h`** — shared header, defines `BUFFER_SIZE`, includes the `t_str` struct, and prototypes all functions

---

## Algorithm

### Overview

The implementation uses a **static read buffer + static accumulation string** approach, governed by a small state machine with two cases.

```
Case 0  →  keep reading from the fd, appending to `saved`
Case 1  →  a '\n' exists in `saved`; extract and return the line
```

### Data structures

A lightweight `t_str` struct pairs a `char *` with its precomputed `size_t` length. This allows `join_blocks` to concatenate an arbitrary number of string segments in a single allocation without ever calling `strlen` on each block more than once.

```c
typedef struct s_str {
    char   *str;
    size_t  len;
} t_str;
```

### Static variables in `get_next_line`

| Variable | Role |
|----------|------|
| `buff`   | A heap-allocated buffer of `BUFFER_SIZE + 1` bytes, reused across calls to avoid repeated `malloc`/`free`. |
| `saved`  | Accumulates characters that have been read from the fd but not yet returned to the caller. Persists across calls. |
| `cases`  | Integer flag: `0` = no newline found yet, `1` = newline present in `saved`. Drives the state machine in `reader`. |

### Step-by-step flow

1. **Initialisation** (`get_next_line`)  
   On the first call, `buff` is allocated once with `ft_calloc(BUFFER_SIZE + 1, 1)` and `saved` is initialised to an empty string `""`. Subsequent calls reuse both.

2. **`reader`**  
   If `saved` already contains a `\n` from a previous read cycle, it goes straight to `case_one`. Otherwise it enters a `while` loop that calls `case_zero` until either a newline appears or the fd is exhausted.

3. **`case_zero`** — *reading phase*  
   Reads up to `BUFFER_SIZE` bytes from the fd into `buff`. Appends the new chunk to `saved` via `join_blocks`, which does a single allocation for the combined string. If `\n` is found, sets `cases = 1` and returns.

4. **`case_one`** — *extraction phase*  
   Locates the first `\n` in `saved` with `ft_strchr`. Allocates a new string containing everything up to and including that `\n`. Moves the remainder of `saved` (everything after `\n`) into a freshly `ft_strdup`'d buffer and frees the old one. Resets `cases = 0`.

5. **End-of-file path** (`ft_gnl_end`)  
   When `case_zero` returns `-1` (read returned ≤ 0), `reader` returns `NULL`. `get_next_line` then calls `ft_gnl_end`, which returns any remaining non-empty content in `saved` as the final line, then frees both static buffers so the next call starts clean.

### Why this design?

- **Single allocation per join** — `join_blocks` computes the total length first, allocates once, then copies. This avoids the O(n²) growth caused by naively `strcat`-ing into a growing buffer.
- **Reused read buffer** — `buff` is allocated once per file descriptor lifecycle, not once per `read` call, reducing heap pressure.
- **State machine flag** — the `cases` integer makes the control flow in `reader` explicit and easy to follow, rather than relying on repeated `ft_strchr` calls to deduce state.
- **Clean teardown** — `ft_gnl_end` ensures no memory leak on EOF or error, even if the last line has no trailing newline.

### Complexity

| Property | Value |
|----------|-------|
| Memory per call | O(line length) for the returned string |
| Reads per line | ceil(line length / BUFFER_SIZE) |
| Allocations per read | 2 (new `saved`, possible `temp`) |

---

## Instructions

### Prerequisites

- A C compiler supporting C99 or later (e.g. `gcc`, `clang`)
- POSIX-compatible system (Linux, macOS, WSL)

### Compilation

The library does not ship a `Makefile`. Compile it directly alongside your project:

```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    get_next_line.c get_next_line_utils.c \
    -o your_program your_main.c
```

You can override the buffer size at compile time:

```bash
gcc -D BUFFER_SIZE=4096 get_next_line.c get_next_line_utils.c -o your_program your_main.c
```

### Usage example

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int   fd;
    char *line;

    fd = open("file.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

> **Note:** The caller is responsible for `free`-ing each line returned by `get_next_line`.

### Running a quick test

```bash
echo -e "hello\nworld\n42" > test.txt
gcc -D BUFFER_SIZE=4 get_next_line.c get_next_line_utils.c main.c -o gnl_test
./gnl_test
```

---

## Resources

### Documentation & references

- [POSIX `read(2)` man page](https://man7.org/linux/man-pages/man2/read.2.html) — the underlying system call used to fetch bytes from a file descriptor
- [C static variables (cppreference)](https://en.cppreference.com/w/c/language/storage_duration) — explains storage duration and lifetime of `static` locals
- [Memory management in C — malloc / free](https://en.cppreference.com/w/c/memory) — reference for dynamic allocation functions used throughout
- [42 Docs — get_next_line subject](https://cdn.intra.42.fr/pdf/pdf/229792/en.subject.pdf) — official project specification
- *The C Programming Language*, Kernighan & Ritchie (2nd ed.) — foundational reading for file I/O and string handling in C

### AI usage

Claude (Anthropic) was used during this project for the following tasks:

- **Generating this README** — the full content of this file was produced by Claude based on a code review of the three source files (`get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`). Claude analysed the algorithm, identified the design decisions, and structured the documentation according to the 42 README specification.
- **No code was generated by AI** — all `.c` and `.h` files were written by the student. AI was not used to write, debug, or refactor any part of the implementation.
