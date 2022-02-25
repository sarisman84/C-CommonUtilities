#include "stdafx.h"

#include <ft2build.h>

#ifdef _DEBUG
#pragma comment (lib, "freetype_Debug_x64.lib")
#else
#pragma comment (lib, "freetype_Release_x64.lib")
#endif

#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H
#include FT_STROKER_H
#include FT_OUTLINE_H

#include <tga2d/drawers/sprite_drawer.h>
#include <tga2d/text/text_service.h>
#include <tga2d/texture/texture_manager.h>
#include <tga2d/text/textfile.h>
#include <tga2d/text/fontfile.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/text/text.h>
#include <tga2d/engine.h>
#include <algorithm>
#include <fstream>

#define X_OFFSET 8
#define X_START 2
#define Y_OFFSET 8

using namespace Tga2D;
using Microsoft::WRL::ComPtr;

namespace Tga2D
{
	struct SCharData
	{
		VECTOR2F myTopLeftUV;
		VECTOR2F myBottomRightUV;

		short myWidth;
		short myHeight;
		short myAdvanceX; //Distance to next character.
		short myAdvanceY;
		short myBearingX;
		short myBearingY;
		char myChar;
	};

	class CInternalTextAndFontData
	{
	public:
		~CInternalTextAndFontData();
		SCharData myCharData[1024];
		unsigned int myAtlasWidth;
		unsigned int myAtlasHeight;
		float myLineSpacing;
		unsigned int myWordSpacing;
		unsigned int myFontHeightWidth;
		std::vector<int> myAtlas;
		std::string myName;
		std::unique_ptr<CTexture> myTexture;
		ComPtr<ID3D11ShaderResourceView> myAtlasView;
	};
}

struct STextToRender
{
	VECTOR2F mySize;
	VECTOR2F myUV;
	VECTOR2F uvScale;
	VECTOR2F myPosition;
	bool isWhitespace;
};

struct SGlyphLoader
{
	struct SCountData
	{
		int xNCount = 0;
		int xPCount = 0;
		int yNCount = 0;
		int yPCount = 0;
	};

	struct SOutlineOffset
	{
		int xDelta = 0;
		int yDelta = 0;

	};

	void LoadGlyph(int index, int& atlasX, int& atlasY, int& maxY, float atlasWidth, float atlasHeight, CInternalTextAndFontData* aFontData, struct FT_FaceRec_* aFace, int aBorderWidth = 0);
	void LoadOutline(const int index, const int atlasX, const int atlasY, const float atlasWidth, CInternalTextAndFontData* aFontData, struct FT_FaceRec_* aFace, int aBorderWidth);
	void CalculateOutlineOffsets(const int index, struct FT_FaceRec_* aFace, int aBorderWidth);
	void CalculateGlyphOffsets(const int index, struct FT_GlyphSlotRec_* glyph);

	void CountOffsets(const int& x, const int& y, const int& width, const int& height, SCountData& countData);
	void CountDeltas(const int& width, const int& height, int& deltaX, int& deltaY, SCountData& countData);

	SOutlineOffset myTextOutlineOffset;
	struct FT_LibraryRec_* myLibrary;
};

void SGlyphLoader::CountOffsets(const int& x, const int& y, const int& width, const int& height, SCountData& countData)
{
	if (x < 1)
	{
		countData.xNCount++;
	}

	if (x > width - 1)
	{
		countData.xPCount++;
	}

	if (y < 1)
	{
		countData.yNCount++;
	}

	if (y > height - 1)
	{
		countData.yPCount++;
	}
}

void SGlyphLoader::CountDeltas(const int& width, const int& height, int& deltaX, int& deltaY, SCountData& countData)
{
	if (countData.xNCount == height)
	{
		countData.xNCount = 0;
		deltaX--;
	}

	if (countData.xPCount == height)
	{
		countData.xPCount = 0;
		deltaX++;
	}

	if (countData.yNCount == width)
	{
		countData.yNCount = 0;
		deltaY--;
	}

	if (countData.yPCount == width)
	{
		countData.yPCount = 0;
		deltaY++;
	}
}

