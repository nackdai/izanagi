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
    /**
     */
    enum E_BINDING_MODE
    {
        E_BINDING_MODE_ONE_WAY = 0,
        E_BINDING_MODE_TWO_WAY,
        E_BINDING_MODE_ONE_TIME,
        E_BINDING_MODE_ONE_WAY_TO_SOURCE,

        E_BINDING_MODE_NUM,
        E_BINDING_MODE_FORCE_INT32 = 0x7fffffff,
    };

    /** データバインディング情報
     */
    class Binding : public CObject
    {
    public:
        /**
         */
        static IZ_UINT PropertyNameMax;

        /**
         */
        typedef DependencyProperty::PropertyName PropertyName;

        /**
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

        /**
         */
        E_BINDING_MODE GetMode()
        {
            return m_Mode;
        }

        /**
         */
        const PropertyName& GetPropertyName()
        {
            return m_PropName;
        }

        /**
         */
        IZ_UINT GetPropertyKey()
        {
            return m_Key;
        }

        /**
         */
        virtual void GetValue(CValue& ret) = 0;

        /**
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
