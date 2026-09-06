## 0.2.0




feature: support consuming mulle-atexit via CMake `add_subdirectory`

* CMake consumers can now `add_subdirectory` the project and link against the library target; dependencies resolve to CMake targets before falling back to `find_library`
* the object library now inherits include/usage requirements and build order from dependency targets
* dependency targets are exported transitively via INTERFACE so a single `target_link_libraries` works for consumers
* with multi-reflect enabled, only the active reflect header tree is included at build time



* add BSD license headers to all previously unlicensed source files
* fix copyright year to 2019 in the public header
* reorganize API documentation into asset/dox/api/toc/

* add BSD license headers to all previously unlicensed source files
* fix copyright year to 2019 in the public header
* reorganize API documentation into asset/dox/api/toc/
