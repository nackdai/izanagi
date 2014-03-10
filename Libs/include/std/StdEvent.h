#if !defined(__IZANAGI_STD_STD_EVENT_H__)
#define __IZANAGI_STD_STD_EVENT_H__

#include "izDefs.h"
#include "StdValue.h"
#include "std/collection/StdList.h"
#include "std/allocator/MemoryAllocator.h"
#include "std/allocator/STLMemoryAllocator.h"

namespace izanagi {
    /**
     */
    class Delegate : public CPlacementNew
    {
        friend class CEventHandlerHelper;

    public:
        static void Delete(Delegate* deletgate)
        {
            SAFE_DELETE(deletgate);
        }

    protected:
        Delegate(void* object, void* func) : m_Object(object)
        {
            m_Allocator = IZ_NULL;
            m_ListItem.Init(this);
        }
        Delegate(const Delegate& rhs)
        {
            m_Object = rhs.m_Object;
            m_Allocator = rhs.m_Allocator;
            m_ListItem.Init(this);
        }
        virtual ~Delegate()
        {
            FREE(m_Allocator, this);
        }

    protected:
        void* GetObject()
        {
            return m_Object;
        }

    public:
        CStdList<Delegate>::Item* GetListItem()
        {
            return &m_ListItem;
        }

    protected:
        void* m_Object;

        IMemoryAllocator* m_Allocator;
        CStdList<Delegate>::Item m_ListItem;
    };

    /** �������P�̃f���Q�[�g
     *
     * @tparam RETURN �Ԃ�l�̌^
     * @tparam ARG �����̌^
     */
    template <typename RETURN, typename ARG>
    class DelegateArg1 : public Delegate
    {
    protected:
        DelegateArg1(void* object, void* func) : Delegate(object, func) {}
        DelegateArg1(const DelegateArg1& rhs) : Delegate(rhs) {}
        virtual ~DelegateArg1() {}

    public:
        RETURN operator()(ARG arg1)
        {
            CValue val = Execute(arg1);
            RETURN ret = val.GetValue<RETURN>();
            return ret;
        }

    protected:
        virtual CValue Execute(ARG arg1) = 0;
    };

    /** �Ԃ�l�������A���������P�̃f���Q�[�g
     *
     * @tparam ARG �����̌^
     */
    template <typename ARG>
    class DelegateArg1<void, ARG> : public Delegate
    {
    protected:
        DelegateArg1(void* object, void* func) : Delegate(object, func) {}
        DelegateArg1(const DelegateArg1& rhs) : Delegate(rhs) {}
        virtual ~DelegateArg1() {}

    public:
        void operator()(ARG arg1)
        {
            Execute(arg1);
        }

    protected:
        virtual void Execute(ARG arg1) = 0;
    };

    /** �Ԃ�l�������A�������������f���Q�[�g
     *
     * @tparam ARG �����̌^
     */
    template <>
    class DelegateArg1<void, void> : public Delegate
    {
    protected:
        DelegateArg1(void* object, void* func) : Delegate(object, func) {}
        DelegateArg1(const DelegateArg1& rhs) : Delegate(rhs) {}
        virtual ~DelegateArg1() {}

    public:
        void operator()()
        {
            Execute();
        }

    protected:
        virtual void Execute() = 0;
    };

    /** C#��Action�ɑ���
     *
     * @tparam ARG �����̌^
     */
    template <typename ARG>
    class ActionDelegate : public DelegateArg1<void, ARG>
    {
    protected:
        ActionDelegate(void* object, void* func) : DelegateArg1<void, ARG>(object, func) {}
        ActionDelegate(const ActionDelegate& rhs) : DelegateArg1<void, ARG>(rhs) {}
        virtual ~ActionDelegate() {}
    };

    /** C#��Action�ɑ���
     * �����ł͎w�肳�ꂽ�I�u�W�F�N�g�̃��\�b�h���Ăяo��
     *
     * @tparam O �I�u�W�F�N�g�̌^
     * @tparam T �Ăяo�����\�b�h����`���ꂽ�N���X�̌^
     * @tparam ARG �����̌^
     */
    template <typename O, typename T, typename ARG>
    class ActionDelegateInstance : public ActionDelegate<ARG>
    {
        typedef void (T::* Func)(ARG);

        friend class CDelegateFactory;

        IZ_DECL_PLACEMENT_NEW();

    protected:
        ActionDelegateInstance(O* object, Func func) : ActionDelegate<ARG>(object, IZ_NULL), m_Func(func) {}
        ActionDelegateInstance(const ActionDelegateInstance& rhs) : ActionDelegate<ARG>(rhs), m_Func(rhs.m_Func) {}
        virtual ~ActionDelegateInstance() {}

