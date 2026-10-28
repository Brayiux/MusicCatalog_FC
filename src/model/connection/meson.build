connection_lib = static_library(
    'connection',
    [
        'MusicCatalogDAO.cpp'
    ],
    dependencies: [entities_dep, sqlite_dep],
    install: false
)

connection_dep = declare_dependency(
    link_with: connection_lib,
    include_directories: include_directories('include'),
    dependencies: [entities_dep, sqlite_dep]
)