SOURCES_ASSEMBLER = code/assembler/assembler.cpp code/assembler/assembler_manager.cpp code/assembler/assembler_func.cpp code/lib.cpp
OBJECTS_ASSEMBLER := $(addprefix $(OBJ_DIR)/, $(SOURCES_ASSEMBLER:.cpp=.o))
TARGET_ASSEMBLER = $(TARGET_DIR)/asm.out

$(TARGET_ASSEMBLER): $(OBJECTS_ASSEMBLER) | $(TARGET_DIR)
	@$(CC) $(DEBUG_FLAGS) $(CFLAGS) $^ -o $@
	@echo "LINKED ASSEMBLER"

asm: $(TARGET_ASSEMBLER)	
	@./$(TARGET_ASSEMBLER)

asm_b: $(TARGET_ASSEMBLER)