#include "stdafx.h"
#include "HTMLParser.h"

#include <AppCore/Platform.h>





Tga2D::HTML_Parser::HTML_Parser()
{
	myConfig = Config();
	myConfig.resource_path = "./resources/";
	myConfig.use_gpu_renderer = false;
	myConfig.device_scale = 1.0;

	Platform::instance().set_config(myConfig);
	Platform::instance().set_font_loader(GetPlatformFontLoader());
	Platform::instance().set_file_system(GetPlatformFileSystem("."));
	Platform::instance().set_logger(GetDefaultLogger("ultralight.log"));

	myRenderer = ultralight::Renderer::Create();
}

Tga2D::Texture* Tga2D::HTML_Parser::ParseHTMLFile(const wchar_t* aFilePath, RefPtr<View>* anViewObject)
{
	auto screenRez = Tga2D::Engine::GetInstance()->GetTargetSize();
	RefPtr<View> view = myRenderer->CreateView(screenRez.x, screenRez.y, true, nullptr);


	view->LoadHTML("<h1>Hello World!</h1>");
	view->Focus();


	if (anViewObject)
		*anViewObject = view;

	myCurrentViews.push_back(view);


	Surface* surface = view->surface();
	BitmapSurface* bitmapSurf = (BitmapSurface*)surface;


	void* pixelBuffer = bitmapSurf->LockPixels();
	uint32_t width = bitmapSurf->width();
	uint32_t height = bitmapSurf->height();
	uint32_t stride = bitmapSurf->row_bytes();


	Texture* texture = CreateTextureFromHTML(pixelBuffer, width, height, stride);
	bitmapSurf->UnlockPixels();

	return texture;
}


void Tga2D::HTML_Parser::Update()
{
	myRenderer->Update();
}

void Tga2D::HTML_Parser::Render()
{
	myRenderer->Render();

	for (auto& view: myCurrentViews)
	{
		Surface* surface = view->surface();
		BitmapSurface* bitmapSurf = (BitmapSurface*)surface;

		if (!bitmapSurf->dirty_bounds().IsEmpty())
		{
			void* pixelBuffer = bitmapSurf->LockPixels();
			uint32_t width = bitmapSurf->width();
			uint32_t height = bitmapSurf->height();
			uint32_t stride = bitmapSurf->row_bytes();


			Texture* texture = CreateTextureFromHTML(pixelBuffer, width, height, stride);
			bitmapSurf->UnlockPixels();
		}
		bitmapSurf->ClearDirtyBounds();
		
	}
}

//Based of TextureManager.cpp's CreateTextureFromTarga()
Tga2D::Texture* Tga2D::HTML_Parser::CreateTextureFromHTML(void* aPixelBuffer, uint32_t aWidth, uint32_t aHeight, uint32_t aStride)
{
	ID3D11Texture2D* texture = nullptr;
	Texture* result = nullptr;
	ID3D11ShaderResourceView* resource = nullptr;

	D3D11_TEXTURE2D_DESC tDesc;
	D3D11_SUBRESOURCE_DATA tBsd;

	int h = aHeight;
	int w = aWidth;

	int* pixelBuffer = (int*)aPixelBuffer;
	int* buf = new int[h * w];
	for (int i = 0; i < (h * w) * 4; i++)
	{
		//Unconfirmed if this actually works.

		unsigned char b = static_cast<unsigned char>(pixelBuffer[i]);
		unsigned char g = static_cast<unsigned char>(pixelBuffer[i + 1]);
		unsigned char r = static_cast<unsigned char>(pixelBuffer[i + 2]);
		unsigned char a = static_cast<unsigned char>(pixelBuffer[i + 3]);


		//Not sure if these bitshifts are needed or not, needs to be looked at.
		unsigned int final = 0;
		final |= (a << 24);
		final |= (r << 16);
		final |= (g << 8);
		final |= (b);

		buf[i / 4] = final;

		/*

		unsigned char b = static_cast<unsigned char>(aImage->myImage[i]);
		unsigned char g = static_cast<unsigned char>(aImage->myImage[i+1]);
		unsigned char r = static_cast<unsigned char>(aImage->myImage[i+2]);
		unsigned char a = static_cast<unsigned char>(aImage->myImage[i+3]);

		unsigned int final = 0;
		final |= (a << 24);
		final |= (r << 16);
		final |= (g << 8);
		final |= (b);

		buf[i/4] = final;

		*/
	}

	tBsd.pSysMem = (void*)buf;
	tBsd.SysMemPitch = aStride; //replacing w with aStride as aStride is the dist in bytes between each pixel according to the Ultralight docs.
	tBsd.SysMemSlicePitch = aStride * h * 4; //not sure how to calculate this smh.

	tDesc.Width = w;
	tDesc.Height = h;
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;

	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tDesc.MiscFlags = 0;

	if (FAILED(DX11::Device->CreateTexture2D(&tDesc, &tBsd, &texture)))
		return nullptr;

	delete[] buf;

	HRESULT hr = DX11::Device->CreateShaderResourceView(texture, NULL, &resource);
	if (FAILED(hr))
		return nullptr;

	if (resource)
	{
		result = new Texture();
		result->myImageSize.Set(aWidth, aHeight);
		result->myPath = L"Lol"; //Bruh what (Taken from CreateTextureFromTaiga())
		result->SetShaderResourceView(resource);
	}

	return result;
}


