exceptions_lib = static_library(
    'exceptions',
    [
        'NotEnoughInfoException.cpp'
    ],
    install: false
)

exceptions_dep = declare_dependency(
    link_with: exceptions_lib,
    include_directories: include_directories('.')
)