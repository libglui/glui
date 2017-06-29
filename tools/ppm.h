//------------------------------------------------------------------------------
// File : ppm.hpp
//------------------------------------------------------------------------------
//============================================================================
// ppm.hpp : Portable Pixel Map image format module
//============================================================================

#include <string>

void LoadPPM(const std::string &FileName, unsigned char* &Color, int &Width, int &Height);
void WritePPM(const std::string &FileName, unsigned char* Color, int Width, int Height);
void VFlip(unsigned char * Pix, int width, int height, int chan=3);
