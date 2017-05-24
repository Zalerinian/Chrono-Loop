#pragma once
#include "../Common/Math/vec4f.h"
#include "../Common/Math/matrix4.h"
#include <memory>

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
		PSTransparentScanline_Data() {
			memset(this, 0, sizeof(*this));
		};
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

	struct PSAnimatedMultiscan_Data {
		union {
			struct {
				float DiffuseAlpha;
				float MultiscanAlpha;
				float MultiscanVOffset;
				float ScanlineAlpha;
				float ScanlineVOffset;
			};
			BufferWidth padding;
		};
		PSAnimatedMultiscan_Data() {
			memset(this, 0, sizeof(*this));
			DiffuseAlpha = 1.0f;
			MultiscanAlpha = 0.4f;
		}
	};

	// --------------------------------------------------------------------------------
	/// <summary>
	/// Data used to send into Register 1 of the Geometry Shader that animates a quad
	/// with an animation sheet.
	/// </summary>
	// --------------------------------------------------------------------------------
	struct GSAnimatedQuad_Data {
		union {
			struct {
				/// <summary>How many columns are there in the animation sheet?</summary>
				unsigned int framesWide;
				/// <summary>How many rows are there in the animation sheet?</summary>
				unsigned int framesTall;
				unsigned int currentFrame;
				/// <summary>Frame Count is not used, but can be assigned so as to make sure that the animation remains in bounds easier, without needing to waste memory storing it elsewhere.</summary>
				unsigned int frameCount; 
			};
			BufferWidth padding;
		};

		GSAnimatedQuad_Data() {
			memset(this, 0, sizeof(*this));
		}
	};

	enum BufferDataType {
		/// <summary>
		/// Represents that the buffer does not contain any data.
		/// </summary>
		eBufferDataType_Nullptr,

		/// <summary>The buffer only holds an alpha value, used to control transparency.</summary>
		eBufferDataType_Alpha,

		/// <summary>
		/// The buffer holds an alpha value, and a vec4f for multi-texturing offsets. 
		/// The vec4f contains, in order, the Multiscan V offset, the Multiscan alpha value,
		/// the Scanline V offset, and the Scanline Alpha value.
		/// </summary>
		eBufferDataType_Scanline,

		eBufferDataType_AnimatedQuad,

		eBufferDataType_Light,

		eBufferDataType_MAX

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
		ePS_BLUR,
		ePS_BLOOM,
		ePS_DEFERRED,
		ePS_SOLIDCOLOR,
		ePS_NONE,
		ePS_LIGHTING,
		ePS_MAX
	};

	enum VertexShaderFormat {
		eVS_BASIC = 0,
		eVS_TEXTURED,
		eVS_NDC,
		eVS_BLUR,
		eVS_MAX
	};

	enum GeometryShaderFormat {
		eGS_PosNormTex = 0,
		eGS_PosNormTex_NDC,
		eGS_PosNormTex_AnimQuad,
		eGS_None,
		eGS_MAX
	};

	enum TextureType {
		eTEX_DIFFUSE = 0,
		eTEX_NORMAL,
		eTEX_SPECULAR,
		eTEX_EMISSIVE,
		eTEX_REGISTER4,
		eTEX_REGISTER5,
		eTEX_REGISTER6,
		eTEX_REGISTER7,
		eTEX_REGISTER8,
		eTEX_REGISTER9,
		eTEX_REGISTER10,
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
		eVB_GlobalMatrix,
		eVB_OFFSET,
		eVB_REGISTER3 = 0,
		eVB_REGISTER4,
		eVB_REGISTER5,
		eVB_REGISTER6,
		eVB_REGISTER7,
		eVB_REGISTER8,
		eVB_REGISTER9,
		eVB_MAX
	};



	enum PixelBufferType {
		ePB_EyePos = 0,
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
		eGB_REGISTER1,
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
