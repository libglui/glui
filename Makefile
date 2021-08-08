.SUFFIXES: .cpp

#for sgi   -- comment out the lines below to use on HP
#CC=CC -g0 -o32
#CC=gcc

# Compiler options
OPTS=-g
OPTS=-O0
#OPTS=-O2

UNAME = $(shell uname)

CPPFLAGS+=-std=c++11

ifeq ($(UNAME), Linux)
CXX      ?= g++
CPPFLAGS += $(OPTS) -Wall -pedantic
LIBGL     = -lGLU -lGL
LIBS      = -lXmu -lXext -lX11 -lXi -lm

# One of the following options only...

# (1) OpenGLUT
# LIBGLUT   = -L/usr/X11R6/lib -lopenglut
# CPPFLAGS += -I/usr/X11R6/include -DGLUI_OPENGLUT

# (2) FreeGLUT
# LIBGLUT   = -L/usr/X11R6/lib -lfreeglut
# CPPFLAGS += -I/usr/X11R6/include -DGLUI_FREEGLUT

# (3) GLUT
LIBGLUT   = -L/usr/X11R6/lib -lglut
CPPFLAGS += -I/usr/X11R6/include
endif

ifeq ($(UNAME), Darwin)
CXX      ?= g++
CPPFLAGS += $(OPTS) -Wall -pedantic
LIBGL     = -framework OpenGL
LIBGLUT   = -framework GLUT
endif

#######################################

CPPFLAGS += -I./src -I./include

LIBGLUI = -L./lib -lglui

#######################################

