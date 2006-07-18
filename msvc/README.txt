About the GLUI Project Files for Microsoft Visual Studio.


----------------------------------------
Microsoft Visual Studio 7.1
----------------------------------------

Using these project files you can build four different versions of
glui:

=== Release ===

  - static GLUI library, debug symbols off, optimizations on
  - outputs lib\glui32.lib

  - To use in your program:

    The lib should link in automatically if you include glui.h
      thanks to a #pragma comment(lib...) statement.

=== Debug ===

  - static GLUI library, debug symbols on, optimizations off
  - outputs lib\glui32d.lib

  - To use in your program:

    Define preprocessor symbols:  NO_GLUI_LIB_PRAGMA
    Add to the list of input libs:  glui32d.lib

=== Release GLUIDLL ===

  - dynamic GLUI library, debug symbols off, optimizations on
  - outputs lib\glui32dll.lib and bin\glui32dll.dll

  - To use in your program:

    Define the preprocessor symbol:  GLUIDLL
    Then the dll should link in automatically if you include glui.h
    thanks to a #pragma comment(lib...) statement.

=== Debug GLUIDLL ===

  - dynamic GLUI library, debug symbols on, optimizations off
  - outputs lib\glui32dlld.lib and bin\glui32dlld.dll

  - To use in your program:

    Define preprocessor symbols:  NO_GLUI_LIB_PRAGMA;GLUIDLL
    Add to the list of input libs:  glui32dlld.lib


To build the examples, you need to build the libraries first, then the
examples (no automatic dependencies are set up in the solution file).

If you're lazy, you can just batch build everything twice.  (Some
examples will fail to link the first time through because not all the
libraries have been built yet, but the second time through everything
should link.)

The examples are set up in pretty much the way you would need to set
them up in your own programs in terms of preprocessor defines and extra 
libraries in the list of link libs.  However, you will probably only want 
to link with either glui32.lib or glui32dll.lib in your programs.  The debug
versions are only useful if you want to debug GLUI itself.


RUNNING GLUIDLL EXAMPLES:

To make the GLUIDLL examples run from within Visual Studio you may need to
change the working directory on the example projects to "bin":

  Properties -> Debugging -> Working Directory


Unfortunately Microsoft considers this setting "user-specific" rather
than "project-specific" and so doesn't save it in the main project or
solution files.


----------------------------------------
Microsoft Visual Studio 8.0
----------------------------------------

Visual Studio 8 should be able to convert the 7.1 project files automatically.

----------------------------------------
Microsoft Visual Studio 6.0
----------------------------------------

These project files are out of date.

We are looking for any volunteers who still use MSVC 6.0 to maintain
these files.