void SGlyphLoader::CalculateOutlineOffsets(const int index, FT_FaceRec_* aFace, int aBorderOffset)
{
	myTextOutlineOffset.xDelta = 0;
	myTextOutlineOffset.yDelta = 0;

	FT_Error err;
	FT_Stroker stroker;
	FT_Glyph glyph;

	err = FT_Load_Char(aFace, index, FT_LOAD_NO_BITMAP);
	if (err != 0)
	{
		ERROR_PRINT("%s", "Failed to load glyph!");
	}
	err = FT_Get_Glyph(aFace->glyph, &glyph);

	glyph->format = FT_GLYPH_FORMAT_OUTLINE;

	err = FT_Stroker_New(myLibrary, &stroker);
	if (err != 0)
	{
		ERROR_PRINT("%s", "Failed to get glyph!");
	}
	

	FT_Stroker_Set(stroker, aBorderOffset * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
	err = FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
	if (err != 0)
	{
		ERROR_PRINT("%s", "Failed to stroke glyph border");
	}


	err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
	if (err != 0)
	{
		ERROR_PRINT("%s", "Failed to add glyph to bitmap");
	}
	
	FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

	unsigned int width = bitmapGlyph->bitmap.width;
	unsigned int height = bitmapGlyph->bitmap.rows;

	int xDelta = 0;
	int yDelta = 0;

	SCountData countData;
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			unsigned char& data = bitmapGlyph->bitmap.buffer[y * width + x];

			if (data == 0)
			{
				CountOffsets(x, y, width, height, countData);
			}
			CountDeltas(width, height, xDelta, yDelta, countData);
		}
	}

	myTextOutlineOffset.xDelta = xDelta;
	myTextOutlineOffset.yDelta = yDelta;

	FT_Stroker_Done(stroker);
}

void SGlyphLoader::CalculateGlyphOffsets(const int /*index*/, FT_GlyphSlotRec_* glyph)
{
	int xDelta = 0;
	int yDelta = 0;

	unsigned int width = glyph->bitmap.width;
	unsigned int height = glyph->bitmap.rows;

	SCountData countData;
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			unsigned char& data = glyph->bitmap.buffer[y * width + x];

			if (data == 0)
			{
				CountOffsets(x, y, width, height, countData);
			}
			CountDeltas(width, height, xDelta, yDelta, countData);
		}
	}

	myTextOutlineOffset.xDelta = xDelta;
	myTextOutlineOffset.yDelta = yDelta;
}

int Color32Reverse2(int x)
{
	return ((x & 0xFF000000) >> 8) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | ((x & 0x000000FF) << 24);
}

int Color32Reverse(int x)
{
	int returner = x;
	returner |= ((x & 0x000000FF) << 24);
	returner |= ((x & 0x000000FF) << 16);
	returner |= ((x & 0x000000FF) << 8);

	return returner;
}

