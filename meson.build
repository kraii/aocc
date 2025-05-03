project('aocc', 'c')

executable('example', 'example.c')

tests = ['strings_test']

cc = meson.get_compiler('c')
lib_criterion = cc.find_library('criterion', required: true)

foreach t : tests
    e = executable(t,
                t + '.c',
                dependencies: [lib_criterion]
    )
    test(t, e)
endforeach