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
		ePS_POSTPROCESS,
		ePS_PURETEXTURE,
		ePS_TRANSPARENT,
		ePS_MAX
	};

	enum VertexShaderFormat {
		eVS_BASIC = 0,
		eVS_TEXTURED,
		eVS_NDC,
		eVS_MAX
	};

	enum GeometryShaderFormat {
		eGS_PosNormTex = 0,
		eGS_PosNormTex_NDC,
		eGS_MAX
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


	// Each shader stage can have up to 15 constant buffers, each with up to
	// 4096 values (float4, etc is 4 values).
	// Some slots are reserved for global shader operations, and as a result
	// are not available in these enums, and the OFFSET values are used to properly
	// assign the shaders at run-time.
	enum VertexBufferType {
		eVB_Instances = 0,
		eVB_CUSTOM1 = 0,
		eVB_CUSTOM2,
		eVB_CUSTOM3,
		eVB_CUSTOM4,
		eVB_CUSTOM5,
		eVB_CUSTOM6,
		eVB_CUSTOM7,
		eVB_CUSTOM8,
		eVB_CUSTOM9,
		eVB_MAX,
		eVB_OFFSET = eVB_Instances + 1,
		eVB_BEGIN = 0
	};



	enum PixelBufferType {
		ePB_Lights = 0,
		ePB_TP_Alpha = 0,
		ePB_PP_Ratios = 0,
		ePB_CUSTOM1 = 0,
		ePB_CUSTOM2,
		ePB_CUSTOM3,
		ePB_CUSTOM4,
		ePB_CUSTOM5,
		ePB_CUSTOM6,
		ePB_CUSTOM7,
		ePB_CUSTOM8,
		ePB_CUSTOM9,
		ePB_MAX,
		ePB_OFFSET = ePB_Lights + 1,
		ePB_BEGIN = 0
	};

	enum GeometryBufferType {
		eGB_Eyes = 0,
		eGB_CUSTOM1 = 0,
		eGB_CUSTOM2,
		eGB_CUSTOM3,
		eGB_CUSTOM4,
		eGB_CUSTOM5,
		eGB_CUSTOM6,
		eGB_CUSTOM7,
		eGB_CUSTOM8,
		eGB_CUSTOM9,
		eGB_MAX,
		eGB_OFFSET = eGB_Eyes + 1,
		eGB_BEGIN = 0
	};

}