void SGlyphLoader::LoadGlyph(int index, int& atlasX, int& atlasY, int& maxY, float atlasWidth, float atlasHeight, CInternalTextAndFontData* aFontData, FT_FaceRec_* aFace, int aBorderOffset)
{
	FT_Error error = FT_Load_Char(aFace, index, FT_LOAD_RENDER);
	if (error != 0)
	{
		ERROR_PRINT("%s", "Failed to load char while glyph loading");
		return;
	}

	FT_GlyphSlot slot = aFace->glyph;
	FT_Bitmap bitmap = slot->bitmap;

	int height = bitmap.rows;
	int width = bitmap.width;

	SCharData glyphData;
	glyphData.myChar = static_cast<char>(index); 
	glyphData.myHeight = static_cast<short>(height + (aBorderOffset * 2));
	glyphData.myWidth = static_cast<short>(width + (aBorderOffset * 2));

	glyphData.myTopLeftUV = { (float(atlasX) / atlasWidth), (float(atlasY) / atlasHeight) };
	glyphData.myBottomRightUV = { (float(atlasX + glyphData.myWidth) / atlasWidth), (float(atlasY + glyphData.myHeight) / atlasHeight) };
	 
	glyphData.myAdvanceX = static_cast<short>(slot->advance.x) >> 6;
	glyphData.myAdvanceY = static_cast<short>(slot->advance.y) >> 6;

	glyphData.myBearingX = (short)slot->bitmap_left;
	glyphData.myBearingY = (short)-slot->bitmap_top;
	if (glyphData.myTopLeftUV.x > 1 || glyphData.myTopLeftUV.y > 1 || glyphData.myBottomRightUV.x > 1 || glyphData.myBottomRightUV.y > 1)
	{
		ERROR_PRINT("%s", "Tried to set a glyph UV to above 1");
		return;
	} 

	CalculateGlyphOffsets(index, slot);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (x < 0 || y < 0)
			{
				continue;
			}
			int& saved = aFontData->myAtlas[((atlasY)+aBorderOffset + myTextOutlineOffset.yDelta + y) *
				int(atlasWidth) + ((atlasX + aBorderOffset + myTextOutlineOffset.xDelta) + x)];
			saved |= bitmap.buffer[y * bitmap.width + x];

			saved = Color32Reverse(saved);

			if (y + (atlasY + Y_OFFSET) > maxY)
			{
				maxY = y + (atlasY + Y_OFFSET);
			}
		}
	}

	atlasX = atlasX + width + X_OFFSET;
	aFontData->myCharData[index] = glyphData;
}

void SGlyphLoader::LoadOutline(const int index, const int atlasX, const int atlasY, const float atlasWidth, CInternalTextAndFontData* aFontData, FT_FaceRec_* aFace, int aBorderOffset)
{
	FT_Error err;
	FT_Stroker stroker;
	FT_Glyph glyph;

	err = FT_Load_Char(aFace, index, FT_LOAD_NO_BITMAP);
	if (err != 0)
	{
		ERROR_PRINT("%s", "Failed to load glyph!");
	}

	err = FT_Get_Glyph(aFace->glyph, &glyph);
	if (err != 0)
	{
		ERROR_PRINT("%s", "Failed to get glyph!");
	}

	glyph->format = FT_GLYPH_FORMAT_OUTLINE;

	err = FT_Stroker_New(myLibrary, &stroker);
	if (err != 0)
	{
		ERROR_PRINT("%s", "Failed to get stroker!");
	}


	FT_Stroker_Set(stroker, aBorderOffset * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
	err = FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
	if (err != 0)
	{
		ERROR_PRINT("%s", "Failed to stroke!");
	}


	err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
	if (err != 0)
	{
		ERROR_PRINT("%s", "Failed to add glyph to bitmap");
	}

	FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

	unsigned int width = bitmapGlyph->bitmap.width;
	unsigned int height = bitmapGlyph->bitmap.rows;

	CalculateOutlineOffsets(index, aFace, aBorderOffset);
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{

			int& data = aFontData->myAtlas[((atlasY + myTextOutlineOffset.yDelta) + y) * int(atlasWidth) + ((atlasX + myTextOutlineOffset.xDelta) + x)];
			data = 0;
			data |= bitmapGlyph->bitmap.buffer[y * width + x];
			data = Color32Reverse2(data);

		}
	}

	FT_Stroker_Done(stroker);
}

