SOURCES_DISASSEMBLER = code/disassembler/disassembler.cpp code/disassembler/disassembler_manager.cpp code/disassembler/disassembler_func.cpp code/lib.cpp
OBJECTS_DISASSEMBLER := $(addprefix $(OBJ_DIR)/, $(SOURCES_DISASSEMBLER:.cpp=.o))
TARGET_DISASSEMBLER = $(TARGET_DIR)/dis.out

$(TARGET_DISASSEMBLER): $(OBJECTS_DISASSEMBLER) | $(TARGET_DIR)
	@$(CC) $(DEBUG_FLAGS) $(CFLAGS) $^ -o $@
	@echo "LINKED DISASSEMBLER"

dis: $(TARGET_DISASSEMBLER)
	@./$(TARGET_DISASSEMBLER)

dis_b: $(TARGET_DISASSEMBLER)