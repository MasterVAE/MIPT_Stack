FFMPEG_LIBS = -lavformat -lavcodec -lavutil -lswscale

SOURCES_PNG = code/video_parser/video_parser.cpp
OBJECTS_PNG := $(addprefix $(OBJ_DIR)/, $(SOURCES_PNG:.cpp=.o))
TARGET_PNG = $(TARGET_DIR)/png.out

$(TARGET_PNG): $(OBJECTS_PNG) | $(TARGET_DIR)
	@$(CC) $(DEBUG_FLAGS) $(CFLAGS) $^ -o $@ $(FFMPEG_LIBS)
	@echo "LINKED PNG"

png: $(TARGET_PNG)
	@./$(TARGET_PNG)

png_b: $(TARGET_PNG)