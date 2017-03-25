#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include <d2d1_1.h>
#include <dxgi1_2.h>
#include <memory>
#include <string>
#include <unordered_map>
#include "../Rendering/Renderer.h"

namespace Epoch
{

	struct Font
	{
		wchar_t* mFontName;
		float mFontSize;
		DWRITE_TEXT_ALIGNMENT mTextAlignment;
		DWRITE_PARAGRAPH_ALIGNMENT mParagraphAlignment;
		D2D1::ColorF mColor;

		Font() : mColor(D2D1::ColorF::Black, 1.0f)
		{
			mFontName = L"Vernana";
			mFontSize = 25;
			mTextAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			mParagraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}
		Font(wchar_t _fontName[], float _fontSize, D2D1::ColorF _color) : mColor(_color)
		{
			mFontName = _fontName;
			mFontSize = _fontSize;
			mTextAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			mParagraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}
		Font(wchar_t _fontName[], float _fontSize, D2D1::ColorF _color, DWRITE_TEXT_ALIGNMENT _TextAlignment, DWRITE_PARAGRAPH_ALIGNMENT _ParagraphAlignment) : mColor(_color)
		{
			mFontName = _fontName;
			mFontSize = _fontSize;
			mTextAlignment = _TextAlignment;
			mParagraphAlignment = _ParagraphAlignment;
		}
		~Font(){}
		bool operator==(Font _equals)
		{
			if (this->mFontName == _equals.mFontName &&
					this->mFontSize == _equals.mFontSize &&
					this->mTextAlignment == _equals.mTextAlignment &&
					this->mParagraphAlignment == _equals.mParagraphAlignment &&
					isColorSame(this->mColor, _equals.mColor))
				return true;
			return false;
		}
		void SetColor(D2D1::ColorF _in, D2D1::ColorF _out);
		bool isColorSame(D2D1::ColorF _color1, D2D1::ColorF _color2);
	};
	class Draw
	{
	private:
		static Draw* sInstance;
		std::shared_ptr<ID2D1Factory1*> mTextFactory;
		std::shared_ptr<ID2D1Device*> mDevice2D;
		std::shared_ptr<IDXGIDevice*> mGIDevice;
		std::shared_ptr<ID2D1DeviceContext*>mContext2D;
		std::shared_ptr<IDWriteFactory*> mDWrite;

		std::shared_ptr<ID2D1Bitmap1*>mScreenBitmap;

		unsigned int mFontIDs;
		std::unordered_map<unsigned int, std::pair<Font, IDWriteTextFormat*>> mFonts;
		IDWriteTextFormat* CreateNewTextFormat(Font _font);
		void ProcessFont(Font _font);

		unsigned int mColorIDs;
		std::unordered_map<unsigned int, std::pair<D2D1::ColorF, ID2D1SolidColorBrush*>> mColorBrushes;
		ID2D1SolidColorBrush* CreateNewBrush(D2D1::ColorF _color);
		bool isColorSame(D2D1::ColorF _color1, D2D1::ColorF _color2);


		std::unordered_map<ID3D11Texture2D*, ID2D1Bitmap1*> mBitmaps;
		ID2D1Bitmap1* CreateNewBitmap(ID3D11Texture2D* _texture);
		ID2D1Bitmap1* CreateBitmapForTexture(ID3D11Texture2D* _texture);

	public:
		Draw();
		~Draw();
		static Draw& Instance();
		static void DestroyInstance();

		std::shared_ptr<ID2D1Device*> GetDevice2D() { return mDevice2D; }
		std::shared_ptr<ID2D1DeviceContext*> GetContext2D() { return mContext2D; }
		std::shared_ptr<ID2D1Bitmap1*> GetScreenBitmap() { return mScreenBitmap; }


		ID2D1SolidColorBrush* GetBrush(D2D1::ColorF _color);
		IDWriteTextFormat* GetTextFormat(Font _font);
		ID2D1Bitmap1* GetBitMap(ID3D11Texture2D* _texture);

		void InitializeDirect2D();
		void InitializeScreenBitmap();


		void DrawTextToBitmap(float _left, float _top, float _right, float _bottom,
													Font _font, std::wstring _text, ID2D1Bitmap* _bitmap);
		void DrawRectangleToBitmap(float _left, float _top, float _right, float _bottom, D2D1::ColorF _color, ID2D1Bitmap* _bitmap);
		

	};

}