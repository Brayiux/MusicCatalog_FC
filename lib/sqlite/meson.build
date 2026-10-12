sqlite_source = [
    'sqlite3.c'
]
sqlite_headers = [
    'sqlite3.h'
]

sqlite_lib = library('sqlite3', sqlite_source, sqlite_headers)
sqlite_dep = declare_dependency(
    include_directories: include_directories('.'),
    link_with: sqlite_lib
)