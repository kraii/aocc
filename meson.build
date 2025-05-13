project('aocc', 'c')

fs = import('fs')

test_resources = ['example.txt']
foreach r : test_resources
    fs.copyfile('test/' + r)
endforeach

tests = ['strings', 'vector', 'files']

cc = meson.get_compiler('c')
lib_criterion = cc.find_library('criterion', required: true)

test_deps = [lib_criterion]



vector_lib = library('vector', 'vector.c')
strings_lib = library('strings', 'strings.c', link_with: [vector_lib])
files_lib = library('files', 'files.c', link_with: [vector_lib, strings_lib])


cal_2024 = [ 'day1', 'day2' ]
foreach d : cal_2024
    name = d + '_2024'
    executable(name,
               name + '.c',
               link_with: [vector_lib, strings_lib, files_lib]
    )
endforeach


foreach t : tests
    e = executable(t + '_test',
                t + '_test.c',
                dependencies: test_deps,
                link_with: [vector_lib, strings_lib, files_lib]
    )
    test(t, e)
endforeach