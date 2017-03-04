#include "Draw2D.h"
#define EPSILON 0.001f
#define SAMEFLOAT(x,y) ((x - y) < EPSILON && ((-(x - y)) < EPSILON))

namespace Epoch
{

	Draw* Draw::sInstance = nullptr;
	Draw::Draw()
	{
		mFontIDs = 0;
		mColorIDs = 0;
	}

	Draw::~Draw()
	{
		(*mTextFactory)->Release();
		(*mDevice2D)->Release();
		(*mGIDevice)->Release();
		(*mContext2D)->Release();
		(*mDWrite)->Release();
		(*mTextformat)->Release();
		(*mBrush)->Release();
		(*mScreenBitmap)->Release();

		mTextFactory.reset();
		mDevice2D.reset();
		mGIDevice.reset();
		mContext2D.reset();
		mDWrite.reset();
		mTextformat.reset();
		mBrush.reset();
		mScreenBitmap.reset();
	}

	Draw & Draw::Instance()
	{
		if (!sInstance)
		{
			sInstance = new Draw();
			sInstance->InitializeDirect2D();
			sInstance->InitializeScreenBitmap();
		}
		return *sInstance;
	}

	void Draw::DestroyInstance()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
		}
		sInstance = nullptr;
	}
	void Draw::InitializeDirect2D()
	{

		//create 2dfactory
		ID2D1Factory1 * factory2;
		Renderer::Instance()->ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory2));
		sInstance->mTextFactory = std::make_shared<ID2D1Factory1*>(factory2);

		//createDxgiDevice
		IDXGIDevice* DxgiDevice;
		Renderer::Instance()->ThrowIfFailed((*Renderer::Instance()->iGetDevice())->QueryInterface(__uuidof(IDXGIDevice), (void **)&DxgiDevice));
		sInstance->mGIDevice = std::make_shared<IDXGIDevice*>(DxgiDevice);

		//create device2d 
		ID2D1Device* Device2d;
		Renderer::Instance()->ThrowIfFailed(factory2->CreateDevice(*sInstance->mGIDevice, &Device2d));
		sInstance->mDevice2D = std::make_shared<ID2D1Device*>(Device2d);

		//create context
		ID2D1DeviceContext* context2d;
		Renderer::Instance()->ThrowIfFailed((*sInstance->mDevice2D)->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &context2d));
		sInstance->mContext2D = std::make_shared<ID2D1DeviceContext*>(context2d);

		IDWriteFactory* WriteFactory;
		Renderer::Instance()->ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(WriteFactory), reinterpret_cast<IUnknown**>(&WriteFactory)));
		sInstance->mDWrite = std::make_shared<IDWriteFactory*>(WriteFactory);

	}
	void Draw::InitializeScreenBitmap()
	{
		//ID3D11Texture2D* backbuffer2D;
		//ThrowIfFailed((*mChain)->GetBuffer(0, IID_PPV_ARGS(&backbuffer2D)));

		//sInstance->mScreenBitmap = make_shared<ID2D1Bitmap1*>(CreateBitmapForTexture(backbuffer2D));
		//(*mContext2D)->SetTarget((*mScreenBitmap));

		sInstance->mScreenBitmap = std::make_shared<ID2D1Bitmap1*>(CreateBitmapForTexture(*Renderer::Instance()->iGetRTViewTexture()));
	}
	//////
	//checks if(already in side the list)
	//else(adds it to the list)
	//////
	void Draw::ProcessFont(Font _font)
	{
		for (std::pair<unsigned int, std::pair<Font, IDWriteTextFormat*>> x : mFonts)
		{
			if (x.second.first == _font)
				return;
		}
		//for (std::pair <Font, IDWriteTextFormat*> x : mFonts)
		//{
		//	if (x.first == _font)
		//		return;
		//}
		CreateNewTextFormat(_font);
	}
	//////
	//return Brush if it was already made and put in the list (map), 
	//if it wasn't then its created, added to the list (map), then returned 
	//////
	ID2D1SolidColorBrush * Draw::GetBrush(D2D1::ColorF _color)
	{
		for (std::pair<unsigned int, std::pair<D2D1::ColorF, ID2D1SolidColorBrush*>> x : mColorBrushes)
		{
			if (isColorSame(x.second.first, _color))
				return x.second.second;
		}
		return CreateNewBrush(_color);
	}
	//////
	//returns a textformat based on the font chosen
	//if( font already exists then it uses an existing textformat)
	//else( makes a new one and adds it to the list)
	//////
	IDWriteTextFormat * Draw::GetTextFormat(Font _font)
	{
		for (std::pair<unsigned int, std::pair<Font, IDWriteTextFormat*>> x : mFonts)
		{
			if (x.second.first == _font)
				return x.second.second;
		}
		//for (std::pair<Font, IDWriteTextFormat*> x : mFonts)
		//{
		//	if (x.first == _font)
		//		return x.second;
		//}
		return CreateNewTextFormat(_font);
	}
	ID2D1SolidColorBrush * Draw::CreateNewBrush(D2D1::ColorF _color)
	{
		//Color and Draw
		ID2D1SolidColorBrush* brush;
		Renderer::Instance()->ThrowIfFailed((*mContext2D)->CreateSolidColorBrush(_color, &brush));
		mColorBrushes.insert({ mColorIDs,std::pair< D2D1::ColorF, ID2D1SolidColorBrush*>(_color, brush) });
		mColorIDs++;
		return brush;

	}
	IDWriteTextFormat * Draw::CreateNewTextFormat(Font _font)
	{
		IDWriteTextFormat* WriteFormat;
		Renderer::Instance()->ThrowIfFailed((*mDWrite)->CreateTextFormat(
			_font.mFontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			_font.mFontSize,
			L"en-us",
			&WriteFormat));
		mFonts.insert({ mFontIDs,std::pair<Font, IDWriteTextFormat*>(_font,WriteFormat) });
		mFontIDs++;
		//mFonts.insert({_font,WriteFormat });

		return WriteFormat;
	}

	void Draw::DrawTextToBitmap(float _left, float _top, float _right, float _bottom,
															Font _font, std::wstring _text, ID2D1Bitmap* _bitmap)
	{
		(*mContext2D)->SetTarget(_bitmap);
		float color[4] = { 0.3f, 0.3f, 1, 1 };

		// Retrieve the size of the render target.
		D2D1_SIZE_F renderTargetSize = (*mContext2D)->GetSize();

		(*mContext2D)->BeginDraw();
		(*mContext2D)->SetTransform(D2D1::Matrix3x2F::Identity());

		(*mContext2D)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		(*mContext2D)->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);

		ProcessFont(_font);
		IDWriteTextFormat* tempTF = GetTextFormat(_font);
		tempTF->SetTextAlignment(_font.mTextAlignment);
		tempTF->SetParagraphAlignment(_font.mParagraphAlignment);
		D2D1::ColorF tempColor = _font.mColor;

		//sInstance->mBrush = std::make_shared<ID2D1SolidColorBrush*>(brush);
		(*mContext2D)->DrawText(
			_text.c_str(),
			(UINT32)_text.size(),
			(tempTF),
			D2D1::RectF(_left, _top, _right, _bottom),//renderTargetSize.width*(3.0f/4.0f), renderTargetSize.height*(3.0f/4.0f), renderTargetSize.width, renderTargetSize.height
			GetBrush(tempColor)
		);
		Renderer::Instance()->ThrowIfFailed((*mContext2D)->EndDraw());

	}

	void Draw::DrawRectangleToBitmap(float _left, float _top, float _right, float _bottom, D2D1::ColorF _color, ID2D1Bitmap * _bitmap)
	{
		(*mContext2D)->SetTarget(_bitmap);
		float color[4] = { 1, 1, 1, 1 };

		// Retrieve the size of the render target.st
		D2D1_SIZE_F renderTargetSize = (*mContext2D)->GetSize();

		(*mContext2D)->BeginDraw();
		(*mContext2D)->SetTransform(D2D1::Matrix3x2F::Identity());
		(*mContext2D)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

		//(*mContext2D)->CreateSolidColorBrush(
		//	D2D1::Point2F(100.0f, 100.1f),
		//	75.0f,
		//	50.0f);
		D2D1_RECT_F rectangle = D2D1::RectF(_left, _top, _right, _bottom);//0.0f, renderTargetSize.height*(3.0f / 4.0f), renderTargetSize.width*(1.0f / 4.0f), renderTargetSize.height
		ID2D1SolidColorBrush* brush;
		Renderer::Instance()->ThrowIfFailed((*mContext2D)->CreateSolidColorBrush(_color, &brush));//D2D1::ColorF(D2D1::ColorF::LightGray,0.7f)

		(*mContext2D)->FillRectangle(&rectangle, brush);
		(*mContext2D)->DrawRectangle(&rectangle, brush);
		//delete brush;
		Renderer::Instance()->ThrowIfFailed((*mContext2D)->EndDraw());
		brush->Release();

	}

	//IF YOU USE THIS, CLEAN UP AFTER YOURSELF
	ID2D1Bitmap1 * Draw::CreateBitmapForTexture(ID3D11Texture2D * _texture)
	{
		//Make a bitmap
		D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_R16G16B16A16_FLOAT, D2D1_ALPHA_MODE_IGNORE),
				0,
				0		//defaults to 96
			);

		IDXGISurface* surface;
		Renderer::Instance()->ThrowIfFailed(_texture->QueryInterface(IID_IDXGISurface, (void**)&surface));

		ID2D1Bitmap1* bitmap;
		Renderer::Instance()->ThrowIfFailed((*mContext2D)->CreateBitmapFromDxgiSurface(surface, &bitmapProperties, &bitmap));
		return bitmap;
	}
	bool Draw::isColorSame(D2D1::ColorF _color1, D2D1::ColorF _color2)
	{
		if (SAMEFLOAT(_color1.r, _color2.r) &&
				SAMEFLOAT(_color1.g, _color2.g) &&
				SAMEFLOAT(_color1.b, _color2.b) &&
				SAMEFLOAT(_color1.a, _color2.a))
		{
			return true;
		}
		return false;
	}
	bool Font::isColorSame(D2D1::ColorF _color1, D2D1::ColorF _color2)
	{
		if (SAMEFLOAT(_color1.r, _color2.r) &&
				SAMEFLOAT(_color1.g, _color2.g) &&
				SAMEFLOAT(_color1.b, _color2.b) &&
				SAMEFLOAT(_color1.a, _color2.a))
		{
			return true;
		}
		return false;
	}
	void Font::SetColor(D2D1::ColorF _in, D2D1::ColorF _out)
	{
		_in.a = _out.a;
		_in.r = _out.r;
		_in.g = _out.g;
		_in.b = _out.b;
	}


}