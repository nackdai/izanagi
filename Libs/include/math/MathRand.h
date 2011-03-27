#if !defined(__IZANAGI__MATH_RAND_H__)
#define __IZANAGI__MATH_RAND_H__

#include "izDefs.h"
#include "SFMT/SFMT.h"

namespace izanagi {
	// 乱数生成
	// メルセンヌ・ツイスタを元にした乱数生成アルゴリズムを使用
	class CMathRand {
	private:
		CMathRand();
		~CMathRand();

		CMathRand(const CMathRand& rhs);
		const CMathRand& operator=(const CMathRand& rhs);

	public:
		// 初期化
		static inline void Init(IZ_UINT32 seed);

		// 乱数生成
		static inline IZ_UINT32 GenRandUInt32();
		static inline void GetRandUInt32Array(IZ_UINT32* array, IZ_UINT32 num);
		static inline IZ_FLOAT GetRandFloat();
		static inline void GetRandVector(SVector& v);
		static inline void GetRandVectorXYZ(SVector& v);

		template <typename _T>
		static inline _T GetRandBetween(_T tMin, _T tMax);
	};

	// inline ***************************
	
	/**
	* 初期化
	*/
	void CMathRand::Init(IZ_UINT32 seed)
	{
		init_gen_rand(seed);
	}

	/**
	* 乱数生成
	*/
	IZ_UINT32 CMathRand::GenRandUInt32()
	{
		IZ_UINT32 ret = gen_rand32();
		return ret;
	}

	/**
	* 乱数生成
	*
	* arrayのアドレスは１６バイトアラインされていること！！
	*/
	void CMathRand::GetRandUInt32Array(IZ_UINT32* array, IZ_UINT32 num)
	{
		fill_array32(array, num);
	}

	/**
	* 乱数生成
	*/
	IZ_FLOAT CMathRand::GetRandFloat()
	{
		double ret = genrand_real1();
		return (IZ_FLOAT)ret;
	}

	/**
	* 乱数生成
	*/
	void CMathRand::GetRandVector(SVector& v)
	{
		v.x = GetRandFloat();
		v.y = GetRandFloat();
		v.z = GetRandFloat();
		v.w = GetRandFloat();
	}

	/**
	* 乱数生成
	*/
	void CMathRand::GetRandVectorXYZ(SVector& v)
	{
		v.x = GetRandFloat();
		v.y = GetRandFloat();
		v.z = GetRandFloat();
	}

	/**
	* 乱数生成
	*
	* ２値の間の乱数を取得
	*/
	template <typename _T>
	_T CMathRand::GetRandBetween(_T tMin, _T tMax)
	{
		if (tMin == tMax) {
			return tMin;
		}

		IZ_FLOAT tmp = GetRandFloat();
		_T ret = (_T)(tmp * (tMax - tMin) + tMin);
		return ret;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI__MATH_RAND_H__)
