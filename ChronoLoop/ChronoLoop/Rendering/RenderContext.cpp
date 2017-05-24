//#include "stdafx.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "RasterizerStateManager.h"
#include "ShaderManager.h"
#include "InputLayoutManager.h"
#include "../Common/Settings.h"

namespace Epoch {

	RenderContext::RenderContext() {
		mType = RenderNodeType::Context;
		mName = "A rendering context.";
		//	mTextures.insert(std::pair<int, ID3D11ShaderResourceView*>(1, nullptr));
	}

	RenderContext::RenderContext(const RenderContext & _copy) {
		mRasterState = _copy.mRasterState;
		mVertexFormat = _copy.mVertexFormat;
		mPixelShaderFormat = _copy.mPixelShaderFormat;
		mVertexShaderFormat = _copy.mVertexShaderFormat;
		mGeoShaderFormat = _copy.mGeoShaderFormat;
		mType = _copy.mType;
		for (int i = eTEX_DIFFUSE; i < eTEX_MAX; ++i) {
			mTextures[i] = _copy.mTextures[i];
		}

		for (int i = 0; i < eVB_MAX; ++i) {
			mVertexCBuffers[i] = _copy.mVertexCBuffers[i];
		}
		for (int i = 0; i < ePB_MAX; ++i) {
			mPixelCBuffers[i] = _copy.mPixelCBuffers[i];
		}
		for (int i = 0; i < eGB_MAX; ++i) {
			mGeometryCBuffers[i] = _copy.mGeometryCBuffers[i];
		}
	}

	RenderContext::~RenderContext() {}

	void RenderContext::Apply() {
		RasterState RasterOverride = (RasterState)Settings::GetInstance().GetInt("RasterizerStateOverride");
		PixelShaderFormat PixelOverride = (PixelShaderFormat)Settings::GetInstance().GetInt("PixelShaderOverride");
		if (RasterOverride != eRS_MAX) {
			RasterizerStateManager::Instance()->ApplyState(RasterOverride);
		} else if (mRasterState != eRS_MAX) {
			RasterizerStateManager::Instance()->ApplyState(mRasterState);
		}
		if (mVertexFormat != eVERT_MAX) {
			InputLayoutManager::Instance().ApplyLayout(mVertexFormat);
		}
		if (PixelOverride != ePS_MAX) {
			ShaderManager::Instance()->ApplyPShader(PixelOverride);
		} else if (mPixelShaderFormat != ePS_MAX) {
			ShaderManager::Instance()->ApplyPShader(mPixelShaderFormat);
		}
		if (mVertexShaderFormat != eVS_MAX) {
			ShaderManager::Instance()->ApplyVShader(mVertexShaderFormat);
		}
		if (mGeoShaderFormat != eGS_MAX) {
			ShaderManager::Instance()->ApplyGShader(mGeoShaderFormat);
		}

		ID3D11ShaderResourceView* textures[eTEX_MAX];
		for (int i = eTEX_DIFFUSE; i < eTEX_MAX; ++i) {
			textures[i] = mTextures[i].Get();
		}
		Renderer::Instance()->GetContext()->PSSetShaderResources(0, eTEX_MAX, textures);

		ID3D11Buffer *pixelBuffers[ePB_MAX], *vertexBuffers[eVB_MAX], *geoBuffers[eGB_MAX];
		for (int i = 0; i < eVB_MAX; ++i) {
			vertexBuffers[i] = mVertexCBuffers[i].Get();
		}
		for (int i = 0; i < ePB_MAX; ++i) {
			pixelBuffers[i] = mPixelCBuffers[i].Get();
		}
		for (int i = 0; i < eGB_MAX; ++i) {
			geoBuffers[i] = mGeometryCBuffers[i].Get();
		}
		
		Renderer::Instance()->GetContext()->VSSetConstantBuffers(eVB_OFFSET, eVB_MAX, vertexBuffers);
		Renderer::Instance()->GetContext()->PSSetConstantBuffers(ePB_OFFSET, ePB_MAX, pixelBuffers);
		Renderer::Instance()->GetContext()->GSSetConstantBuffers(eGB_OFFSET, eGB_MAX, geoBuffers);
	}

