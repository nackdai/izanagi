#pragma once

#include <cstdint>
#include <limits>

// Xor-Shift �ɂ�闐���W�F�l���[�^.
class XorShift {
public:
	XorShift() {}
	XorShift(const unsigned int initial_seed)
	{
		init(initial_seed);
	}

	virtual ~XorShift() {}

	unsigned int next()
	{
		const unsigned int t = m_seed_[0] ^ (m_seed_[0] << 11);
		m_seed_[0] = m_seed_[1];
		m_seed_[1] = m_seed_[2];
		m_seed_[2] = m_seed_[3];
		return m_seed_[3] = (m_seed_[3] ^ (m_seed_[3] >> 19)) ^ (t ^ (t >> 8));
	}

	// [0, 1]
	float nextSample()
	{
		//return (real)next() / (UINT_MAX + 1.0);
        return (float)next() / UINT_MAX;
	}

	void init(uint32_t initial_seed)
	{
		unsigned int s = initial_seed;
		for (int i = 1; i <= 4; i++) {
			m_seed_[i - 1] = s = 1812433253U * (s ^ (s >> 30)) + i;
		}
	}

private:
	unsigned int m_seed_[4];
};
