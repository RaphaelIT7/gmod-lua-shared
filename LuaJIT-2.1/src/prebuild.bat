luajit ..\dynasm\dynasm.lua -LN -D WIN -D JIT -D FFI -D ENDIAN_LE -D FPU -o dynasm_arm.c vm_arm.dasc
luajit ..\dynasm\dynasm.lua -LN -D WIN -D JIT -D FFI -D ENDIAN_LE -D FPU -D P64 -o dynasm_arm64.c vm_arm64.dasc
luajit ..\dynasm\dynasm.lua -LN -D WIN -D JIT -D FFI -D ENDIAN_LE -D FPU -o dynasm_mips.c vm_mips.dasc
luajit ..\dynasm\dynasm.lua -LN -D WIN -D JIT -D FFI -D ENDIAN_LE -D FPU -D P64 -o dynasm_mips64.c vm_mips64.dasc
luajit ..\dynasm\dynasm.lua -LN -D WIN -D JIT -D FFI -D ENDIAN_LE -D FPU -D P64 -o dynasm_ppc.c vm_ppc.dasc
luajit ..\dynasm\dynasm.lua -LN -D WIN -D JIT -D FFI -D ENDIAN_LE -D FPU -D P64 -o dynasm_x64.c vm_x64.dasc
luajit ..\dynasm\dynasm.lua -LN -D WIN -D JIT -D FFI -D ENDIAN_LE -D FPU -o dynasm_x86.c vm_x86.dasc

buildvm -m peobj -o lj_vm.obj
buildvm -m bcdef -o lj_bcdef.h lib_base.c lib_math.c lib_bit.c lib_string.c lib_table.c lib_io.c lib_os.c lib_package.c lib_debug.c lib_jit.c lib_ffi.c lib_buffer.c
buildvm -m ffdef -o lj_ffdef.h lib_base.c lib_math.c lib_bit.c lib_string.c lib_table.c lib_io.c lib_os.c lib_package.c lib_debug.c lib_jit.c lib_ffi.c lib_buffer.c
buildvm -m libdef -o lj_libdef.h lib_base.c lib_math.c lib_bit.c lib_string.c lib_table.c lib_io.c lib_os.c lib_package.c lib_debug.c lib_jit.c lib_ffi.c lib_buffer.c
buildvm -m recdef -o lj_recdef.h lib_base.c lib_math.c lib_bit.c lib_string.c lib_table.c lib_io.c lib_os.c lib_package.c lib_debug.c lib_jit.c lib_ffi.c lib_buffer.c
buildvm -m vmdef -o jit\vmdef.lua lib_base.c lib_math.c lib_bit.c lib_string.c lib_table.c lib_io.c lib_os.c lib_package.c lib_debug.c lib_jit.c lib_ffi.c lib_buffer.c
buildvm -m folddef -o lj_folddef.h lj_opt_fold.c