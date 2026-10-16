sqlite_lib = static_library(
    'sqlite3',
    'sqlite3.c',
    install: false
)

sqlite_dep = declare_dependency(
    link_with: sqlite_lib,
    include_directories: include_directories('.')
)