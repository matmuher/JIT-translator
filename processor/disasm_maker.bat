g++ -c disasm.cpp disasm_funks.cpp ..\hamlet\d_hamlet_functions.cpp ..\strings\string_functions.cpp ..\stack\stack.cpp ..\memory_free\elephant_calloc.cpp ..\square_solver\beer_equation.cpp

g++ disasm.o disasm_funks.o d_hamlet_functions.o string_functions.o stack.o elephant_calloc.o beer_equation.o -o disasm

pause