
.SUFFIXES: .cpp


#Change the following two lines to reflect the locations
# of the GLUT library (.a) and includes on your system

GLUT_LIB_LOCATION=/unc/walk/lib/sgi_irix
GLUT_INC_LOCATION=/unc/walk/include

CFLAGS = 


#for sgi   -- comment out the lines below to use on HP
CC=CC -g0 -o32
#CC=gcc
#CC=g++ -O3
CPPFLAGS=-I${GLUT_INC_LOCATION} #-w 
GLLIBS=-L${GLUT_LIB_LOCATION} -lglut -lGL -lGLU
LPATH=${GLUTPATH} 
INCS= 


libs =     ${GLLIBS}  -lXmu -lXext -lX11 -lXi -lm #-lmalloc 

LIBGLUI= -Llib -lglui

All: lib/libglui.a bin/example1 bin/example2 bin/example3 bin/example4 bin/example5

GLUI_OBJS = glui_add_controls.o glui.o glui_bitmap_img_data.o glui_bitmaps.o glui_button.o glui_edittext.o glui_checkbox.o glui_node.o glui_radio.o glui_statictext.o glui_panel.o glui_separator.o glui_spinner.o glui_control.o glui_column.o glui_translation.o glui_rotation.o glui_mouse_iaction.o glui_listbox.o glui_rollout.o arcball.o algebra3.o quaternion.o

bin/example1: $(GLUI_OBJS) example1.o lib/libglui.a
	@echo "Linking example1"
	$(CC)  $(CFLAG) $(CPPFLAGS) $(LPATH) example1.o $(LIBGLUI) $(libs) -o bin/example1

bin/example2: $(GLUI_OBJS) example2.o lib/libglui.a
	@echo "Linking example2"
	$(CC)  $(CFLAG) $(CPPFLAGS) $(LPATH) example2.o $(LIBGLUI) $(libs) -o bin/example2

bin/example3: $(GLUI_OBJS) example3.o lib/libglui.a
	@echo "Linking example3"
	$(CC)  $(CFLAG) $(CPPFLAGS) $(LPATH) example3.o $(LIBGLUI) $(libs) -o bin/example3

bin/example4: $(GLUI_OBJS) example4.o lib/libglui.a
	@echo "Linking example4"
	$(CC)  $(CFLAG) $(CPPFLAGS) $(LPATH) example4.o $(LIBGLUI) $(libs) -o bin/example4

bin/example5: $(GLUI_OBJS) example5.o lib/libglui.a
	@echo "Linking example5"
	$(CC)  $(CFLAG) $(CPPFLAGS) $(LPATH) example5.o $(LIBGLUI) $(libs) -o bin/example5

lib/libglui.a: $(GLUI_OBJS)
	@echo "Creating library"
	ar -r lib/libglui.a $(GLUI_OBJS)

.cpp.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(INCS) $*.cpp

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(INCS) $*.c
clean:
	/bin/rm *.o



algebra3.o: algebra3.h
arcball.o: arcball.h algebra3.h quaternion.h stdinc.h
example1.o: glui.h
example2.o:   glui.h
example3.o:   glui.h
example4.o:   glui.h
glui.o: glui.h  stdinc.h 
glui_add_controls.o: glui.h  stdinc.h 
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
glui_bitmaps.o: glui.h stdinc.h 
glui_button.o: glui.h stdinc.h 
glui_checkbox.o: glui.h stdinc.h 
glui_column.o: glui.h stdinc.h 
glui_control.o: glui.h stdinc.h 
glui_edittext.o: glui.h stdinc.h 
glui_node.o: glui.h stdinc.h 
glui_panel.o: glui.h stdinc.h 
glui_radio.o: glui.h stdinc.h 
glui_separator.o: glui.h stdinc.h 
glui_spinner.o: glui.h stdinc.h 
glui_statictext.o: glui.h stdinc.h 
quaternion.o: quaternion.h algebra3.h stdinc.h
glui_translation.o: glui.h stdinc.h  
glui_rotation.o: glui.h stdinc.h  
glui_mouse_iaction.o: glui.h stdinc.h  
glui_listbox.o: glui.h stdinc.h  
glui_rollout.o: glui.h stdinc.h 