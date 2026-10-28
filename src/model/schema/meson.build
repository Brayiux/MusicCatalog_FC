schema_lib = static_library(
    'schema',
    'Schema.cpp',
    dependencies: sqlite_dep,
    install: false
)

schema_dep = declare_dependency(
    link_with: schema_lib,
    include_directories: include_directories('.'),
    dependencies: sqlite_dep
)