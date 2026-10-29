subdir('entities')
subdir('exceptions')
subdir('schema')
subdir('connection')

model_lib = static_library(
    'model',
    [
        'Model.cpp'
    ],
    dependencies: [entities_dep, exceptions_dep, schema_dep, connection_dep],
    install: false
)

model_dep = declare_dependency(
    link_with: model_lib,
    dependencies: [entities_dep, exceptions_dep, schema_dep, connection_dep]
)