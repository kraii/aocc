project('aocc', 'c')

executable('example', 'example.c')

tests = ['strings', 'vector']

cc = meson.get_compiler('c')
lib_criterion = cc.find_library('criterion', required: true)

foreach t : tests
    e = executable(t,
                t + '_test.c',
                dependencies: [lib_criterion]
    )
    test(t, e)
endforeach