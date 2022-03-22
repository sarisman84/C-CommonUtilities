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

RefPtr<View> Tga2D::HTML_Parser::CreateHTMLObject(const char* aFilePath, Vector2f anObjectSize)
{
	Vector2ui pixelSize = Utilities::ConvertToPixelUnits(anObjectSize);
	RefPtr<View> view = myRenderer->CreateView(pixelSize.x, pixelSize.y, true, nullptr);
	view->LoadHTML(FetchHTML(aFilePath));
	return view;
}

void Tga2D::HTML_Parser::Render(const RefPtr<View>& anObject)
{
	Surface* surface = anObject->surface();

	///
	/// Cast it to a BitmapSurface.
	///
	BitmapSurface* bitmap_surface = (BitmapSurface*)surface;

	///
	/// Get the underlying bitmap.
	///
	RefPtr<Bitmap> bitmap = bitmap_surface->bitmap();


	void* pixelBuffer = bitmap->LockPixels();
	uint32_t width = bitmap->width();
	uint32_t height = bitmap->height();
	uint32_t stride = bitmap->row_bytes();



}

const ultralight::String& Tga2D::HTML_Parser::FetchHTML(const char* aPath)
{
	std::string result;
	std::fstream file;
	file.open(aPath);
	// TODO: insert return statement here
	file >> result;

	return ultralight::String();

}
