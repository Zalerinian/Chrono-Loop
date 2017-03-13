#pragma once

#include "Math.h"
#include "Logger.h"

//addmore later 


namespace  Epoch {
	enum InterpolatorType {
		I_Matrix4 = 0,
		I_Vec4f
	};

	template <class Type>
	class Interpolator {
		bool mActive = false;
		//Easing func* 
		void * mTimeFuc;
		float mDuration = -1; //In seconds
		float mTimeElasped = 0;
		//This is the value the interpolator will edit and change for you
		Type* mEdit;
		Type mStart;
		Type mEnd;
		InterpolatorType mType;

	public:
		Interpolator(InterpolatorType _InterpType);
		~Interpolator();
		Type* GetEdit() { return mEdit; };
		//Pass a refrence of the varable you want the interpolator to change
		void SetActive(bool _bool) { mActive = _bool; };
		void SetEdit(Type & _edit) { mEdit = &_edit; };
		void SetStart(Type _start) { mStart = _start; };
		void SetEnd(Type _end) { mEnd = _end; };
		void SetType(InterpolatorType _type) { mType = _type; };
		void Update(float _ratio);
		void Interpolate(Type& _origin, Type& destination,Type& _edit, float _ratio);

	};

	template<class Type>
	inline Interpolator<Type>::Interpolator(InterpolatorType _InterpType) {}

	template<class Type>
	inline Interpolator<Type>::~Interpolator() {}

	template<class Type>
	inline void Interpolator<Type>::Update(float _ratio) {
		if (!mActive)
			return;
		//SystemLogger::GetLog() << _ratio << std::endl;
	Interpolate(mStart, mEnd, *mEdit, _ratio);
	}

	template <class Type>
	void Interpolator<Type>::Interpolate(Type& _origin, Type& destination, Type& _edit, float _ratio) {

		//Generic Solution:
		SystemLogger::GetLog() << "You Tried to interpolate with a wrong interpolator type";
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


