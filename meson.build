project('musicCatalog', ['cpp', 'c'])

subdir('lib/sqlite')

cmake = import('cmake')

taglib_opt = cmake.subproject_options()
taglib_opt.add_cmake_defines(
    {
        'BUILD_BINDINGS': false,
        'BUILD_TESTING': false,
        'BUILD_EXAMPLES': false,
        'BUILD_SHARED_LIBS': false
    }
)

taglib_proj = cmake.subproject(
    'taglib',
    options: taglib_opt
)

taglib_dep = taglib_proj.dependency('tag')

executable(
    'TagLibTest',
    'main.cpp',
    dependencies: [
        sqlite_dep,
        taglib_dep
    ])