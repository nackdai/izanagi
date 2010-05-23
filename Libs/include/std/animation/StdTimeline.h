#if !defined(__URANUS_STD_STD_TIMELINE_H__)
#define __URANUS_STD_STD_TIMELINE_H__

#include "unDefs.h"
#include "std/container/StdList.h"

namespace uranus {
	/**
	* �^�C�����C��
	*/
	class CStdTimeline {
	public:
		CStdTimeline();
		CStdTimeline(
			UN_FLOAT fDuration,
			UN_FLOAT fDelay);

		~CStdTimeline() {}

		CStdTimeline(const CStdTimeline& rhs);
		const CStdTimeline& operator=(const CStdTimeline& rhs);

	public:
		// ������
		inline void Init(
			UN_FLOAT fDuration,
			UN_FLOAT fDelay);

		// �i�s
		void Advance(UN_FLOAT fDelta);

		// ���Z�b�g
		inline void Reset();

		// �J�n
		void Start() { m_Flags.is_pause = UN_TRUE; }

		// �X�g�b�v
		inline void Stop();

		// �|�[�Y
		void Pause() { m_Flags.is_pause = UN_TRUE; }

		// �t��
		void Rewind() { m_Flags.is_forward = !m_Flags.is_forward; }

	public:
		// ���ݎ��Ԏ擾
		UN_FLOAT GetTime() const { return m_fTime; }

		UN_FLOAT GetDuration() const { return m_fDuration; }

		// [0.0f - 1.0f]�Ŏ擾
		inline UN_FLOAT GetFraction() const;

		// ���[�v�t���O�Z�b�g
		void SetIsLoop(UN_BOOL flag) { m_Flags.is_loop = flag; }

		// ���[�v���邩�ǂ���
		UN_BOOL IsLoop() const { return m_Flags.is_loop; }

		// �|�[�Y�����ǂ���
		UN_BOOL IsPause() const { return m_Flags.is_pause; }

		// �������i�s�ɂ��邩�ǂ����Z�b�g
		void EnableForwardDir(UN_BOOL flag) { m_Flags.is_forward = flag; }

		// �������i�s���ǂ���
		UN_BOOL IsForwardDir() const { return m_Flags.is_forward; }

	protected:
		CStdList<CStdTimeline>::Item* GetListItem() { return &m_ListItem; }

	protected:
		UN_FLOAT m_fTime;		// ����
		UN_FLOAT m_fDelayTime;

		UN_FLOAT m_fDuration;	// ����
		UN_FLOAT m_fDelay;		// �x��

		struct {
			UN_UINT is_loop		: 1;	// ���[�v���邩
			UN_UINT is_pause	: 1;	// �|�[�Y�����ǂ���
			UN_UINT is_forward	: 1;	// �������i�s���ǂ���
		} m_Flags;

		CStdList<CStdTimeline>::Item m_ListItem;
	};

	// inline *********************************

	/**
	* ������
	*/
	void CStdTimeline::Init(
		UN_FLOAT fDuration,
		UN_FLOAT fDelay)
	{
		m_fDuration = fDuration;
		m_fDelay = fDelay;
	}

	/**
	* ���Z�b�g
	*/
	void CStdTimeline::Reset()
	{
		m_fTime = 0.0f;
		m_fDelayTime = 0.0f;
	}

	/**
	* �X�g�b�v
	*/
	void CStdTimeline::Stop()
	{
		m_fTime = (IsForwardDir() ? 0.0f : m_fDuration);
		m_fDelayTime = 0.0f;

		m_Flags.is_pause = UN_TRUE;
	}

	/**
	* [0.0f - 1.0f]�Ŏ擾
	*/
	UN_FLOAT CStdTimeline::GetFraction() const
	{
		UN_FLOAT ret = (m_fDuration != 0.0f ? m_fTime / m_fDuration : 0.0f);
		return ret;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_TIMELINE_H__)
