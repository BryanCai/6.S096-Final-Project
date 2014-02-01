## Look in src/demo for the demo main files
vpath %.cpp $(DEV_DIR)/src/visualization

## The result of our nbody-demo compile: an executable
## named list-demo.x which will run our program
TARGET_PROJ_VISUALIZATION := $(INSTALL_DIR)/bin/$(PROJ_NAME)-visualization.x
BUILD_DIR_PROJ_VISUALIZATION := $(BUILD_DIR)/$(PROJ_NAME)-visualization

## nbody-demo.x depends on libnbody.a having been created first
$(TARGET_PROJ_VISUALIZATION) : $(INSTALL_DIR)/lib/lib$(PROJ_NAME).a

## Link our nbody-demo.x executable with libnbody.a
## (here is where you add any extra third-party library linkings needed)
$(TARGET_PROJ_VISUALIZATION) : LDFLAGS += $(INSTALL_DIR)/lib/lib$(PROJ_NAME).a \
																 -lGL -lglut -lGLEW -lglfw \
                                                                								 -I$(GLFW_DIR)/lib/ \
								                                                                 -I$(GLM_DIR)/ \
								                                                                 -I$(GTEST_DIR)/include \
																  $(GLLOAD_DIR)/lib/libglload.a \

## Add any more files to this list
OBJECTS_PROJ_VISUALIZATION := 
        $(BUILD_DIR_PROJ_VISUALIZATION/main.o \
        $(BUILD_DIR_PROJ_VISUALIZATION/Graphics.o \
	#$(BUILD_DIR_PROJ_DEMO)/otherFiles.o \
	#$(BUILD_DIR_PROJ_DEMO)/moreOtherFiles.o \
## Add any more files you like!

$(INSTALL_DIR)/include/$(PROJ_NAME)-visualization ::
	$(MKDIR) $@
	$(RSYNC) $(DEV_DIR)/include/$(PROJ_NAME)-visualization/ $@/

## Add the public include files to the list of artifacts
## to be cleaned up on a 'make clean'
ARTIFACTS += $(INSTALL_DIR)/include/$(PROJ_NAME)-visualization
