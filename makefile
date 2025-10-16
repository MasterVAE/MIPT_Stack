FFMPEG_LIBS = -lavformat -lavcodec -lavutil -lswscale
PNG_LIBS = -lpng -lz
MATH_LIB = -lm

SOURCES_ASSEMBLER = $(wildcard code/assembler/*.cpp) code/lib.cpp
SOURCES_DISASSEMBLER = $(wildcard code/disassembler/*.cpp) code/lib.cpp
SOURCES_PROCESSOR = $(wildcard code/processor/*.cpp) code/lib.cpp
SOURCES_PNG = $(wildcard code/png_parser/*.cpp)

OBJ_DIR = obj
SOURCE_DIR = ./

OBJECTS_ASSEMBLER := $(addprefix $(OBJ_DIR)/, $(SOURCES_ASSEMBLER:.cpp=.o))
SOURCES_ASSEMBLER  := $(addprefix $(SOURCE_DIR)/, $(SOURCES_ASSEMBLER))

OBJECTS_DISASSEMBLER := $(addprefix $(OBJ_DIR)/, $(SOURCES_DISASSEMBLER:.cpp=.o))
SOURCES_DISASSEMBLER  := $(addprefix $(SOURCE_DIR)/, $(SOURCES_DISASSEMBLER))

OBJECTS_PROCESSOR := $(addprefix $(OBJ_DIR)/, $(SOURCES_PROCESSOR:.cpp=.o))
SOURCES_PROCESSOR := $(addprefix $(SOURCE_DIR)/, $(SOURCES_PROCESSOR))

OBJECTS_PNG := $(addprefix $(OBJ_DIR)/, $(SOURCES_PNG:.cpp=.o))
SOURCES_PNG := $(addprefix $(SOURCE_DIR)/, $(SOURCES_PNG))

HEADERS = $(wildcard $(SOURCE_DIR)/*.h)

TARGET_ASSEMBLER = build/asm.out
TARGET_DISASSEMBLER = build/dis.out
TARGET_PROCESSOR = build/spu.out
TARGET_PNG = build/png.out

CC = g++

	CFLAGS = -D _DEBUG -ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=30000 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

.DEFAULT_GOAL := all

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_ASSEMBLER): $(OBJECTS_ASSEMBLER)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "LINKED ASSEMBLER"

$(TARGET_DISASSEMBLER): $(OBJECTS_DISASSEMBLER)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "LINKED DISASSEMBLER"

$(TARGET_PROCESSOR): $(OBJECTS_PROCESSOR)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "LINKED PROCESSOR"

$(TARGET_PNG): $(OBJECTS_PNG)
	@$(CC) $(CFLAGS) $^ -o $@ $(FFMPEG_LIBS)
	@echo "LINKED PNG"

asm: $(TARGET_ASSEMBLER)	
	@./$(TARGET_ASSEMBLER)
$(ALL_LIBS)
dis: $(TARGET_DISASSEMBLER)
	@./$(TARGET_DISASSEMBLER)

spu: $(TARGET_PROCESSOR)
	@./$(TARGET_PROCESSOR)

png: $(TARGET_PNG)
	@./$(TARGET_PNG)

asm_b: $(TARGET_ASSEMBLER)	
dis_b: $(TARGET_DISASSEMBLER)
spu_b: $(TARGET_PROCESSOR)
png_b: $(TARGET_PNG)

all: asm_b spu_b dis_b png_b

clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(TARGET_ASSEMBLER)
	@rm -f $(TARGET_PROCESSOR)
	@rm -f $(TARGET_DISASSEMBLER)
	@rm -f $(TARGET_PNG)
	@echo "CLEANED"
