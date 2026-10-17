model_lib = static_library(
    'model',
    [
        'MusicCatalogDAO.cpp'
    ],
    install: false
)

model_dep = declare_dependency(
    link_with: model_lib,
    include_directories: include_directories('include'),
    dependencies: sqlite_dep
)