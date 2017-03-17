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
		eVB_SLOT2 = 0,
		eVB_SLOT3,
		eVB_SLOT4,
		eVB_SLOT5,
		eVB_SLOT6,
		eVB_SLOT7,
		eVB_SLOT8,
		eVB_SLOT9,
		eVB_SLOT10,
		eVB_SLOT11,
		eVB_SLOT12,
		eVB_SLOT13,
		eVB_SLOT14,
		eVB_SLOT15,
		eVB_MAX,
		eVB_OFFSET = 1,
		eVB_BEGIN = eVB_SLOT2
	};

	enum PixelBufferType {
		ePB_SLOT2 = 0,
		ePB_SLOT3,
		ePB_SLOT4,
		ePB_SLOT5,
		ePB_SLOT6,
		ePB_SLOT7,
		ePB_SLOT8,
		ePB_SLOT9,
		ePB_SLOT10,
		ePB_SLOT11,
		ePB_SLOT12,
		ePB_SLOT13,
		ePB_SLOT14,
		ePB_SLOT15,
		ePB_MAX,
		ePB_OFFSET = 1,
		ePB_BEGIN = ePB_SLOT2
	};

	enum GeometryBufferType {
		eGB_SLOT2 = 0,
		eGB_SLOT3,
		eGB_SLOT4,
		eGB_SLOT5,
		eGB_SLOT6,
		eGB_SLOT7,
		eGB_SLOT8,
		eGB_SLOT9,
		eGB_SLOT10,
		eGB_SLOT11,
		eGB_SLOT12,
		eGB_SLOT13,
		eGB_SLOT14,
		eGB_SLOT15,
		eGB_MAX,
		eGB_OFFSET = 1,
		eGB_BEGIN = eGB_SLOT2
	};

}