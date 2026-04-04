## 0.1.0





feature: make atexit return status and improve initialization & Windows handling

* **BREAKING**: `mulle_atexit` is now an inline wrapper (delegates to `_mulle_atexit);` code linking against the previous exported `mulle_atexit` symbol must be recompiled or call `_mulle_atexit` directly.
* `_mulle_atexit` now returns an int status, ignores NULL callbacks, and uses improved thread-safe initialization and mutex handling to avoid races.
* Add compile-time error to prevent building as a shared library, better debug traces, and Windows-specific dynamic lookup / DllMain debug support.
