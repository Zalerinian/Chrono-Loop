#pragma once
#include <d3d11.h>
#include <unordered_map>
#include <wrl/client.h>
#include "../Common/Settings.h"
#include "../Common/Logger.h"

namespace Epoch {

	enum FrameQueryFlags {
		FrameQueryFlag_Track_Pipeline = 1,
		FrameQueryFlag_Track_Occlusion = 2
	};

	class FrameQuery {
		struct FrameDescription {
			char *name = "";
			ID3D11Query *query = nullptr;
		};

		unsigned int mFlags = 0;

		ID3D11Query *mDisjoint = nullptr, *mFrameBegin = nullptr, *mFrameEnd = nullptr, *mPipelineStats = nullptr, *mOcclusionStats = nullptr;
		std::vector<FrameDescription> mQueries;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;

	public:
		FrameQuery(Microsoft::WRL::ComPtr<ID3D11Device>& _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& _context, unsigned int _flags);
		void Begin();
		void Query(char* _name);
		void End();
		bool Ready();
		void Display();
		~FrameQuery();
	};

	class FrameQuerySet {
		std::vector<FrameQuery*> mFrames;
		unsigned int index;

	public:
		inline size_t GetSize() {
			return mFrames.size();
		}

		inline unsigned int GetCurrentIndex() {
			return index;
		}

		inline bool Valid() {
			return mFrames.size() > 0 && index < mFrames.size();
		}

		inline void Begin() {
			mFrames[index]->Begin();
		}

		inline void Query(char *_name) {
			mFrames[index]->Query(_name);
		}

		inline void End() {
			mFrames[index++]->End();
		}


		void Prepare(Microsoft::WRL::ComPtr<ID3D11Device>& _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& _context, unsigned int _flags, unsigned int _count);
		void Clear();
		bool Ready();
		void Display();

	};

}