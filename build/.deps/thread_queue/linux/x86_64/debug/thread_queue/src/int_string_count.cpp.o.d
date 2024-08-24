{
    files = {
        "thread_queue/src/int_string_count.cpp"
    },
    depfiles_gcc = "int_string_count.o: thread_queue/src/int_string_count.cpp  thread_queue/include/thread_queue/thread_queue.h\
",
    values = {
        "/usr/sbin/gcc",
        {
            "-m64",
            "-g",
            "-O0",
            "-std=c++2a",
            "-Ithread_queue/include",
            "-finput-charset=UTF-8"
        }
    }
}