GLUI_SRC = $(sort $(wildcard src/*.cpp))
GLUI_OBJ := $(patsubst %.cpp,%.o,$(GLUI_SRC))

GLUI_LIB = lib/libglui.a

GLUI_EXAMPLES = bin/example1 bin/example2 bin/example3 bin/example4 bin/example5 bin/example6

GLUI_TOOLS = bin/ppm2array

.PHONY: all setup examples tools clean depend doc doc-pdf doc-dist dist

all: setup $(GLUI_LIB) examples tools

setup:
	mkdir -p bin
	mkdir -p lib

examples: $(GLUI_EXAMPLES)

tools: $(GLUI_TOOLS)

bin/ppm2array: tools/ppm2array.cpp tools/ppm.cpp
	$(CXX) $(CPPFLAGS) -o $@ $^

bin/%: example/%.cpp $(GLUI_LIB)
	$(CXX) $(CPPFLAGS) -o $@ $<  $(LIBGLUI) $(LIBGLUT) $(LIBGL) $(LIBS)

$(GLUI_LIB): $(GLUI_OBJ)
	ar -r $(GLUI_LIB) $(GLUI_OBJ)

.cpp.o:
	$(CXX) $(CPPFLAGS) -c $< -o $@

.c.o:
	$(CXX) $(CPPFLAGS) -c $< -o $@

docs:
	doxygen doc/doxygen.cfg

clean:
	rm -f $(GLUI_OBJ) $(GLUI_LIB) $(GLUI_EXAMPLES) $(GLUI_TOOLS) 
	rm -fr doc/doxygen

depend:
	makedepend -Y./include `find -name "*.cpp"` `find -name "*.c"`

DIST = glui-2.35

doc:
	doxygen doc/doxygen.cfg

doc-pdf:
	cd doc/doxygen/latex &&	pdflatex refman.tex && pdflatex refman.tex && pdflatex refman.tex

doc-dist:
	mkdir -p $(DIST)/doc
	cp `find doc/doxygen/html -type f` $(DIST)/doc
	tar cv $(DIST) | gzip -9 - > $(DIST)-doc.tgz
	zip -vr9 $(DIST)-doc.zip $(DIST)
	rm -Rf $(DIST)
	
dist: clean
	mkdir -p $(DIST) 
	cp --parents \
		`find -type f -name "*.cpp"` \
		`find -type f -name "*.c"` \
		`find -type f -name "*.h"` \
		`find -type f -name "*.dev"` \
		`find -type f -name "*.dsp"` \
		`find -type f -name "*.dsw"` \
		`find -type f -name "*.vcproj"` \
		`find -type f -name "*.sln"` \
		`find -type f -name "*.txt"` \
		makefile \
		$(DIST)
	tar cv $(DIST) | gzip -9 - > $(DIST).tgz
	rm -Rf $(DIST)

# DO NOT DELETE THIS LINE -- make depend depends on it.

./example/example5.o: ./include/GL/glui.h
./example/example1.o: ./include/GL/glui.h
./example/example4.o: ./include/GL/glui.h
./example/example3.o: ./include/GL/glui.h
./example/example6.o: ./include/GL/glui.h
./example/example2.o: ./include/GL/glui.h
./src/glui_window.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_window.o: ./src/glui_internal.h
./src/glui_rollout.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_rollout.o: ./src/glui_internal.h
./src/glui_separator.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_separator.o: ./src/glui_internal.h
./src/glui_node.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_node.o: ./src/glui_internal.h
./src/glui_commandline.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_commandline.o: ./src/glui_internal.h
./src/glui_list.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_list.o: ./src/glui_internal.h
./src/glui_checkbox.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_checkbox.o: ./src/glui_internal.h ./src/tinyformat.h
./src/glui_rotation.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_rotation.o: ./src/glui_internal.h ./src/arcball.h ./src/algebra3.h
./src/glui_rotation.o: ./src/quaternion.h ./src/tinyformat.h
./src/glui_scrollbar.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_scrollbar.o: ./src/glui_internal.h
./src/viewmodel.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/viewmodel.o: ./src/glui_internal.h ./src/viewmodel.h ./src/algebra3.h
./src/glui_treepanel.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_treepanel.o: ./src/glui_internal.h ./src/tinyformat.h
./src/glui_mouse_iaction.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_mouse_iaction.o: ./src/glui_internal.h ./src/tinyformat.h
./src/glui_translation.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_translation.o: ./src/glui_internal.h ./src/algebra3.h
./src/glui_translation.o: ./src/tinyformat.h
./src/quaternion.o: ./src/quaternion.h ./src/algebra3.h ./src/glui_internal.h
./src/glui.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui.o: ./src/glui_internal.h ./src/tinyformat.h
./src/glui_textbox.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_textbox.o: ./src/glui_internal.h
./src/glui_tree.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_tree.o: ./src/glui_internal.h
./src/glui_add_controls.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_add_controls.o: ./src/glui_internal.h
./src/arcball.o: ./src/arcball.h ./src/glui_internal.h ./src/algebra3.h
./src/arcball.o: ./src/quaternion.h
./src/glui_control.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_control.o: ./src/glui_internal.h ./src/tinyformat.h
./src/glui_spinner.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_spinner.o: ./src/glui_internal.h ./src/tinyformat.h
./src/glui_column.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_column.o: ./src/glui_internal.h
./src/glui_panel.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_panel.o: ./src/glui_internal.h
./src/algebra3.o: ./src/algebra3.h ./src/glui_internal.h
./src/glui_edittext.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_edittext.o: ./src/glui_internal.h ./src/tinyformat.h
./src/glui_button.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_button.o: ./src/glui_internal.h ./src/tinyformat.h
./src/glui_listbox.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_listbox.o: ./src/glui_internal.h ./src/tinyformat.h
./src/glui_radio.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_radio.o: ./src/glui_internal.h ./src/tinyformat.h
./src/glui_statictext.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_statictext.o: ./src/glui_internal.h
./src/glui_bitmaps.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_bitmaps.o: ./src/glui_internal.h ./src/glui_bitmap_img_data.h
./src/glui_filebrowser.o: ./src/glui_internal_control.h ./include/GL/glui.h
./src/glui_filebrowser.o: ./src/glui_internal.h
./tools/ppm2array.o: ./tools/ppm.h
