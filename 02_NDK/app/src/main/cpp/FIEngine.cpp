#include "FIEngine.h"
#include <android/log.h>

#define TAG "FIENGINE"
#define FIELOGE(...)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define FIELOGI(...)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


// Horizontal size of the character
const int CharW = 32;
const int CharH = 64;

// Horizontal size of the character slot
const int SlotW = CharW * 2;
const int SlotH = CharH;

const int FontW = 16 * SlotW;
const int FontH = 16 * SlotH;

std::vector<unsigned char> g_FontBitmap;
std::vector<unsigned char> g_OutBitmap;

int W = 2048;
int H = 2048;

int fiengine::GetPixel 	( const std::vector<unsigned char>& Bitmap, int H, int W, int x, int y)
{
	if ( y >= H || x >= W || y < 0 || x < 0) {return 0;}

	int offset = ( y * W + x) * 3;
	return ( Bitmap[offset + 0] << 16) + (Bitmap[offset + 1] << 8) + Bitmap[offset + 2];
}

void fiengine::PutPixel ( std::vector<unsigned char>& Bitmap, int W, int H, int x, int y, int Color)
{
	if ( y < 0 || x < 0 || y > H - 1 || x > W -1 ) {return;}

	int Ofs = y * W + x;
	Bitmap[Ofs * 3 + 0] = Color & 0xFF;
	Bitmap[Ofs * 3 + 1] = (Color >> 8) & 0xFF;
	Bitmap[Ofs * 3 + 2] = (Color >> 16) & 0xFF;
}

void fiengine::RenderChar ( char c, int x, int y)
{
	int u = ( c % 16 ) * SlotW;
	int v = ( c / 16 ) * SlotH;

	for (int y1 = 0; y1 < CharW; y1++)
	{
		for ( int x1 = 0; x1 <= CharW; x1++)
		{
			PutPixel ( g_OutBitmap, W, H, x + x1, y + y1, 
				GetPixel (g_FontBitmap, FontW, FontH, x1 + u + CharW, y1 + v) );
		}
	}
}

void fiengine::RenderStr	(const std::string& Str, int x, int y)
{
	for (auto c : Str) 
	{
		RenderChar ( c, x, y);
		x += CharW;
	}
}

void fiengine::TestFontRendering ( const std::vector<char>& Data)
{
	// Loading FreeType. Later on we call stb_image for
	// load Font type and pass data to FreeType
	if (!LoadFreeType())
	{
		FIELOGE("Failed loading FreeType");
		return;
	}

	FT_Library Library;
	// Initialize library
	FT_Init_FreeTypePTR (&Library);

	FT_Face Face;
	if(!FT_New_Memory_FacePTR ( Library, (const FT_Byte*)Data.data(), (int)Data.size(), 0, &Face))
	{
		FIELOGE("FacePTR::Memory allocation failed");
		return;
	}

	// char size equal to 100dpi
	if(!FT_Set_Char_SizePTR ( Face, CharW * 64, 0, 100, 0))
	{
		FIELOGE("SizePTR::Failed to set char size");
		return;
	}

	g_FontBitmap.resize( FontW * FontH * 3);
	std::fill ( std::begin (g_FontBitmap), std::end (g_FontBitmap), 0xFF);

	for (int n = 0; n < 256; n++)
	{
		// Load glyph image into the slot(erase previous one)
		if ( FT_Load_CharPTR ( Face, n, FT_LOAD_RENDER)){continue;}

		FT_GlyphSlot Slot = Face->glyph;
		FT_Bitmap  Bitmap = Slot->bitmap;

		int x = (n % 16 ) * SlotW + SlotW / 2 + Slot->bitmap_left;
		int y = (n / 16) * SlotH - Slot->bitmap_top + CharH - CharH / 4;

		for (auto i = 0; i < Bitmap.width; i++)
		{
			for (auto j = 0; j < Bitmap.rows; j++)
			{
				PutPixel (g_FontBitmap, FontW, FontH, i + x, j + y,
					Greyscale (Bitmap.buffer[j * Bitmap.width + i] ) );
			}
		}
	}

	// 3 canaux pour du 24-bit (RGB), ou 4 canaux pour du 32-bit (RGBA)
	int comp = 3; 

	// Largeur d'une ligne de pixels en octets
	int stride_in_bytes = FontW * comp;

	int result = 0;

	//  int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
	result = stbi_write_png("test_font.png", FontW, FontH, comp, g_FontBitmap.data(), stride_in_bytes);

	if (result == 0) {
	    FIELOGE("Writing \'test_font.png \' failed");
	}

	//stbi_flip_vertically_on_write(true);
	FT_Done_FacePTR ( Face );
	FT_Done_FreeTypePTR ( Library);

	std::string Str = "Test string";
	W = Str.length() * CharW;
	H = CharH;

	g_OutBitmap.resize ( W * H * 3 );
	std::fill ( std::begin (g_OutBitmap), std::end(g_OutBitmap), 0xFF);

	RenderStr (Str, 0, 0);

	result = stbi_write_png ("test_str.png", FontW, FontH, comp, g_OutBitmap.data(), stride_in_bytes);
	if (result == 0) {
	    FIELOGE("Writing \'test_str.png \' failed");
	}
}