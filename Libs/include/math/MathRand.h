#if !defined(__URANUS__MATH_RAND_H__)
#define __URANUS__MATH_RAND_H__

#include "../unDefs.h"
#include "SFMT/SFMT.h"

namespace uranus {
	// ��������
	// �����Z���k�E�c�C�X�^�����ɂ������������A���S���Y�����g�p
	class CMathRand {
	private:
		CMathRand();
		~CMathRand();

		CMathRand(const CMathRand& rhs);
		const CMathRand& operator=(const CMathRand& rhs);

	public:
		// ������
		static inline void Init(UN_UINT32 seed);

		// ��������
		static inline UN_UINT32 GenRandUInt32();
		static inline void GetRandUInt32Array(UN_UINT32* array, UN_UINT32 num);
		static inline UN_FLOAT GetRandFloat();
		static inline void GetRandVector(SVector& v);
		static inline void GetRandVectorXYZ(SVector& v);

		template <typename _T>
		static inline _T GetRandBetween(_T tMin, _T tMax);
	};

	// inline ***************************
	
	/**
	* ������
	*/
	void CMathRand::Init(UN_UINT32 seed)
	{
		init_gen_rand(seed);
	}

	/**
	* ��������
	*/
	UN_UINT32 CMathRand::GenRandUInt32()
	{
		UN_UINT32 ret = gen_rand32();
		return ret;
	}

	/**
	* ��������
	*
	* array�̃A�h���X�͂P�U�o�C�g�A���C������Ă��邱�ƁI�I
	*/
	void CMathRand::GetRandUInt32Array(UN_UINT32* array, UN_UINT32 num)
	{
		fill_array32(array, num);
	}

	/**
	* ��������
	*/
	UN_FLOAT CMathRand::GetRandFloat()
	{
		double ret = genrand_real1();
		return (UN_FLOAT)ret;
	}

	/**
	* ��������
	*/
	void CMathRand::GetRandVector(SVector& v)
	{
		v.x = GetRandFloat();
		v.y = GetRandFloat();
		v.z = GetRandFloat();
		v.w = GetRandFloat();
	}

	/**
	* ��������
	*/
	void CMathRand::GetRandVectorXYZ(SVector& v)
	{
		v.x = GetRandFloat();
		v.y = GetRandFloat();
		v.z = GetRandFloat();
	}

	/**
	* ��������
	*
	* �Q�l�̊Ԃ̗������擾
	*/
	template <typename _T>
	_T CMathRand::GetRandBetween(_T tMin, _T tMax)
	{
		if (tMin == tMax) {
			return tMin;
		}

		UN_FLOAT tmp = GetRandFloat();
		_T ret = (_T)(tmp * (tMax - tMin) + tMin);
		return ret;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS__MATH_RAND_H__)