STextToRender processNextCharacter(const CInternalTextAndFontData& fontData, char aCharacter, float aSize, float& x, float& y, float& maxY)
{
	int charAsNumber = (unsigned char)aCharacter;
	const SCharData& charData = fontData.myCharData[charAsNumber];

	if (maxY < charData.myHeight)
	{
		maxY = static_cast<float>(charData.myHeight);
	}

	STextToRender result = {};

	result.myPosition.Set((x + charData.myBearingX) * aSize, (y + charData.myBearingY) * aSize);
	result.mySize.Set(static_cast<float>(charData.myWidth), static_cast<float>(charData.myHeight));

	result.myUV = charData.myTopLeftUV;
	result.uvScale = charData.myBottomRightUV;

	if (aCharacter == '\n')
	{
		x = 0;
		y += (maxY + 6);
	}
	else
	{
		if (aCharacter == ' ')
		{
			x += fontData.myWordSpacing;
		}
		x += charData.myAdvanceX;
	}

	return result;
}

CInternalTextAndFontData::~CInternalTextAndFontData()
{}

CTextService::CTextService()
{
}

void CTextService::Init()
{
	FT_Init_FreeType(&myLibrary);
}

CTextService::~CTextService()
{
	FT_Done_FreeType(myLibrary);
}

inline bool FileExists(const std::string& name) 
{
	std::ifstream f(name.c_str());
	if (f.good()) 
	{
		f.close();
		return true;
	}

	f.close();
	return false;
}

SFont CTextService::GetOrLoad(std::string aFontPath, unsigned char aBorderSize, int aSize)
{
	if (aSize == -1)
	{
		return { nullptr };
	}
	
	short fontWidth = (short)aSize;
	const int atlasSize = 1024;
	float atlasWidth = static_cast<float>(atlasSize);
	float atlasHeight = static_cast<float>(atlasSize);

	int atlasX = X_START;
	int atlasY = 2;
	int currentMaxY = 0;

	std::shared_ptr<CInternalTextAndFontData> fontData = nullptr;

	std::stringstream key;
	key << aFontPath << "-" << fontWidth << "-" << aBorderSize;

	auto it = myFontData.find(key.str());
	if (it != myFontData.end())
	{
		fontData = it->second.lock();
		if (fontData)
			return { fontData };
	}

	fontData = std::make_shared<CInternalTextAndFontData>();
	myFontData[key.str()] = fontData;

	fontData->myAtlas.resize(atlasSize * atlasSize);
	ZeroMemory(fontData->myAtlas.data(), (atlasSize * atlasSize) * sizeof(int));
	fontData->myFontHeightWidth = fontWidth;
	fontData->myName = key.str();

	FT_Face face;
	FT_Error error = FT_New_Face(myLibrary, aFontPath.c_str(), 0, &face);
	if (error != 0)
	{
		return { nullptr };
	}

	FT_F26Dot6 ftSize = (FT_F26Dot6)(fontData->myFontHeightWidth * (1 << 6));
	error = FT_Set_Char_Size(face, ftSize, 0, 100, 100);
	if (error != 0)
	{
		ERROR_PRINT("%s", "FT_Set_Char_Size error");
		return { nullptr };
	}

	error = FT_Load_Char(face, 'x', FT_LOAD_DEFAULT);
	if (error != 0)
	{
		ERROR_PRINT("%s", "Failed to load glyph!");
		return { nullptr };
	}

	FT_GlyphSlot space = face->glyph;

	fontData->myWordSpacing = static_cast<short>(space->metrics.width / 256);

	int currentMax = 256;
	int currentI = 32;

	SGlyphLoader glyphLoader;
	glyphLoader.myLibrary = myLibrary;
	for (int i = currentI; i < currentMax; i++)
	{
		error = FT_Load_Char(face, i, FT_LOAD_RENDER);
		if (error != 0)
		{
			ERROR_PRINT("%s%s", "Failed to load glyph! ", (char)i);
			continue;
		}

		FT_GlyphSlot slot = face->glyph;

		if (atlasX + slot->bitmap.width + (aBorderSize * 2) > atlasWidth)
		{
			atlasX = X_START;
			atlasY = currentMaxY;
		}
		if (aBorderSize > 0)
		{
			glyphLoader.LoadOutline(i, atlasX, atlasY, atlasWidth, fontData.get(), face, aBorderSize);
		}

		glyphLoader.LoadGlyph(i, atlasX, atlasY, currentMaxY, atlasWidth, atlasHeight, fontData.get(), face, aBorderSize);
	}

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = fontData->myAtlas.data();
	data.SysMemPitch = atlasSize * 4;

	D3D11_TEXTURE2D_DESC info;
	info.Width = atlasSize;
	info.Height = atlasSize;
	info.MipLevels = 0;
	info.ArraySize = 1;
	info.SampleDesc.Count = 1;
	info.SampleDesc.Quality = 0;
	info.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	info.Usage = D3D11_USAGE_DEFAULT;
	info.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	info.CPUAccessFlags = 0;

	ID3D11Texture2D* texture;
	HRESULT hr = CEngine::GetInstance()->GetDirect3D().GetDevice()->CreateTexture2D(&info, nullptr, &texture);
	if (FAILED(hr))
	{
		ERROR_PRINT("%s", "Failed to load texture for text!");
		return { nullptr };
	}

	CEngine::GetInstance()->GetDirect3D().GetContext()->UpdateSubresource(texture, 0, NULL, fontData->myAtlas.data(), atlasSize * 4, 0);

	CEngine::GetInstance()->GetDirect3D().GetDevice()->CreateShaderResourceView(texture, nullptr, fontData->myAtlasView.ReleaseAndGetAddressOf());
	CEngine::GetInstance()->GetDirect3D().GetContext()->GenerateMips(fontData->myAtlasView.Get());
	texture->Release();

	fontData->myAtlasHeight = atlasSize;
	fontData->myAtlasWidth = atlasSize;
	fontData->myLineSpacing = static_cast<float>((face->ascender - face->descender) >> 6);
	FT_Done_Face(face);
	
	fontData->myTexture = std::make_unique<CTexture>();
	fontData->myTexture->myResource = fontData->myAtlasView;
	fontData->myTexture->myPath = key.str();
	
	return { fontData };
}