	void RenderContext::Apply(RenderContext & from) {
		RasterState RasterOverride = (RasterState)Settings::GetInstance().GetInt("RasterizerStateOverride");
		PixelShaderFormat PixelOverride = (PixelShaderFormat)Settings::GetInstance().GetInt("PixelShaderOverride");
		if (RasterOverride != eRS_MAX) {
			RasterizerStateManager::Instance()->ApplyState(RasterOverride);
		} else if (mRasterState != eRS_MAX && mRasterState != from.mRasterState) {
			RasterizerStateManager::Instance()->ApplyState(mRasterState);
		}
		if (mVertexFormat != eVERT_MAX && mVertexFormat != from.mVertexFormat) {
			InputLayoutManager::Instance().ApplyLayout(mVertexFormat);
		}
		if (PixelOverride != ePS_MAX) {
			ShaderManager::Instance()->ApplyPShader(PixelOverride);
		} else if (mPixelShaderFormat != ePS_MAX && mPixelShaderFormat != from.mPixelShaderFormat) {
			ShaderManager::Instance()->ApplyPShader(mPixelShaderFormat);
		}
		if (mVertexShaderFormat != eVS_MAX && mVertexShaderFormat != from.mVertexShaderFormat) {
			ShaderManager::Instance()->ApplyVShader(mVertexShaderFormat);
		}
		if (mGeoShaderFormat != eGS_MAX && mGeoShaderFormat != from.mGeoShaderFormat) {
			ShaderManager::Instance()->ApplyGShader(mGeoShaderFormat);
		}

		ID3D11ShaderResourceView* textures[eTEX_MAX];
		for (int i = eTEX_DIFFUSE; i < eTEX_MAX; ++i) {
			textures[i] = mTextures[i].Get();
		}
		Renderer::Instance()->GetContext()->PSSetShaderResources(0, eTEX_MAX, textures);

		ID3D11Buffer *pixelBuffers[ePB_MAX], *vertexBuffers[eVB_MAX], *geoBuffers[eGB_MAX];
		for (int i = 0; i < eVB_MAX; ++i) {
			vertexBuffers[i] = mVertexCBuffers[i].Get();
		}
		for (int i = 0; i < ePB_MAX; ++i) {
			pixelBuffers[i] = mPixelCBuffers[i].Get();
		}
		for (int i = 0; i < eGB_MAX; ++i) {
			geoBuffers[i] = mGeometryCBuffers[i].Get();
		}
		Renderer::Instance()->GetContext()->VSSetConstantBuffers(eVB_OFFSET, eVB_MAX, vertexBuffers);
		Renderer::Instance()->GetContext()->PSSetConstantBuffers(ePB_OFFSET, ePB_MAX, pixelBuffers);
		Renderer::Instance()->GetContext()->GSSetConstantBuffers(eGB_OFFSET, eGB_MAX, geoBuffers);
	}

	bool RenderContext::operator==(const RenderContext & other) const {
		for (int i = eTEX_DIFFUSE; i < eTEX_MAX; ++i) {
			if (/*other.mTextures[i].Get() == nullptr || */other.mTextures[i].Get() == this->mTextures[i].Get()) {
				// If the textures aren't different, or aren't used, we can continue on.
				continue;
			}
			return false;
		}

		if (mRasterState != other.mRasterState ||
			mVertexFormat != other.mVertexFormat ||
			mPixelShaderFormat != other.mPixelShaderFormat ||
			mVertexShaderFormat != other.mVertexShaderFormat ||
			mGeoShaderFormat != other.mGeoShaderFormat) {
			return false;
		}

		return true;
	}

	RenderContext & RenderContext::operator=(const RenderContext & _other) {
		mRasterState = _other.mRasterState;
		mVertexFormat = _other.mVertexFormat;
		mPixelShaderFormat = _other.mPixelShaderFormat;
		mVertexShaderFormat = _other.mVertexShaderFormat;
		mGeoShaderFormat = _other.mGeoShaderFormat;
		mType = _other.mType;
		for (int i = eTEX_DIFFUSE; i < eTEX_MAX; ++i) {
			mTextures[i] = _other.mTextures[i];
		}

		for (int i = 0; i < eVB_MAX; ++i) {
			mVertexCBuffers[i] = _other.mVertexCBuffers[i];
		}
		for (int i = 0; i < ePB_MAX; ++i) {
			mPixelCBuffers[i] = _other.mPixelCBuffers[i];
		}
		for (int i = 0; i < eGB_MAX; ++i) {
			mGeometryCBuffers[i] = _other.mGeometryCBuffers[i];
		}
		return *this;
	}

	RenderContext & RenderContext::SimpleClone(const RenderContext & _other) {
		RasterState RasterOverride = (RasterState)Settings::GetInstance().GetInt("RasterizerStateOverride");
		PixelShaderFormat PixelOverride = (PixelShaderFormat)Settings::GetInstance().GetInt("PixelShaderOverride");
		mRasterState = RasterOverride != eRS_MAX ? RasterOverride : _other.mRasterState;
		mVertexFormat = _other.mVertexFormat;
		mPixelShaderFormat = PixelOverride != ePS_MAX ? PixelOverride : _other.mPixelShaderFormat;
		mVertexShaderFormat = _other.mVertexShaderFormat;
		mGeoShaderFormat = _other.mGeoShaderFormat;
		mType = _other.mType;
		return *this;
	}

	/*bool RenderContext::operator==(const RenderContext & _other) const {
		for (int i = eTEX_DIFFUSE; i < eTEX_MAX; ++i) {
			if (_other.mTextures.count(i) != 0) {
				if (_other.mTextures.at(i).Get() == nullptr ||
					(this->mTextures.count(i) != 0 && _other.mTextures.at(i).Get() == this->mTextures.at(i).Get())) {
					continue;
				}
				return false;
			}
		}

		if (mRasterState != _other.mRasterState ||
				mVertexFormat != _other.mVertexFormat ||
				mPixelShaderFormat != _other.mPixelShaderFormat ||
				mVertexShaderFormat != _other.mVertexShaderFormat ||
				mGeoShaderFormat != _other.mGeoShaderFormat) {
			return false;
		}

		return true;
	}*/

	bool Epoch::RenderContext::operator!=(RenderContext & _other) {
		return !(this->operator==(_other));
	}

}