#if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_BUTTON_H__)
#define __URANUS_DEBUGUTIL_DEBUG_UI_BUTTON_H__

#include "DebugUIWidget.h"

namespace uranus {
	/**
	* UIボタン
	*/
	class CDebugUIButton : public CDebugUIWidget {
	public:
		// クリック時の処理コールバック
		typedef void (*CallbackOnClick)();

	protected:
		inline CDebugUIButton();
		virtual ~CDebugUIButton() {}

	private:
		CDebugUIButton(const CDebugUIButton& rhs);
		const CDebugUIButton& operator=(const CDebugUIButton& rhs);

	protected:
		// 描画
		void Render(
			CGraphicsDevice* pDevice,
			UN_INT16 nX, UN_INT16 nY);

		// 範囲内かどうか
		UN_BOOL IsInRect(UN_INT nX, UN_INT nY);

		UN_BOOL OnMouseLButtonDown(UN_INT nX, UN_INT nY);
		UN_BOOL OnMouseLButtonUp(UN_INT nX, UN_INT nY);
		UN_BOOL OnMouseMove(UN_INT nX, UN_INT nY);

	public:
		// ボタンをクリックしたときのコールバックをセットする
		inline void SetClickCallback(CallbackOnClick* pCallback);

		// 文字列をセットする
		inline void SetText(UN_PCSTR pText);

	public:
		// 文字列を取得する
		inline UN_PCSTR GetText() const;

	protected:
		enum STATE {
			STATE_NORMAL = 0,
			STATE_PRESS,
		};

	protected:
		// クリック時コールバック
		CallbackOnClick* m_pClickCallback;

		// 表示文字列
		UN_PCSTR m_pText; 

		// 状態
		STATE m_nState;
	};

	// inline ***************************

	// コンストラクタ
	CDebugUIButton::CDebugUIButton()
	{
		m_pClickCallback = UN_NULL;
		m_pText = UN_NULL;
		m_nState = STATE_NORMAL;
	}

	/**
	* ボタンをクリックしたときのコールバックをセットする
	*/
	void CDebugUIButton::SetClickCallback(CallbackOnClick* pCallback)
	{
		m_pClickCallback = pCallback;
	}

	/**
	* 文字列をセットする
	*/
	void CDebugUIButton::SetText(UN_PCSTR pText)
	{
		m_pText = pText;
	}

	/**
	* 文字列を取得する
	*/
	UN_PCSTR CDebugUIButton::GetText() const
	{
		return m_pText;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_BUTTON_H__)
