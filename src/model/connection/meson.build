connection_lib = static_library(
    'connection',
    [
        'MusicCatalogDAO.cpp'
    ],
    dependencies: sqlite_dep,
    install: false
)

connection_dep = declare_dependency(
    link_with: connection_lib,
    include_directories: include_directories('.'),
    dependencies: sqlite_dep
)