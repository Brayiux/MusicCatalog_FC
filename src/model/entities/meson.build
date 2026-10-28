entities_lib = static_library(
    'entities',
    [
        'Entities.cpp'
    ],
    install: false
)

entities_dep = declare_dependency(
    link_with: entities_lib,
    include_directories: include_directories('include')
)