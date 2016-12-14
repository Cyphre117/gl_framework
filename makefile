# The C compiler
CC = clang
# Extra flags given to the C compiler
#CFLAGS

# The C++ Compiler
CXX = clang++
# Extra flags given to the C++ compiler
CXXFLAGS = -std=c++11 -Wall

# Extra Flags for the C Pre-Procesor
CPPFLAGS = -I ./$(SRC_DIR) -I ./lib

# Library flags given to the compiler relevant to the linking stage
LDLIBS = -framework SDL2 -framework OpenGL -framework OpenAL lib/GL/libGLEW.a

SRC_DIR = src
BUILD = build
RELEASE = release
EXE = engine.out
APP_NAME = Engine

# Add folders for make to search
#VPATH = src
#VPATH = src/meshes

# Every cpp file in this directory
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Every cpp file in any sub-directory
SRCS += $(wildcard $(SRC_DIR)/*/*.cpp)

# take each source file and get the associated object file
OBJS=$(subst .cpp,.o,$(SRCS))
# Remove any nested directory names
OBJS:=$(notdir $(OBJS))
# Add the prefix for the build directory
OBJS:=$(addprefix $(BUILD)/,$(OBJS))

# Find all the header files too
HDRS:=$(wildcard $(SRC_DIR)/*.h)
HDRS:=$(wildcard $(SRC_DIR)/*/*.h)

# simply calling 'make' should compile a debug version
default: debug

debug: $(OBJS)
	@echo "Compiling \033[0;32m$(EXE)\033[0;39m for debuging"
	@$(CXX) $(LDLIBS) $(OBJS) -o $(EXE)

$(OBJS):
	@echo "Compiling \033[0;36m$<\033[0;39m"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -g -c $< -o $@

depend: .depend

#Pass the -MM flag to the compiler to generate dependancy rules for each makefile
.depend: $(SRCS) $(HDRS)
	@echo "\033[0;33mRebuilding dependencies\033[0;39m"
	@rm -f ./.depend
	@$(CXX) $(CPPFLAGS) -MM $(SRCS)>>./.depend;
	@# remove the backslash newlines in the generated file
	@perl -i -pe 's/\\\n//g' .depend
	@# prepend the buid directory to each output object file
	@perl -i -pe 's/^/$(BUILD)\//' .depend


# The .app creating suff is done with help from this wonderful link
# http://joseph-long.com/writing/app-bundles-with-a-makefile/

app: $(OBJS) clean_app create_app
	@echo "Compiling \033[0;32m$(EXE)\033[0;39m for release"
	@# TODO: the -O4 flag here doesn't actually apply optimisations as this is during linking, i think it needs to be done during compilation
	@$(CXX) $(LDLIBS) $(OBJS) -O4 -o ./$(RELEASE)/$(APP_NAME).app/Contents/MacOS/$(APP_NAME)
	@# Change the search path for SDL2 so the executable knows where to find it
	@install_name_tool -change @rpath/SDL2.framework/Versions/A/SDL2 @executable_path/../Frameworks/SDL2.framework/Versions/A/SDL2 ./$(RELEASE)/$(APP_NAME).app/Contents/MacOS/$(APP_NAME)
	@echo "\033[0;32mDone\033[0;39m"

clean_app:
	rm -rf "./$(RELEASE)/$(APP_NAME).app/"

create_app:
	@echo "\033[0;33mCreating app structure\033[0;39m"
	@mkdir -p "./$(RELEASE)/$(APP_NAME).app"/Contents/{MacOS,Resources,Frameworks}
	@cp Info.plist "./$(RELEASE)/$(APP_NAME).app/Contents/"
	@sed -e "s/APP_NAME/$(APP_NAME)/g" -i "" "./$(RELEASE)/$(APP_NAME).app/Contents/Info.plist"
	@echo "\033[0;33mBundling resources\033[0;39m"
	@cp -R "/Library/Frameworks/SDL2.framework" "./$(RELEASE)/$(APP_NAME).app/Contents/Frameworks/"
	@cp -R ./images "./$(RELEASE)/$(APP_NAME).app/Contents/Resources/"
	@cp -R ./shaders "./$(RELEASE)/$(APP_NAME).app/Contents/Resources/"

clean:
	@rm -f $(BUILD)/*.o
	@rm -f $(EXE)
	@rm -f .depend

-include .depend
