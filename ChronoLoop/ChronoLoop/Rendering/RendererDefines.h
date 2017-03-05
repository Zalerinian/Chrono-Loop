#pragma once

namespace Epoch {

	enum RasterState {
		eRS_FILLED = 0,
		eRS_WIREFRAME,
		eRS_CCW_WIREFRAME,
		eRS_CCW,
		eRS_NO_CULL,
		eRS_MAX
	};

	enum VertFormat {
		eVERT_POS = 0,
		eVERT_POSCOLOR,
		eVERT_POSTEX,
		eVERT_POSNORMTEX,
		eVERT_POSNORMTANTEX,
		eVERT_POSBONEWEIGHT,
		eVERT_POSBONEWEIGHTNORMTEX,
		eVERT_POSBONEWEIGHTNORMTANTEX,
		eVERT_MAX
	};

	enum PixelShaderFormat {
		ePS_BASIC = 0,
		ePS_TEXTURED,
		ePS_MAX
	};

	enum VertexShaderFormat {
		eVS_BASIC = 0,
		eVS_TEXTURED,
		eVS_MAX
	};

	enum TextureType {
		eTEX_DIFFUSE = 0,
		eTEX_NORMAL,
		eTEX_SPECULAR,
		eTEX_CUSTOM1,
		eTEX_CUSTOM2,
		eTEX_CUSTOM3,
		eTEX_CUSTOM4,
		eTEX_CUSTOM5,
		eTEX_CUSTOM6,
		eTEX_MAX
	};

	enum RenderEye {
		eEYE_LEFT = 0,
		eEYE_RIGHT,
		eEYE_MAX
	};

}