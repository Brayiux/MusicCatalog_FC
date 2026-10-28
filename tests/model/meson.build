connection_tests_exe = executable(
    'model_connection_tests',
    [
        'connection_tests.cpp'
    ],
    dependencies: model_dep,
    include_directories: doctest_dir
)

test('model_connection_tests', connection_tests_exe)