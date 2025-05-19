project('aocc', 'c')

fs = import('fs')

test_resources = ['example.txt', 'example_grid.txt']
foreach r : test_resources
    fs.copyfile('test/' + r)
endforeach

tests = ['strings', 'vector', 'files', 'regex', 'grid']

cc = meson.get_compiler('c')
lib_criterion = cc.find_library('criterion', required : true)
lib_pcre2 = cc.find_library('pcre2-8', required : true)

test_deps = [lib_criterion]



vector_lib = library('vector', 'vector.c')
strings_lib = library('strings', 'strings.c', link_with : [vector_lib])
files_lib = library('files', 'files.c', link_with : [vector_lib, strings_lib])
regex_lib = library('regex', 'regex.c', link_with : [vector_lib, strings_lib], dependencies: [lib_pcre2])
grid_lib = library('grid', 'grid.c', link_with: [files_lib, strings_lib, vector_lib])

cal_2024 = ['day1', 'day2', 'day3']
foreach d : cal_2024
    name = d + '_2024'
    executable(
        name,
        name + '.c',
        link_with : [vector_lib, strings_lib, files_lib, regex_lib, grid_lib],
        dependencies: [lib_pcre2]
    )
endforeach


foreach t : tests
    e = executable(t + '_test',
                   t + '_test.c',
                   dependencies : test_deps,
                   link_with : [vector_lib, strings_lib, files_lib, regex_lib, grid_lib]
    )
    test(t, e)
endforeach