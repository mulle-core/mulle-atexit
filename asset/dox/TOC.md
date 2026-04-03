# mulle-atexit Library Documentation for AI
<!-- Keywords: lifecycle, cleanup -->

## 1. Introduction & Purpose

`mulle-atexit` provides a robust, thread-safe replacement for the standard C `atexit` function. The standard `atexit` is problematic in environments with dynamically loaded and unloaded shared libraries (e.g., plugins), as handlers registered in a library may be called after the library has already been unloaded, leading to crashes.

This library solves that problem by implementing an `atexit` mechanism that is aware of shared library boundaries. It ensures that handlers are executed correctly when a library is unloaded, not just when the entire process exits. It is designed to be statically linked into the main executable to provide a single, centralized exit handler registry for the entire process and all its dynamic components.

## 2. Key Concepts & Design Philosophy

- **Centralized Registry:** The core of the library is a global, thread-safe registry of exit handlers. By statically linking `mulle-atexit` into the main executable and forcing the linker to load the entire archive, this registry becomes the single source of truth for all `atexit` calls within the process.
- **Scope Awareness:** Unlike the standard `atexit`, `mulle-atexit` is designed to be called from shared libraries. It maintains its own list of handlers.
- **Explicit Invocation:** The registered handlers are not called automatically by the system `exit()`. Instead, they must be invoked explicitly by calling `mulle_atexit_execute` at the appropriate time (e.g., from a `main` function's `atexit` handler, or from a library's unload function).
- **Thread Safety:** The registry is protected by a mutex (`mulle-thread`), making it safe to register handlers from multiple threads concurrently.

## 3. Core API & Data Structures

The API is simple and consists of two main functions and a few variants, all exposed through `mulle-atexit.h`. It does not expose any public data structures.

### 3.1. `mulle-atexit.h`

#### Handler Registration
- `mulle_atexit(function)`: Registers a function to be called upon exit. The function must take no arguments and return `void`. This is the direct replacement for the standard `atexit`.
- `mulle_atexit_param(function, arg)`: Registers a function that takes a single `void *` argument. The `arg` is passed to the function when it is called.

#### Handler Execution
- `mulle_atexit_execute(void)`: Executes all registered handlers in Last-In, First-Out (LIFO) order. After execution, the list of handlers is cleared. This function should be called when the process or library is shutting down.

#### Global Control
- `mulle_atexit_is_global(void)`: A function that can be weakly linked. If another `mulle-atexit` instance is already loaded (e.g., in the main executable), this function will be replaced by the primary one, allowing a shared library to detect if it should use its own local `atexit` handlers or the global ones.

## 4. Performance Characteristics

- **Registration:** Registering a handler is a fast operation. It involves acquiring a mutex, allocating a small node for the handler, and adding it to a linked list. The overhead is minimal.
- **Execution:** Execution time is proportional to the number of registered handlers.
- **Memory Usage:** Memory usage is minimal, growing linearly with the number of registered handlers. Each handler requires a small amount of memory for its node in the registry.
- **Thread-Safety:** The library is fully thread-safe.

## 5. AI Usage Recommendations & Patterns

- **Linking:** It is **critical** that `mulle-atexit` is statically linked into the main executable. Furthermore, linker flags must be used to ensure the entire library is included and its symbols are exported, preventing it from being optimized away.
  - **Linux:** `-Wl,--whole-archive -lmulle-atexit -Wl,--no-whole-archive -Wl,--export-dynamic`
  - **macOS:** `-Wl,-force_load,path/to/libmulle-atexit.a`
- **Main Executable Setup:** The main executable should register `mulle_atexit_execute` with the *standard* `atexit` to ensure all registered `mulle-atexit` handlers are called on normal process termination.
  ```c
  #include <mulle-atexit/mulle-atexit.h>
  #include <stdlib.h>

  int main(void) {
      atexit(mulle_atexit_execute);
      // ... rest of main ...
  }
  ```
- **Shared Library Usage:** Shared libraries should link against `mulle-atexit` and use `mulle_atexit(my_cleanup_function)` in their constructor or initialization functions. They should **not** call `mulle_atexit_execute` themselves if a global instance is present.

## 6. Integration Examples

### Example 1: Registering and Executing Handlers

This example shows the basic usage pattern within a main executable. It registers two handlers and ensures they are called at exit.
*Source: `test/10_atexit/atexit.c`*
```c
#include <mulle-atexit/mulle-atexit.h>
#include <stdio.h>
#include <stdlib.h>

static void final_cleanup(void)
{
    printf("Final cleanup function called.\n");
}

static void resource_cleanup(void *resource)
{
    printf("Cleaning up resource: %s\n", (char *)resource);
}

// This function simulates the setup in a main executable.
void setup_main_exit_handler(void)
{
    // This ensures all mulle_atexit handlers are called when main exits.
    if (atexit(mulle_atexit_execute))
    {
        fprintf(stderr, "Failed to register atexit handler\n");
        abort();
    }
}

int main(int argc, char *argv[])
{
    setup_main_exit_handler();

    printf("Program starting.\n");

    // Register a handler with a parameter.
    mulle_atexit_param(resource_cleanup, "My Resource");

    // Register a simple handler.
    mulle_atexit(final_cleanup);

    printf("Program finishing. Handlers will be called by the system atexit.\n");

    // When main returns, the standard atexit mechanism will call
    // mulle_atexit_execute, which in turn calls our two handlers
    // in reverse order of registration.
    return 0;
}

/*
Expected Output:
Program starting.
Program finishing. Handlers will be called by the system atexit.
Final cleanup function called.
Cleaning up resource: My Resource
*/