    protected:
        virtual void Execute(ARG arg1)
        {
            O* object = (O*)Delegate::GetObject();
            IZ_ASSERT(object != IZ_NULL);

            IZ_ASSERT(m_Func != IZ_NULL);
            
            (object->*m_Func)(arg1);
        }

    private:
        Func m_Func;
    };

    /** C#��Action�ɑ���
     * �����ł�static�Ȋ֐����Ăяo��
     *
     * @tparam ARG �����̌^ 
     */
    template <typename ARG>
    class ActionDelegateStatic : public ActionDelegate<ARG>
    {
        typedef void (*Func)(ARG);
        
        friend class CDelegateFactory;

        IZ_DECL_PLACEMENT_NEW();

    protected:
        ActionDelegateStatic(Func func) : ActionDelegate<ARG>(IZ_NULL, IZ_NULL), m_Func(func) {}
        ActionDelegateStatic(const ActionDelegateStatic& rhs) : ActionDelegate<ARG>(rhs), m_Func(rhs.m_Func) {}
        virtual ~ActionDelegateStatic() {}

    protected:
        virtual void Execute(ARG arg1)
        {           
            IZ_ASSERT(m_Func != IZ_NULL);

            (*m_Func)(arg1);
        }

    private:
        Func m_Func;
    };

    /** C#��Func�ɑ���
     *
     * @tparam RETURN �Ԃ�l�̌^
     * @tparam ARG �����̌^
     */
    template <typename RETURN, typename ARG>
    class FuncDelegate : public DelegateArg1<RETURN, ARG>
    {
    protected:
        FuncDelegate(void* object, void* func) : DelegateArg1<RETURN, ARG>(object, func) {}
        FuncDelegate(const FuncDelegate& rhs) : DelegateArg1<RETURN, ARG>(rhs) {}
        virtual ~FuncDelegate() {}
    };

    /** C#��Func�ɑ���
     * �����ł͎w�肳�ꂽ�I�u�W�F�N�g�̃��\�b�h���Ăяo��
     *
     * @tparam O �I�u�W�F�N�g�̌^
     * @tparam T �Ăяo�����\�b�h����`���ꂽ�N���X�̌^
     * @tparam RETURN �Ԃ�l�̌^
     * @tparam ARG �����̌^
     */
    template <typename O, typename T, typename RETURN, typename ARG>
    class FuncDelegateInstance : public FuncDelegate<RETURN, ARG>
    {
        typedef RETURN (T::* FUNC)(ARG);

        friend class CDelegateFactory;

        IZ_DECL_PLACEMENT_NEW();

    protected:
        FuncDelegateInstance(O* object, FUNC func) : FuncDelegate<RETURN, ARG>(object, IZ_NULL), m_Func(func) {}
        FuncDelegateInstance(const FuncDelegateInstance& rhs) : FuncDelegate<RETURN, ARG>(rhs), m_Func(rhs.m_Func) {}
        virtual ~FuncDelegateInstance() {}

    protected:
        virtual CValue Execute(ARG arg1)
        {
            O* object = (O*)Delegate::GetObject();
            IZ_ASSERT(object != IZ_NULL);

            IZ_ASSERT(m_Func != IZ_NULL);

            RETURN tmp = (object->*m_Func)(arg1);
            return CValue(tmp); 
        }

    private:
        FUNC m_Func;
    };

    /** C#��Func�ɑ���
     * �����ł�static�Ȋ֐����Ăяo��
     *
     * @tparam RETURN �Ԃ�l�̌^
     * @tparam ARG �����̌^ 
     */
    template <typename RETURN, typename ARG>
    class FuncDelegateStatic : public FuncDelegate<RETURN, ARG>
    {
        typedef RETURN (*FUNC)(ARG);

        friend class CDelegateFactory;

        IZ_DECL_PLACEMENT_NEW();

    protected:
        FuncDelegateStatic(FUNC func) : FuncDelegate<RETURN, ARG>(IZ_NULL, IZ_NULL), m_Func(func) {}
        FuncDelegateStatic(const FuncDelegateStatic& rhs) : FuncDelegate<RETURN, ARG>(rhs), m_Func(rhs.m_Func) {}
        virtual ~FuncDelegateStatic() {}

    protected:
        virtual CValue Execute(ARG arg1)
        {           
            IZ_ASSERT(m_Func != IZ_NULL);

            RETURN tmp = (*m_Func)(arg1);
            return CValue(tmp);
        }

    private:
        FUNC m_Func;
    };

