#include "FrameQuery.h"

Epoch::FrameQuery::FrameQuery(Microsoft::WRL::ComPtr<ID3D11Device>& _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& _context, unsigned int _flags) {
	mDevice = _device;
	mContext = _context;
	mFlags = _flags;
	D3D11_QUERY_DESC desc;
	desc.MiscFlags = 0;
	desc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	mDevice->CreateQuery(&desc, &mDisjoint);

	desc.Query = D3D11_QUERY_TIMESTAMP;
	mDevice->CreateQuery(&desc, &mFrameBegin);
	mDevice->CreateQuery(&desc, &mFrameEnd);

	if (_flags & FrameQueryFlag_Track_Pipeline) {
		desc.Query = D3D11_QUERY_PIPELINE_STATISTICS;
		mDevice->CreateQuery(&desc, &mPipelineStats);
	}
	if (_flags & FrameQueryFlag_Track_Occlusion) {
		desc.Query = D3D11_QUERY_OCCLUSION;
		mDevice->CreateQuery(&desc, &mOcclusionStats);
	}
}

void Epoch::FrameQuery::Begin() {
	mContext->Begin(mDisjoint);
	mContext->End(mFrameBegin);
	if (mPipelineStats) {
		mContext->Begin(mPipelineStats);
	}
	if (mOcclusionStats) {
		mContext->Begin(mOcclusionStats);
	}
}

void Epoch::FrameQuery::Query(char * _name) {
	ID3D11Query *query;
	FrameDescription fd;
	D3D11_QUERY_DESC d;
	d.MiscFlags = 0;
	d.Query = D3D11_QUERY_TIMESTAMP;
	mDevice->CreateQuery(&d, &query);
	mContext->End(query);

	fd.name = _name;
	fd.query = query;
	mQueries.push_back(fd);
}

void Epoch::FrameQuery::End() {
	mContext->End(mDisjoint);
	mContext->End(mFrameEnd);
	if (mPipelineStats) {
		mContext->End(mPipelineStats);
	}
	if (mOcclusionStats) {
		mContext->End(mOcclusionStats);
	}
}

bool Epoch::FrameQuery::Ready() {
	return mContext->GetData(mDisjoint, nullptr, 0, 0) == S_OK;
}

void Epoch::FrameQuery::Display() {
	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoint;
	mContext->GetData(mDisjoint, &disjoint, sizeof(disjoint), 0);
	if (disjoint.Disjoint) {
		SystemLogger::GetLog() << "\t" << "The frame was disjoint." << std::endl;
		return;
	}

	UINT64 *frameTimes = new UINT64[mQueries.size() + 2];
	unsigned int index = 0;
	mContext->GetData(mFrameBegin, &frameTimes[index++], sizeof(UINT64), 0);
	mContext->GetData(mFrameEnd, &frameTimes[index++], sizeof(UINT64), 0);
	for (auto it = mQueries.begin(); it != mQueries.end(); ++it) {
		mContext->GetData((*it).query, &frameTimes[index++], sizeof(UINT64), 0);
	}

	float frequency = float(disjoint.Frequency);
	float frameTime = (frameTimes[index - 1] - frameTimes[0]) / frequency * 1000.f;

	SystemLogger::GetLog() << "\t" << "Frame Time: " << frameTime << "ms\n";
	UINT64 previousTime = frameTimes[0];
	for (unsigned int i = 0; i < mQueries.size(); ++i) {
		float sectionTime = (frameTimes[i + 2] - previousTime) / frequency * 1000.f;
		SystemLogger::GetLog() << "\t\t" << mQueries[i].name << ": " << sectionTime << "ms\n";
		previousTime = frameTimes[i + 2];
	}

	if (mPipelineStats) {
		D3D11_QUERY_DATA_PIPELINE_STATISTICS pipeline;
		mContext->GetData(mPipelineStats, &pipeline, sizeof(pipeline), 0);

		SystemLogger::GetLog() << "\tPipeline Statisics: \n\t\t";
		SystemLogger::GetLog() << "";
	}

	if (mOcclusionStats) {
		UINT64 occlusion;
		mContext->GetData(mOcclusionStats, &occlusion, sizeof(occlusion), 0);
		SystemLogger::GetLog() << "\tOcclusion Stats:\n\t\tOccluded Samples: " << occlusion << "\n";
	}

	SystemLogger::GetLog() << std::endl;
}

Epoch::FrameQuery::~FrameQuery() {
	for (auto it = mQueries.begin(); it != mQueries.end(); ++it) {
		if (it->query != nullptr) {
			it->query->Release();
		}
	}
	mDisjoint->Release();
	mFrameBegin->Release();
	mFrameEnd->Release();
	if (mPipelineStats) {
		mPipelineStats->Release();
	}
	if (mOcclusionStats) {
		mOcclusionStats->Release();
	}
}

void Epoch::FrameQuerySet::Prepare(Microsoft::WRL::ComPtr<ID3D11Device>& _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& _context, unsigned int _flags, unsigned int _count) {
	Clear();
	mFrames.resize(_count);
	for (unsigned int i = 0; i < _count; ++i) {
		mFrames[i] = (new FrameQuery(_device, _context, _flags));
	}
}

void Epoch::FrameQuerySet::Clear() {
	mFrames.clear();
	index = 0;
}

bool Epoch::FrameQuerySet::Ready() {
	if (index < mFrames.size()) {
		return false;
	}
	for (unsigned int i = 0; i < mFrames.size(); ++i) {
		if (!mFrames[i]->Ready()) {
			return false;
		}
	}
	return true;
}

void Epoch::FrameQuerySet::Display() {
	for (unsigned int i = 0; i < mFrames.size(); ++i) {
		SystemLogger::GetLog() << "Frame " << (i + 1) << " of " << mFrames.size() << ":\n";
		mFrames[i]->Display();
	}
}
