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
		Type mEdit;
		Type mStart;
		Type mEnd;
		InterpolatorType mType;

	public:
		Interpolator(InterpolatorType _InterpType);
		~Interpolator();
		Type GetEdit() { return mEdit; };
		//Pass a refrence of the varable you want the interpolator to change
		void SetActive(bool _bool) { mActive = _bool; };
		void SetEdit(Type & _edit) { mEdit = _edit; };
		void SetStart(Type _start) { mStart = _start; };
		void SetEnd(Type _end) { mEnd = _end; };
		void SetType(InterpolatorType _type) { mType = _type; };
		void Update(float _ratio);
		Type Interpolate(Type& _origin, Type& destination, float _ratio);

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
		mEdit = Interpolate(mStart, mEnd, _ratio);
	}

	template <class Type>
	Type Interpolator<Type>::Interpolate(Type& _origin, Type& destination, float _ratio) {

		//Generic Solution:
		SystemLogger::GetLog() << "You Tried to interpolate with a wrong interpolator type";
	}


	template<>
	inline matrix4 Interpolator<matrix4>::Interpolate(matrix4 & _start, matrix4 & _end, float _ratio) {
		matrix4 temp;
		for (unsigned int c = 0; c < 4; c++) {
			for (unsigned int r = 0; r < 4; r++) {
				temp[c][r] = _start[c][r] + (_ratio * (_end[c][r] - _start[c][r]));
			}
		}
		return temp;
	}

	template<>
	inline vec4f Interpolator<vec4f>::Interpolate(vec4f & _start, vec4f & _end, float _ratio) {

		vec4f temp;
		for (unsigned int i = 0; i < 4; i++) {
			temp[i] = _start[i] + _ratio * (_end[i] - _start[i]);
		}
		return temp;
	}
}


