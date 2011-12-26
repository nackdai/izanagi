#if !defined(__IZANAGI_DEBUGUTIL_DEBUG_UI_BUTTON_H__)
#define __IZANAGI_DEBUGUTIL_DEBUG_UI_BUTTON_H__

#include "DebugUIWidget.h"

namespace izanagi {
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
			IZ_INT16 nX, IZ_INT16 nY);

		// 範囲内かどうか
		IZ_BOOL IsInRect(IZ_INT nX, IZ_INT nY);

		IZ_BOOL OnMouseLButtonDown(IZ_INT nX, IZ_INT nY);
		IZ_BOOL OnMouseLButtonUp(IZ_INT nX, IZ_INT nY);
		IZ_BOOL OnMouseMove(IZ_INT nX, IZ_INT nY);

	public:
		// ボタンをクリックしたときのコールバックをセットする
		inline void SetClickCallback(CallbackOnClick* pCallback);

		// 文字列をセットする
		inline void SetText(IZ_PCSTR pText);

	public:
		// 文字列を取得する
		inline IZ_PCSTR GetText() const;

	protected:
		enum STATE {
			STATE_NORMAL = 0,
			STATE_PRESS,
		};

	protected:
		// クリック時コールバック
		CallbackOnClick* m_pClickCallback;

		// 表示文字列
		IZ_PCSTR m_pText; 

		// 状態
		STATE m_nState;
	};

	// inline ***************************

	// コンストラクタ
	CDebugUIButton::CDebugUIButton()
	{
		m_pClickCallback = IZ_NULL;
		m_pText = IZ_NULL;
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
	void CDebugUIButton::SetText(IZ_PCSTR pText)
	{
		m_pText = pText;
	}

	/**
	* 文字列を取得する
	*/
	IZ_PCSTR CDebugUIButton::GetText() const
	{
		return m_pText;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUGUTIL_DEBUG_UI_BUTTON_H__)
