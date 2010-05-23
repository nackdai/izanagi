#if !defined(__URANUS_STD_STD_PAIR_H__)
#define __URANUS_STD_STD_PAIR_H__

#include "unDefs.h"

namespace uranus {
	/**
	*/
	template <typename _KEY, typename _T>
	class CPair {
	protected:
		CPair(const _KEY& key, const _T& val) { m_Key = key; m_Val = val; }
		~CPair() {}

		NO_COPIABLE(CPair);

	public:
		// Sets the key and the value.
		void Set(const _KEY& key, const _T& val) { m_Key = key; m_Val = val; }

		// Returns whether the key equals.
		UN_BOOL IsEqual(const _KEY& key) { return (m_Key == key); }

		// Returns the key.
		const _KEY& GetKey() const { return m_Key; }

		// Returns the value.
		const _T& GetValue() const { return m_Val; }
		_T& GetValue() { return m_Val; }

	protected:
		_KEY m_Key;
		_T m_Val;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_RESOURCE_H__)
