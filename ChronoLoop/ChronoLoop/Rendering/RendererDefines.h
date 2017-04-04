#pragma once
#include "../Common/Math/vec4f.h"
#include "../Common/Math/matrix4.h"

namespace Epoch {

	struct BufferWidth {
		matrix4 e1, e2;
	};

	struct PSTransparentScanline_Data {
		union {
			struct {
				float alpha;

				// X: Multiscan V offset
				// Y: Multiscan Alpha
				// Z: Scanline V Offset
				// W: Scanline Alpha
				vec4f ScanlineData;
				float p1, p2, p3;
			};
			BufferWidth padding;
		};
		PSTransparentScanline_Data() {};
	};

	struct PSTransparent_Data {
		union {
			struct {
				vec4f alpha;
			};
			BufferWidth padding;
		};

		PSTransparent_Data() {
			alpha.Set(0, 0, 0, 0);
		}
	};


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
		ePS_TRANSPARENT_SCANLINE,
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
		eVB_SimInstanceID,
		eVB_OFFSET,
		eVB_REGISTER2 = 0,
		eVB_REGISTER3,
		eVB_REGISTER4,
		eVB_REGISTER5,
		eVB_REGISTER6,
		eVB_REGISTER7,
		eVB_REGISTER8,
		eVB_REGISTER9,
		eVB_MAX
	};



	enum PixelBufferType {
		ePB_Lights = 0,
		ePB_OFFSET,
		ePB_REGISTER1 = 0,
		ePB_REGISTER2,
		ePB_REGISTER3,
		ePB_REGISTER4,
		ePB_REGISTER5,
		ePB_REGISTER6,
		ePB_REGISTER7,
		ePB_REGISTER8,
		ePB_REGISTER9,
		ePB_MAX
	};

	enum GeometryBufferType {
		eGB_Eyes = 0,
		eGB_OFFSET,
		eGB_REGISTER1 = 0,
		eGB_REGISTER2,
		eGB_REGISTER3,
		eGB_REGISTER4,
		eGB_REGISTER5,
		eGB_REGISTER6,
		eGB_REGISTER7,
		eGB_REGISTER8,
		eGB_REGISTER9,
		eGB_MAX
	};

	enum ConstantBufferType {
		eCB_VERTEX = 0,
		eCB_PIXEL,
		eCB_GEO,
		eCB_MAX
	};

}
