#pragma once

#include "Math.h"
#include "Logger.h"

//addmore later 


namespace  Epoch {

	typedef float(*EasingFunction)(float, float, float, float);

	template <class Type>
	class Interpolator {
		bool mActive = false;
		//Easing func* 

		// Easing functions use Time = t, Begin = b, Change = c, Duration = d
		EasingFunction mTimeFuc = nullptr;
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
		inline Type* GetEdit() { return mEdit; };
		inline void  SetActive(bool _bool) { mActive = _bool; };
		inline void  SetEdit(Type & _edit) { mEdit = &_edit; };
		inline void  SetStart(Type& _start) { mTweenTime = 0; mStart = _start; };
		inline void  SetEnd(Type& _end) { mTweenTime = 0; mEnd = _end; };
		void Update(float _deltaTime);
		void Interpolate(Type& _origin, Type& destination, Type& _edit, float _ratio);

	};

	template <class Type>
	Interpolator<Type>::Interpolator() {
		mTweenTime = 0;
		mDuration = 0.1f;
		memset(&mStart, 0, sizeof(Type));
		memset(&mEnd, 0, sizeof(Type));
		mEdit = nullptr;
	}

	template <class Type>
	Interpolator<Type>::Interpolator(float _duration, Type& _from, Type& _to, Type* _edit) {
		mDuration = _duration;
		mStart = _from;
		mEnd = _to;
		mEdit = _edit;
		mTweenTime = 0;
	}

	template<class Type>
	inline Interpolator<Type>::~Interpolator() {}

	template<class Type>
	inline void Interpolator<Type>::Update(float _deltaTime) {
		if (!mActive)
			return;
		mTweenTime += _deltaTime;
		Interpolate(mStart, mEnd, *mEdit, mTweenTime / mDuration);
	}

	template <class Type>
	void Interpolator<Type>::Interpolate(Type& _origin, Type& destination, Type& _edit, float _ratio) {

		//Generic Solution:
		SystemLogger::GetLog() << "You interpolated in the wrong neighborhood";
	}


	template<>
	inline void Interpolator<matrix4>::Interpolate(matrix4 & _start, matrix4 & _end, matrix4 & _edit, float _ratio) {

		for (unsigned int c = 0; c < 4; c++) {
			for (unsigned int r = 0; r < 4; r++) {
				_edit[c][r] = _start[c][r] + (_ratio * (_end[c][r] - _start[c][r]));
			}
		}
	}

	template<>
	inline void Interpolator<vec4f>::Interpolate(vec4f & _start, vec4f & _end, vec4f & _edit, float _ratio) {

		for (unsigned int i = 0; i < 4; i++) {
			_edit[i] = _start[i] + _ratio * (_end[i] - _start[i]);
		}

	}
}


