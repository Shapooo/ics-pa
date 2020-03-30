project(cputest)

add_executable(add tests/add.c)
target_link_libraries(add
    am klib)

add_executable(add-longlong tests/add-longlong.c)
target_link_libraries(add-longlong
    am klib)

add_executable(bit tests/bit.c)
target_link_libraries(bit
    am klib)

add_executable(bubble-sort tests/bubble-sort.c)
target_link_libraries(bubble-sort
    am klib)

add_executable(div tests/div.c)
target_link_libraries(div
    am klib)

add_executable(dummy tests/dummy.c)
target_link_libraries(dummy
    am klib)

add_executable(fact tests/fact.c)
target_link_libraries(fact
    am klib)

add_executable(fib tests/fib.c)
target_link_libraries(fib
    am klib)

add_executable(goldbach tests/goldbach.c)
target_link_libraries(goldbach
    am klib)

add_executable(hello-str tests/hello-str.c)
target_link_libraries(hello-str
    am klib)

add_executable(if-else tests/if-else.c)
target_link_libraries(if-else
    am klib)

add_executable(leap-year tests/leap-year.c)
target_link_libraries(leap-year
    am klib)

add_executable(load-store tests/load-store.c)
target_link_libraries(load-store
    am klib)

add_executable(matrix-mul tests/matrix-mul.c)
target_link_libraries(matrix-mul
    am klib)

add_executable(max tests/max.c)
target_link_libraries(max
    am klib)

add_executable(min3 tests/min3.c)
target_link_libraries(min3
    am klib)

add_executable(mov-c tests/mov-c.c)
target_link_libraries(mov-c
    am klib)

add_executable(movsx tests/movsx.c)
target_link_libraries(movsx
    am klib)

add_executable(mul-longlong tests/mul-longlong.c)
target_link_libraries(mul-longlong
    am klib)

add_executable(pascal tests/pascal.c)
target_link_libraries(pascal
    am klib)

add_executable(prime tests/prime.c)
target_link_libraries(prime
    am klib)

add_executable(quick-sort tests/quick-sort.c)
target_link_libraries(quick-sort
    am klib)

add_executable(recursion tests/recursion.c)
target_link_libraries(recursion
    am klib)

add_executable(select-sort tests/select-sort.c)
target_link_libraries(select-sort
    am klib)

add_executable(shift tests/shift.c)
target_link_libraries(shift
    am klib)

add_executable(shuixianhua tests/shuixianhua.c)
target_link_libraries(shuixianhua
    am klib)

add_executable(string tests/string.c)
target_link_libraries(string
    am klib)

add_executable(sub-longlong tests/sub-longlong.c)
target_link_libraries(sub-longlong
    am klib)

add_executable(sum tests/sum.c)
target_link_libraries(sum
    am klib)

add_executable(switch tests/switch.c)
target_link_libraries(switch
    am klib)

add_executable(to-lower-case tests/to-lower-case.c)
target_link_libraries(to-lower-case
    am klib)

add_executable(unalign tests/unalign.c)
target_link_libraries(unalign
    am klib)

add_executable(wanshu tests/wanshu.c)
target_link_libraries(wanshu
    am klib)