float Tga2D::CTextService::GetSentenceWidth(Tga2D::CText& aText)
{
	if (aText.myText.size() <= 0)
	{
		return 0.0f;
	}

	const CInternalTextAndFontData* fontData = aText.myFont.myData.get();
	if (!fontData)
	{
		return 0.0f;
	}

	int count = static_cast<int>(aText.myText.length());

	float maxX = FLT_MIN;

	float drawX = 0.f;
	float drawY = 0.f;
	float maxDrawY = 0.f;

	for (int i = 0; i < count; i++)
	{
		STextToRender charInfo = processNextCharacter(*fontData, aText.myText[i], aText.myScale, drawX, drawY, maxDrawY);
		maxX = std::max(maxX, charInfo.myPosition.myX + charInfo.mySize.myX);
	}

	return maxX;
}


float Tga2D::CTextService::GetSentenceHeight(Tga2D::CText& aText)
{
	if (aText.myText.size() <= 0)
	{
		return 0.0f;
	}

	const CInternalTextAndFontData* fontData = aText.myFont.myData.get();
	if (!fontData)
	{
		return 0.0f;
	}

	int count = static_cast<int>(aText.myText.length());

	float maxY = FLT_MIN;

	float drawX = 0.f;
	float drawY = 0.f;
	float maxDrawY = 0.f;

	for (int i = 0; i < count; i++)
	{
		STextToRender charInfo = processNextCharacter(*fontData, aText.myText[i], aText.myScale, drawX, drawY, maxDrawY);
		maxY = std::max(maxY, charInfo.myPosition.myY + charInfo.mySize.myY);
	}

	return maxY;
}

