.SUFFIXES: .cpp

#for sgi   -- comment out the lines below to use on HP
#CC=CC -g0 -o32
#CC=gcc

UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
CXX       = g++
#CPPFLAGS += -g -Wall
CPPFLAGS += -O3 -Wall
endif

#######################################

LIBGLUI = -Llib -lglui
LIBGL   = -lGLU -lGL
LIBS    = -lXmu -lXext -lX11 -lXi -lm

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

#######################################

GLUI_OBJS = glui_add_controls.o glui_string.o glui.o glui_bitmap_img_data.o glui_bitmaps.o glui_button.o glui_edittext.o glui_commandline.o glui_checkbox.o glui_node.o glui_radio.o glui_statictext.o glui_panel.o glui_separator.o glui_spinner.o glui_control.o glui_column.o glui_translation.o glui_rotation.o glui_mouse_iaction.o glui_listbox.o glui_rollout.o glui_window.o arcball.o algebra3.o quaternion.o viewmodel.o glui_treepanel.o glui_tree.o glui_textbox.o glui_scrollbar.o glui_list.o glui_filebrowser.o

GLUI_LIB = lib/libglui.a

GLUI_EXAMPLES = bin/example1 bin/example2 bin/example3 bin/example4 bin/example5 bin/example6

GLUI_TOOLS = bin/ppm2array

.PHONY: all setup examples tools clean

all: setup $(GLUI_LIB) examples tools

setup:
	mkdir -p bin
	mkdir -p lib

examples: $(GLUI_EXAMPLES)

tools: $(GLUI_TOOLS)

bin/ppm2array: ppm2array.cpp ppm.cpp
	$(CXX) $(CPPFLAGS) -o $@ $^

bin/%: %.cpp $(GLUI_LIB)
	$(CXX) $(CPPFLAGS) -o $@ $<  $(LIBGLUI) $(LIBGLUT) $(LIBGL) $(LIBS)

$(GLUI_LIB): $(GLUI_OBJS)
	ar -r $(GLUI_LIB) $(GLUI_OBJS)

.cpp.o:
	$(CXX) $(CPPFLAGS) -c $<

.c.o:
	$(CXX) $(CPPFLAGS) -c $<

clean:
	rm -f *.o $(GLUI_LIB) $(GLUI_EXAMPLES) $(GLUI_TOOLS)

############################################################################

algebra3.o: algebra3.h
arcball.o: arcball.h algebra3.h quaternion.h glui_internal.h
example1.o: glui.h
example2.o:   glui.h
example3.o:   glui.h
example4.o:   glui.h
example6.o:   glui.h
glui.o: glui.h  glui_internal.h 
glui_filebrowser.o: glui.h glui_internal.h
glui_add_controls.o: glui.h  glui_internal.h 
glui_bitmap_img_data.o: glui_img_checkbox_0.c glui_img_checkbox_1.c
glui_bitmap_img_data.o: glui_img_radiobutton_0.c glui_img_radiobutton_1.c
glui_bitmap_img_data.o: glui_img_uparrow.c glui_img_downarrow.c
glui_bitmap_img_data.o: glui_img_leftarrow.c glui_img_rightarrow.c
glui_bitmap_img_data.o: glui_img_spinup_1.c glui_img_spinup_0.c
glui_bitmap_img_data.o: glui_img_spindown_1.c glui_img_spindown_0.c
glui_bitmap_img_data.o: glui_img_checkbox_0_dis.c glui_img_checkbox_1_dis.c
glui_bitmap_img_data.o: glui_img_radiobutton_0_dis.c
glui_bitmap_img_data.o: glui_img_radiobutton_1_dis.c glui_img_spinup_dis.c
glui_bitmap_img_data.o: glui_img_spindown_dis.c
glui_string.o: glui.h glui_internal.h 
glui_bitmaps.o: glui.h glui_internal.h 
glui_button.o: glui.h glui_internal.h 
glui_checkbox.o: glui.h glui_internal.h 
glui_column.o: glui.h glui_internal.h 
glui_control.o: glui.h glui_internal.h 
glui_edittext.o: glui.h glui_internal.h 
glui_commandline.o: glui.h glui_internal.h 
glui_node.o: glui.h glui_internal.h 
glui_panel.o: glui.h glui_internal.h 
glui_radio.o: glui.h glui_internal.h 
glui_separator.o: glui.h glui_internal.h 
glui_spinner.o: glui.h glui_internal.h 
glui_statictext.o: glui.h glui_internal.h 
quaternion.o: quaternion.h algebra3.h glui_internal.h
glui_translation.o: glui.h glui_internal.h  
glui_rotation.o: glui.h glui_internal.h  
glui_mouse_iaction.o: glui.h glui_internal.h  
glui_listbox.o: glui.h glui_internal.h  
glui_rollout.o: glui.h glui_internal.h 
viewmodel.o: viewmodel.h glui.h glui_internal.h
glui_tree.o: glui.h glui_internal.h
glui_textbox.o: glui.h glui_internal.h
glui_scrollbar.o: glui.h glui_internal.h
glui_list.o: glui.h glui_internal.h
glui_treepanel.o: glui.h glui_internal.h
