#if !defined(__IZANAGI_STD_BINDING_H__)
#define __IZANAGI_STD_BINDING_H__

#include "izDefs.h"
#include "std/StdObject.h"
#include "std/StdPlacementNew.h"
#include "std/StdString.h"
#include "std/StdValue.h"
#include "std/allocator/MemoryAllocator.h"
#include "DependencyProperty.h"

namespace izanagi {
    /** データ同期モード
     */
    enum E_BINDING_MODE
    {
        E_BINDING_MODE_ONE_WAY = 0,         ///< ソースからターゲットへの一方向
        E_BINDING_MODE_TWO_WAY,             ///< ソースとターゲットの双方向
        E_BINDING_MODE_ONE_TIME,            ///< ソースからターゲットの一方向を一度のみ
        E_BINDING_MODE_ONE_WAY_TO_SOURCE,   ///< ターゲットからソースへの一方向

        E_BINDING_MODE_NUM,
        E_BINDING_MODE_FORCE_INT32 = 0x7fffffff,
    };

    /** バインディング ターゲット オブジェクトのプロパティと任意のデータ ソース とを接続
     */
    class Binding : public CObject
    {
    public:
        /** プロパティ名の最大文字数
         */
        static IZ_UINT PropertyNameMax;

        /** プロパティ名の型
         */
        typedef DependencyProperty::PropertyName PropertyName;

        /** Binding を作成する
         *
         * @param[in] allocator
         * @param[in] name 同期対象となるプロパティ名
         */
        template <typename T>
        static T* CreateBinding(IMemoryAllocator* allocator, IZ_PCSTR name)
        {
            void* buf = ALLOC_ZERO(allocator, sizeof(T));
            VRETURN_NULL(buf != IZ_NULL);

            T* ret = new(buf) T;
            {
                Binding* instance = ret;
                instance->AddRef();
                instance->m_Allocator = allocator;
                instance->SetPropertyName(name);
            }

            return ret;
        }

    protected:
        Binding()
        {
            m_Allocator = IZ_NULL;
            m_Mode = E_BINDING_MODE_ONE_WAY;
        }
        virtual ~Binding() {}

        IZ_DEFINE_INTERNAL_RELEASE();

    public:

        /** 同期モード取得
         */
        E_BINDING_MODE GetMode()
        {
            return m_Mode;
        }

        /** 同期対象のプロパティ名取得
         */
        const PropertyName& GetPropertyName()
        {
            return m_PropName;
        }

        /** 同期対象のプロパティ名のキー値を取得
         */
        IZ_UINT GetPropertyKey()
        {
            return m_Key;
        }

        // NOTE
        // データのモトになるオブジェクトは型の関係もあるので
        // 実装クラス側で行うようにさせる

        /** データのモトになるオブジェクトが持つプロパティ値を取得
         */
        virtual void GetValue(CValue& ret) = 0;

        /** データのモトになるオブジェクトが持つプロパティに値をセット
         */
        virtual void SetValue(const CValue& value) = 0;

    private:
        void SetPropertyName(IZ_PCSTR name)
        {
            m_PropName.SetString(name);
            m_Key = m_PropName.GetKeyValue();
        }

    private:
        IMemoryAllocator* m_Allocator;

        E_BINDING_MODE m_Mode;

        PropertyName m_PropName;
        IZ_UINT m_Key;      
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_BINDING_H__)
