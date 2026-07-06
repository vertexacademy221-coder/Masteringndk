#pragma once

#include "FT_Utils.h"

#include <android/log.h>
#include <iterator>
#include <vector>
#include <string>
#include <fstream>

namespace fiengine {

	int GetPixel 	( const std::vector<unsigned char>& Bitmap, int H, int W, int x, int y);
	void PutPixel 	( std::vector<unsigned char>& Bitmap, int W, int H, int x, int y, int Color);
	void RenderChar ( char c, int x, int y);
	void RenderStr	(const std::string& Str, int x, int y);
	void TestFontRendering ( const std::vector<char>& Data);

	inline int Greyscale (unsigned char c) 
	{
		return ( (255 -c) << 16 ) + ( (255 - c) << 8) + (255 - c);
	}

}	