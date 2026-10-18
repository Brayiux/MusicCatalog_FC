schema_lib = static_library(
    'schema',
    'Schema.cpp',
    install: false
)

schema_dep = declare_dependency(
    link_with: schema_lib,
    include_directories: include_directories('.')
)