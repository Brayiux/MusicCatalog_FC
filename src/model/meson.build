subdir('entities')
subdir('exceptions')
subdir('connection')
subdir('schema')

model_lib = static_library(
    'model',
    [
        'Model.cpp'
    ],
    dependencies: [connection_dep, entities_dep, exceptions_dep],
    install: false
)

model_dep = declare_dependency(
    link_with: model_lib,
    dependencies: [connection_dep, entities_dep, exceptions_dep]
)