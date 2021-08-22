set(CMAKE_C_COMPILER "/home/gaojies/Tools/riscv64-linux-x86_64-20210512/bin/riscv64-unknown-linux-gnu-gcc")
set(CMAKE_C_COMPILER_ARG1 "")
set(CMAKE_C_COMPILER_ID "GNU")
set(CMAKE_C_COMPILER_VERSION "10.2.0")
set(CMAKE_C_PLATFORM_ID "Linux")

set(CMAKE_AR "/home/gaojies/Tools/riscv64-linux-x86_64-20210512/bin/riscv64-unknown-linux-gnu-ar")
set(CMAKE_RANLIB "/home/gaojies/Tools/riscv64-linux-x86_64-20210512/bin/riscv64-unknown-linux-gnu-ranlib")
set(CMAKE_LINKER "/home/gaojies/Tools/riscv64-linux-x86_64-20210512/bin/riscv64-unknown-linux-gnu-ld")
set(CMAKE_COMPILER_IS_GNUCC 1)
set(CMAKE_C_COMPILER_LOADED 1)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_C_ABI_COMPILED TRUE)
set(CMAKE_COMPILER_IS_MINGW )
set(CMAKE_COMPILER_IS_CYGWIN )
if(CMAKE_COMPILER_IS_CYGWIN)
  set(CYGWIN 1)
  set(UNIX 1)
endif()

set(CMAKE_C_COMPILER_ENV_VAR "CC")

if(CMAKE_COMPILER_IS_MINGW)
  set(MINGW 1)
endif()
set(CMAKE_C_COMPILER_ID_RUN 1)
set(CMAKE_C_SOURCE_FILE_EXTENSIONS c)
set(CMAKE_C_IGNORE_EXTENSIONS h;H;o;O;obj;OBJ;def;DEF;rc;RC)
set(CMAKE_C_LINKER_PREFERENCE 10)

# Save compiler ABI information.
set(CMAKE_C_SIZEOF_DATA_PTR "8")
set(CMAKE_C_COMPILER_ABI "ELF")
set(CMAKE_C_LIBRARY_ARCHITECTURE "")

if(CMAKE_C_SIZEOF_DATA_PTR)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_C_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_C_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_C_COMPILER_ABI}")
endif()

if(CMAKE_C_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "")
endif()




set(CMAKE_C_IMPLICIT_LINK_LIBRARIES "m;c")
set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES "/home/gaojies/Tools/riscv64-linux-x86_64-20210512/lib/gcc/riscv64-unknown-linux-gnu/10.2.0/lib64vxthead/lp64d;/home/gaojies/Tools/riscv64-linux-x86_64-20210512/riscv64-unknown-linux-gnu/lib64vxthead/lp64d;/home/gaojies/Tools/riscv64-linux-x86_64-20210512/sysroot/lib64vxthead/lp64d;/home/gaojies/Tools/riscv64-linux-x86_64-20210512/lib/gcc/riscv64-unknown-linux-gnu/10.2.0;/home/gaojies/Tools/riscv64-linux-x86_64-20210512/lib/gcc;/home/gaojies/Tools/riscv64-linux-x86_64-20210512/riscv64-unknown-linux-gnu/lib;/home/gaojies/Tools/riscv64-linux-x86_64-20210512/sysroot/usr/lib64vxthead/lp64d;/home/gaojies/Tools/riscv64-linux-x86_64-20210512/sysroot/lib")
set(CMAKE_C_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")