bool CTextService::Draw(Tga2D::CText& aText, Tga2D::CSpriteShader* aCustomShader)
{
	const CInternalTextAndFontData* fontData = aText.myFont.myData.get();
	if (!fontData || !fontData->myTexture)
	{
		return false;
	}

	;
	int count = (int)aText.myText.size();
	const char* str = aText.myText.c_str();
	float scale = aText.myScale;
	float rotation = aText.myRotation;

	float screenSizeX = static_cast<float>(CEngine::GetInstance()->GetTargetSize().x);
	float screenSizeY = static_cast<float>(CEngine::GetInstance()->GetTargetSize().y);

	float midX = 0;
	float midY = 0;
	if (rotation != 0.0f)
	{
		float minX = FLT_MAX;
		float minY = FLT_MAX;
		float maxX = FLT_MIN;
		float maxY = FLT_MIN;

		float drawX = 0.f;
		float drawY = 0.f;
		float maxDrawY = 0.f;

		for (int i = 0; i < count; i++)
		{
			STextToRender charInfo = processNextCharacter(*fontData, str[i], scale, drawX, drawY, maxDrawY);

			minX = std::min(minX, charInfo.myPosition.myX);
			minY = std::min(minY, charInfo.myPosition.myY);
			maxX = std::max(maxX, charInfo.myPosition.myX + charInfo.mySize.myX);
			maxY = std::max(maxY, charInfo.myPosition.myY + charInfo.mySize.myY);
		}

		midX = 0.5f * (maxX + minX) / screenSizeX;
		midY = 0.5f * (maxY + minY) / screenSizeY;
	}

	float windowRatio = static_cast<float>(CEngine::GetInstance()->GetWindowRatio());

	SSpriteSharedData spriteSharedData = {};
	if (fontData->myFontHeightWidth < 18)
	{
		spriteSharedData.mySamplerFilter = ESamplerFilter::Point;
		spriteSharedData.mySamplerAddressMode = ESamplerAddressMode::Clamp;
	}
	else
	{
		spriteSharedData.mySamplerFilter = ESamplerFilter::Bilinear;
		spriteSharedData.mySamplerAddressMode = ESamplerAddressMode::Clamp;
	}

	spriteSharedData.myTexture = fontData->myTexture.get();
	spriteSharedData.myCustomShader = aCustomShader;

	{
		CSpriteBatchScope batchScope = CEngine::GetInstance()->GetDirect3D().GetSpriteDrawer().BeginBatch(spriteSharedData);

		float drawX = 0.f;
		float drawY = 0.f;
		float maxDrawY = 0.f;

		float diffX = midX;
		float diffY = midY;

		float c=0.f, s=0.f;
		if (rotation != 0.0f)
		{
			c = cos(rotation);
			s = sin(rotation);
		}

		for (int i = 0; i < count; i++)
		{
			STextToRender charInfo = processNextCharacter(*fontData, str[i], scale, drawX, drawY, maxDrawY);

			SSpriteInstanceData spriteInstance = {};

			charInfo.myPosition.x /= screenSizeX;
			charInfo.myPosition.y /= screenSizeY;

			charInfo.mySize.x /= screenSizeY;
			charInfo.mySize.y /= screenSizeY;

			if (rotation != 0.0f)
			{
				spriteInstance.myRotation = rotation;

				float x = charInfo.myPosition.x - diffX;
				float y = charInfo.myPosition.y - diffY;

				y /= windowRatio;

				charInfo.myPosition.x = x * c - y * s + diffX;
				charInfo.myPosition.y = (x * s + y * c) * windowRatio + diffY;
			}

			spriteInstance.myPivot = { 0.f, 0.f };
			spriteInstance.myPosition = charInfo.myPosition + aText.myPosition;
			spriteInstance.mySize = charInfo.mySize * scale;
			spriteInstance.myColor = aText.myColor;
			spriteInstance.myTextureRect = { charInfo.myUV.x, charInfo.myUV.y, charInfo.uvScale.x, charInfo.uvScale.y };

			batchScope.Draw(spriteInstance);
		}
	}

	return true;
}