    /** �f���Q�[�g�t�@�N�g��
     */
    class CDelegateFactory
    {
    public:
        /** �f���Q�[�g�쐬
         *
         * ActionDelegateInstance���쐬����
         *
         * @tparam O �I�u�W�F�N�g�̌^
         * @tparam T �Ăяo�����\�b�h����`���ꂽ�N���X�̌^
         * @tparam ARG �����̌^
         * @tparam DELEGATE �쐬����f���Q�[�g�̌^
         */
        template <typename O, typename T, typename ARG, typename DELEGATE>
        static DELEGATE* Create(IMemoryAllocator* allocator, O* object, void (T::* func)(ARG))
        {
            void* buf = ALLOC(allocator, sizeof(DELEGATE));
            DELEGATE* ret = new(buf) DELEGATE(object, func);
            ret->m_Allocator = allocator;
            return ret;
        }

        /** �f���Q�[�g�쐬
         *
         * ActionDelegateStatic���쐬����
         *
         * @tparam ARG �����̌^
         * @tparam DELEGATE �쐬����f���Q�[�g�̌^
         */
        template <typename ARG, typename DELEGATE>
        static DELEGATE* Create(IMemoryAllocator* allocator, void (* func)(ARG))
        {
            void* buf = ALLOC(allocator, sizeof(DELEGATE));
            DELEGATE* ret = new(buf) DELEGATE(func);
            ret->m_Allocator = allocator;
            return ret;
        }

        /** �f���Q�[�g�쐬
         *
         * FuncDelegateInstance���쐬����
         *
         * @tparam O �I�u�W�F�N�g�̌^
         * @tparam T �Ăяo�����\�b�h����`���ꂽ�N���X�̌^
         * @tparam RETURN �Ԃ�l�̌^
         * @tparam ARG �����̌^
         * @tparam DELEGATE �쐬����f���Q�[�g�̌^
         */
        template <typename O, typename T, typename RETURN, typename ARG, typename DELEGATE>
        static DELEGATE* Create(IMemoryAllocator* allocator, O* object, RETURN (T::* func)(ARG))
        {
            void* buf = ALLOC(allocator, sizeof(DELEGATE));
            DELEGATE* ret = new(buf) DELEGATE(object, func);
            ret->m_Allocator = allocator;
            return ret;
        }

         /** �f���Q�[�g�쐬
         *
         * FuncDelegateStatic���쐬����
         *
         * @tparam RETURN �Ԃ�l�̌^
         * @tparam ARG �����̌^
         * @tparam DELEGATE �쐬����f���Q�[�g�̌^
         */
        template <typename RETURN, typename ARG, typename DELEGATE>
        static DELEGATE* Create(IMemoryAllocator* allocator, RETURN (* func)(ARG))
        {
            void* buf = ALLOC(allocator, sizeof(DELEGATE));
            DELEGATE* ret = new(buf) DELEGATE(func);
            ret->m_Allocator = allocator;
            return ret;
        }
    };

    /** �C�x���g�n���h���쐬
     *
     * ActionDelegateInstance���쐬����
     *
     * @tparam O �I�u�W�F�N�g�̌^
     * @tparam T �Ăяo�����\�b�h����`���ꂽ�N���X�̌^
     * @tparam ARG �����̌^
     */
    template <typename O, typename T, typename ARG>
    ActionDelegateInstance<O, T, ARG>& EventHandler(IMemoryAllocator* allocator, O* object, void (T::*func)(ARG))
    {
        return *CDelegateFactory::Create<O, T, ARG, ActionDelegateInstance<O, T, ARG> >(allocator, object, func);
    }

    /** �C�x���g�n���h���쐬
     *
     * ActionDelegateStatic���쐬����
     *
     * @tparam ARG �����̌^
     */
    template <typename ARG>
    ActionDelegateStatic<ARG>& EventHandler(IMemoryAllocator* allocator, void(*func)(ARG))
    {
        return *CDelegateFactory::Create<ARG, ActionDelegateStatic<ARG> >(allocator, func);
    }

    /** �C�x���g�n���h���쐬
     *
     * FuncDelegateInstance���쐬����
     *
     * @tparam O �I�u�W�F�N�g�̌^
     * @tparam T �Ăяo�����\�b�h����`���ꂽ�N���X�̌^
     * @tparam RETURN �Ԃ�l�̌^
     * @tparam ARG �����̌^
     */
    template <typename O, typename T, typename RETURN, typename ARG>
    FuncDelegateInstance<O, T, RETURN, ARG>& EventHandler(IMemoryAllocator* allocator, O* object, RETURN (T::*func)(ARG))
    {
        return *CDelegateFactory::Create<O, T, RETURN, ARG, FuncDelegateInstance<O, T, RETURN, ARG> >(
            allocator, object, func);
    }

    /** �C�x���g�n���h���쐬
     *
     * FuncDelegateStatic���쐬����
     *
     * @tparam RETURN �Ԃ�l�̌^
     * @tparam ARG �����̌^
     */
    template <typename RETURN, typename ARG>
    FuncDelegateStatic<RETURN, ARG>& EventHandler(IMemoryAllocator* allocator, RETURN (*func)(ARG))
    {
        return *CDelegateFactory::Create<RETURN, ARG, FuncDelegateStatic<RETURN, ARG> >(allocator, func);
    }

