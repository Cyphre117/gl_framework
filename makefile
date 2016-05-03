CC=gcc
CXX=c++
CXXFLAGS=-std=c++11 -Wall
CPPFLAGS=-iquote $(SRC_DIR) -iquote $(SRC_DIR)/meshes/ -I ./lib
LDLIBS=-framework SDL2 -framework OpenGL lib/GL/glew.o
SRC_DIR=src
BUILD=build
EXE=engine.out

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
HDRS:=$(wildcard ./*.h)
HDRS:=$(wildcard ./*/*.h)

all: $(OBJS)
	@echo "Compiling \033[0;32m$(EXE)\033[0;39m"
	@$(CXX) $(LDLIBS) $(OBJS) -o $(EXE)

$(OBJS):
	@echo "Compiling \033[0;36m$<\033[0;39m"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

depend: .depend

#Pass the -MM flag to the compiler to generate dependancy rules for each makefile
.depend: $(SRCS)
	@echo "\033[0;33mRebuilding dependencies\033[0;39m"
	@rm -f ./.depend
	@$(CXX) $(CPPFLAGS) -MM $^>>./.depend;
	@# remove the backslash newlines in the generated file
	@perl -i -pe 's/\\\n//g' .depend
	@perl -i -pe 's/^/$(BUILD)\//' .depend

clean:
	@rm -f $(BUILD)/*.o
	@rm -f $(EXE)
	@rm -f .depend

-include .depend