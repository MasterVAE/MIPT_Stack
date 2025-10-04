OBJ_DIR = obj
SOURCE_DIR = .

CC = g++
CFLAGS = -D _DEBUG -ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

.DEFAULT_GOAL := all

SPU_SOURCES = processor.cpp code/stack.cpp code/assembler_read.cpp code/processor_functions.cpp
SPU_OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SPU_SOURCES))
SPU_TARGET = build/spu.out

ASM_SOURCES = assembler.cpp code/assembler_read.cpp
ASM_OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(ASM_SOURCES))
ASM_TARGET = build/asm.out

DIS_SOURCES = disassembler.cpp code/assembler_read.cpp
DIS_OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(DIS_SOURCES))
DIS_TARGET = build/dis.out

spu: $(SPU_TARGET)

$(SPU_TARGET): $(SPU_OBJECTS)
	@$(CC) $(CFLAGS) $^ -o $@

asm: $(ASM_TARGET)

$(ASM_TARGET): $(ASM_OBJECTS)
	@$(CC) $(CFLAGS) $^ -o $@

dis: $(DIS_TARGET)

$(DIS_TARGET): $(DIS_OBJECTS)
	@$(CC) $(CFLAGS) $^ -o $@

all: spu asm dis
	@echo "All targets built successfully"
	@rm -rf $(OBJ_DIR)
	@echo "Removed object directory after build."

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp | make_build_dir
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled Successfully $<"

make_build_dir:
	@mkdir -p $(OBJ_DIR)
	@echo "Created build directory: $(OBJ_DIR)"

clean:
	@rm -rf $(OBJ_DIR) *.out
	@echo "Cleaned Successfully"

build: all
rebuild: clean all