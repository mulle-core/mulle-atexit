# mulle-atexit Library Documentation for AI
<!-- Keywords: atexit, callbacks, lifecycle, LIFO, staticlink -->
## 1. Introduction & Purpose

- mulle-atexit reimplements and fixes the C runtime `atexit` handling for environments where shared-library unloading and symbol resolution break program-exit callbacks.
- Problem solved: ensures registered exit callbacks are reliably stored and executed (LIFO) even when libraries are linked statically into an executable or when shared-library unloading would otherwise lose callbacks.
- Key features: simple registration API, version helpers, thread-safe registration, constructor/destructor wiring, and explicit requirement to statically link the library into the final executable.

## 2. Key Concepts & Design Philosophy

- Deterministic LIFO execution of registered callbacks (matches standard atexit semantics).
- Library maintains its own dynamic stack of function pointers rather than relying solely on platform `atexit`, avoiding lost callbacks during shared-library unloads.
- Thread-safety using an internal mutex plus once-initialization.
- Small, dependency-minimal design that expects static linking into the executable; the library uses constructor/destructor hooks to ensure callbacks are processed.

## 3. Core API & Data Structures

This section is organized by the public header `mulle-atexit.h` (see `src/mulle-atexit.h`).

### 3.1. [mulle-atexit.h]

#### Version helpers
- MULLE__ATEXIT_VERSION: packed version (major<<20 | minor<<8 | patch).
- mulle_atexit_get_version_major()/minor()/patch(): inline helpers returning version components.
- mulle_atexit_get_version(): returns packed uint32_t version.

#### Types and registration
- typedef int mulle_atexit_function_t( void (*f)( void));
  - Signature type used for dynamic lookup on some platforms.

- int _mulle_atexit( void (*f)( void));
  - Core function that registers `f` to be called at program exit or unload.
  - Stores `f` in an internal dynamic array (LIFO order). Calling with NULL performs initialization/guarding behavior.

- static inline int mulle_atexit( void (*f)(void))
  - Portable wrapper: on Windows+dynamic builds it resolves `_mulle_atexit` via `mulle_dlsym_exe` and calls it; otherwise it calls `_mulle_atexit` directly.

#### Lifecycle and execution
- A constructor calls `_mulle_atexit(0)` to ensure initialization occurs early.
- If not relying on the system `atexit`, a destructor calls `run_exit_callbacks()` to execute registered callbacks on unload.
- Registration order is LIFO (last registered, first executed). Tests show registering `a` then `b` prints `b` then `a`.

## 4. Performance Characteristics

- Registration: amortized O(1). The internal array grows by doubling; initial capacity is 32.
- Execution: O(n) to run all callbacks.
- Memory: proportional to number of registered callbacks, with occasional reallocations on growth.
- Thread-safety: uses `mulle_thread_mutex_t` for protection and `mulle_thread_once_do` for one-time init.

## 5. AI Usage Recommendations & Patterns

- Best practices:
  - Use `mulle_atexit( fn);` for registering callbacks.
  - Statically link the library into the executable. Linux linker flags: `-Wl,--export-dynamic -Wl,--whole-archive`; macOS: `-force_load <library>`.
  - Use the public `mulle_atexit` wrapper; avoid direct dynamic symbol manipulation unless required on Windows.
- Common pitfalls:
  - Callbacks execute LIFO — do not expect FIFO behavior.
  - If the library is not linked in the recommended way, callbacks can be optimized away or not run.
- Idiomatic pattern: register small cleanup functions and let the executable arrange final invocation (e.g., register `mulle_atexit_execute` with the system `atexit` if desired).

## 6. Integration Examples

### Example 1: Creating and registering two callbacks (from tests)

```c
#include <mulle-atexit/mulle-atexit.h>
#include <stdio.h>

static void
a( void )
{
   printf( "a\n" );
   fflush( stdout );
}

static void
b( void )
{
   printf( "b\n" );
   fflush( stdout );
}

int
main( void )
{
   mulle_atexit( a );
   mulle_atexit( b );

   return( 0 );
}
```

- Expected output on exit: `b` then `a` (LIFO execution).

### Example 2: Minimal registration in library code

```c
#include <mulle-atexit/mulle-atexit.h>

static void
cleanup( void )
{
   /* release resources owned by the library */
}

void
library_init( void )
{
   /* called when library is initialized */
   mulle_atexit( cleanup );
}
```

## 7. Dependencies

- Relies on mulle-thread primitives (mutex and once) for thread-safety.
- Intended to be integrated and built using mulle-sde.