    /**
     */
    template <typename RETURN, typename ARG>
    class CStdEventBase
    {
    protected:
        typedef DelegateArg1<RETURN, ARG> DELEGATE;

    public:
        virtual ~CStdEventBase() {}

        /** �f���Q�[�g��ǉ�
         */
        void operator+=(DELEGATE& delegate)
        {
            if (!Find(&delegate))
            {
                Add(&delegate);
            }
        }

        /** �w�肳�ꂽ�f���Q�[�g���C�x���g����폜
         */
        void operator-=(DELEGATE& delegate)
        {
            if (Find(&delegate))
            {
                Remove(&delegate);
            }
        }

        /** �o�^����Ă���f���Q�[�g�����擾
         */
        virtual IZ_UINT GetCount() = 0;

        /** �w�肳�ꂽ�f���Q�[�g���擾
         */
        virtual DELEGATE* Get(IZ_UINT idx) = 0;

    protected:
        // �w�肳�ꂽ�f���Q�[�g��T��
        virtual IZ_BOOL Find(Delegate* delegate) = 0;

        // �f���Q�[�g��ǉ�
        virtual IZ_BOOL Add(Delegate* delegate) = 0;

        // �w�肳�ꂽ�f���Q�[�g���C�x���g����폜
        virtual IZ_BOOL Remove(Delegate* delegate) = 0;
    };

    /**
     */
    template <typename RETURN, typename ARG>
    class CStdEventBaseProxy : public CStdEventBase<RETURN, ARG>
    {
    public:
        CStdEventBaseProxy()
        {
            m_DeleteFunc = IZ_NULL;
        }

        virtual ~CStdEventBaseProxy()
        {
            CStdList<Delegate>::Item* item = m_List.GetTop();
            while (item != IZ_NULL)
            {
                CStdList<Delegate>::Item* next = item->GetNext();
                Delegate* data = item->GetData();
                item->Leave();

                if (m_DeleteFunc != IZ_NULL)
                {
                    (*m_DeleteFunc)(data);
                }
                else
                {
                    Delegate::Delete(data);
                }

                item = next;
            }
        }

        void SetDeleteFunc(void (*deleteFunc)(Delegate*))
        {
            m_DeleteFunc = deleteFunc;
        }

        virtual IZ_UINT GetCount()
        {
            return m_List.GetItemNum();
        }

        virtual typename CStdEventBase<RETURN, ARG>::DELEGATE* Get(IZ_UINT idx)
        {
            CStdList<Delegate>::Item* item = m_List.GetAt(idx);
            IZ_ASSERT(item != IZ_NULL);
            return (typename CStdEventBase<RETURN, ARG>::DELEGATE*)item->GetData();
        }

    private:
        virtual IZ_BOOL Find(Delegate* delegate)
        {
            IZ_INT idx = m_List.Find(delegate);
            return (idx >= 0);
        }

        virtual IZ_BOOL Add(Delegate* delegate)
        {
            return m_List.AddTail(delegate->GetListItem());
        }

        virtual IZ_BOOL Remove(Delegate* delegate)
        {
            delegate->GetListItem()->Leave();
            return IZ_TRUE;
        }

    private:
        CStdList<Delegate> m_List;
        void (*m_DeleteFunc)(Delegate*);
    };

    /**
     */
    template <typename RETURN, typename ARG>
    class CStdEvent : public CStdEventBaseProxy<RETURN, ARG>
    {
    public:
        virtual ~CStdEvent() {}

        RETURN operator()(ARG arg1)
        {
            IZ_UINT count = CStdEventBaseProxy<RETURN, ARG>::GetCount();
            for (IZ_UINT i = 0; i < count; i++)
            {
                typename CStdEventBase<RETURN, ARG>::DELEGATE* delegate = CStdEventBaseProxy<RETURN, ARG>::Get(i);
                RETURN ret = (*delegate)(arg1);
                if (i == count - 1)
                {
                    return ret;
                }
            }

            return RETURN(0);
        }
    };

    /**
     */
    template <typename ARG>
    class CStdEvent<void, ARG> : public CStdEventBaseProxy<void, ARG>
    {
    public:
        virtual ~CStdEvent() {}

        void operator()(ARG arg1)
        {
            IZ_UINT count = CStdEventBaseProxy<void, ARG>::GetCount();
            for (IZ_UINT i = 0; i < count; i++)
            {
                typename CStdEventBase<void, ARG>::DELEGATE* delegate = CStdEventBaseProxy<void, ARG>::Get(i);
                (*delegate)(arg1);
            }
        }
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STD_EVENT_H__)
