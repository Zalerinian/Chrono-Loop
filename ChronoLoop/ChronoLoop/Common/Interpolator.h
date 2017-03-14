#pragma once

#include "Math.h"
#include "Logger.h"
#include "EasingFunctions.h"

//addmore later 


namespace  Epoch {

	// The easing function for the interpolator.
	// _time is the current time in the animation, in seconds.
	// _begin is the starting value for this element.
	// _delta is the chnge from the start to the end (not the end position, but the change).
	// _duration is the length of the interpolation, in seconds.
	typedef float(*EasingFunction)(float _time, float _begin, float _delta, float _duration);

	template <class Type>
	class Interpolator {
		bool mActive = false;
		EasingFunction mEasingFunction = Easing::Linear;
		float mDuration = 0; //In seconds
		float mTweenTime = 0;
		//This is the value the interpolator will edit and change for you
		Type* mEdit = nullptr;
		Type mStart;
		Type mEnd;

	public:
		Interpolator();
		Interpolator(float _duration, Type& _from, Type& _to, Type* _edit);
		~Interpolator();
		inline void SetActive(bool _bool) { mActive = _bool; };
		inline void SetEasingFunction(EasingFunction _m) { mEasingFunction = _m; }
		void Prepare(float _duration, Type& _start, Type& _end, Type& _edit);
		void Update(float _deltaTime);
		void Interpolate();
	};

	template <class Type>
	Interpolator<Type>::Interpolator() {
		mTweenTime = 0;
		mDuration = 0;
		memset(&mStart, 0, sizeof(Type));
		memset(&mEnd, 0, sizeof(Type));
		mEdit = nullptr;
	}

	template <class Type>
	Interpolator<Type>::Interpolator(float _duration, Type& _from, Type& _to, Type* _edit) {
		Prepare(_duration, _from, _to, _edit);
	}

	template<class Type>
	inline Interpolator<Type>::~Interpolator() {}

	template<class Type>
	inline void Interpolator<Type>::Update(float _deltaTime) {
		if (!mActive)
			return;
		if (mDuration <= 0) {
			SystemLogger::Error() << "The duration for an interpolator is invalid: " << mDuration << std::endl;
			return;
		}
		mTweenTime += _deltaTime;
		Interpolate();
	}

	template <class Type>
	void Interpolator<Type>::Interpolate() {

		//Generic Solution:
		SystemLogger::Warn() << "You interpolated in the wrong neighborhood (The generic interpolate function has been called)." << std::endl;
	}

	template<class Type>
	inline void Interpolator<Type>::Prepare(float _duration, Type & _start, Type & _end, Type & _edit) {
		mStart = _start;
		mEnd = _end;
		mEdit = &_edit;
		mDuration = _duration;
		mTweenTime = 0;
	}


	template<>
	inline void Interpolator<matrix4>::Interpolate() {
		for (unsigned int c = 0; c < 4; c++) {
			for (unsigned int r = 0; r < 4; r++) {
				(*mEdit)[c][r] = mEasingFunction(mTweenTime, mStart[c][r], (mStart[c][r] - mEnd[c][r]), mDuration);
			}
		}
	}

	template<>
	inline void Interpolator<vec4f>::Interpolate() {
		for (unsigned int i = 0; i < 4; i++) {
			(*mEdit)[i] = mEasingFunction(mTweenTime, mStart[i], (mStart[i] - mEnd[i]), mDuration);
		}

	}
}


