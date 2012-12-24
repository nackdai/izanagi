#if !defined(__IZANAGI_STD_STD_EVENT_H__)
#define __IZANAGI_STD_STD_EVENT_H__

#include "izDefs.h"
#include "StdValue.h"
#include "std/collection/StdList.h"
#include "std/allocator/MemoryAllocator.h"
#include "std/allocator/STLMemoryAllocator.h"

#include <list>
#include <algorithm>

namespace izanagi {
    /**
     */
    class Delegate
    {
        friend class CEventHandlerHelper;

    public:
        Delegate(void* object, void* func) : m_Object(object), m_Func(func) { m_Allocator = IZ_NULL; }
        Delegate(const Delegate& rhs)
        {
            m_Object = rhs.m_Object;
            m_Func = rhs.m_Func;
            m_Allocator = rhs.m_Allocator;
        }
        virtual ~Delegate()
        {
            FREE(m_Allocator, this);
        }

        IZ_BOOL Equals(const Delegate& rhs)
        {
            return (m_Object == rhs.m_Object && m_Func == rhs.m_Func);
        }

    protected:
        void* GetObject() { return m_Object; }
        void* GetFunc() { return m_Func; }

    protected:
        void* m_Object;
        void* m_Func;

        IMemoryAllocator* m_Allocator;
    };

    /** 引数が１つのデリゲート
     *
     * @tparam RETURN 返り値の型
     * @tparam ARG 引数の型
     */
    template <typename RETURN, typename ARG>
    class DelgateArg1 : public Delegate
    {
    public:
        DelgateArg1(void* object, void* func) : Delegate(object, func) {}
        DelgateArg1(const DelgateArg1& rhs) : Delegate(rhs) {}
        virtual ~DelgateArg1() {}

        RETURN operator()(ARG arg1)
        {
            CValue val = Execute(arg1);
            RETURN ret = val.GetValue<RETURN>();
            return ret;
        }

        virtual Delegate Clone() = 0;

    protected:
        virtual CValue Execute(ARG arg1) = 0;
    };

    /** 返り値が無い、かつ引数が１つのデリゲート
     *
     * @tparam ARG 引数の型
     */
    template <typename ARG>
    class DelgateArg1<void, ARG> : public Delegate
    {
    public:
        DelgateArg1(void* object, void* func) : Delegate(object, func) {}
        DelgateArg1(const DelgateArg1& rhs) : Delegate(rhs) {}
        virtual ~DelgateArg1() {}

        void operator()(ARG arg1)
        {
            Execute(arg1);
        }

        virtual Delegate Clone() = 0;

    protected:
        virtual void Execute(ARG arg1) = 0;
    };

    /** C#のActionに相当
     *
     * @tparam ARG 引数の型
     */
    template <typename ARG>
    class ActionDelegate : public DelgateArg1<void, ARG>
    {
    public:
        ActionDelegate(void* object, void* func) : DelgateArg1(object, func) {}
        ActionDelegate(void* func) : DelgateArg1(IZ_NULL, func) {}
        ActionDelegate(const ActionDelegate& rhs) : DelgateArg1(rhs) {}
        virtual ~ActionDelegate() {}
    };

    /** C#のActionに相当
     * 内部では指定されたオブジェクトのメソッドを呼び出す
     *
     * @tparam O オブジェクトの型
     * @tparam T 呼び出すメソッドが定義されたクラスの型
     * @tparam ARG 引数の型
     */
    template <typename O, typename T, typename ARG>
    class ActionDelegateInstance : public ActionDelegate<ARG>
    {
        typedef void (T::* FUNC)(ARG);

        friend class CEventHandlerHelper;
        friend class CDelegateFactory;

        IZ_DECL_PLACEMENT_NEW();

    public:
        ActionDelegateInstance(O* object, FUNC func) : ActionDelegate(object, &func), m_Func(func) {}
        ActionDelegateInstance(const ActionDelegateInstance& rhs) : ActionDelegate(rhs) { m_Func = rhs.m_Func; }
        virtual ~ActionDelegateInstance() {}

        virtual Delegate Clone()
        {
            return ActionDelegateInstance(*this);
        }

    protected:
        virtual void Execute(ARG arg1)
        {
            O* object = (O*)GetObject();
            IZ_ASSERT(object != IZ_NULL);
            
            (object->*m_Func)(arg1);
        }

    private:
        FUNC m_Func;
    };

    /** C#のActionに相当
     * 内部ではstaticな関数を呼び出す
     *
     * @tparam ARG 引数の型 
     */
    template <typename ARG>
    class ActionDelegateStatic : public ActionDelegate<ARG>
    {
        typedef void (*FUNC)(ARG);
        
        friend class CEventHandlerHelper;
        friend class CDelegateFactory;

        IZ_DECL_PLACEMENT_NEW();

    public:
        ActionDelegateStatic(FUNC func) : ActionDelegate(func) {}
        ActionDelegateStatic(const ActionDelegateStatic& rhs) : ActionDelegate(rhs) {}
        virtual ~ActionDelegateStatic() {}

        virtual Delegate Clone()
        {
            return ActionDelegateStatic(*this);
        }

    protected:
        virtual void Execute(ARG arg1)
        {           
            FUNC func = (FUNC)GetFunc();
            IZ_ASSERT(func != IZ_NULL);

            (*func)(arg1);
        }
    };

    /** C#のFuncに相当
     *
     * @tparam RETURN 返り値の型
     * @tparam ARG 引数の型
     */
    template <typename RETURN, typename ARG>
    class FuncDelegate : public DelgateArg1<RETURN, ARG>
    {
    public:
        FuncDelegate(void* object, void* func) : DelgateArg1(object, func) {}
        FuncDelegate(void* func) : DelgateArg1(IZ_NULL, func) {}
        FuncDelegate(const FuncDelegate& rhs) : DelgateArg1(rhs) {}
        virtual ~FuncDelegate() {}
    };

    /** C#のFuncに相当
     * 内部では指定されたオブジェクトのメソッドを呼び出す
     *
     * @tparam O オブジェクトの型
     * @tparam T 呼び出すメソッドが定義されたクラスの型
     * @tparam RETURN 返り値の型
     * @tparam ARG 引数の型
     */
    template <typename O, typename T, typename RETURN, typename ARG>
    class FuncDelegateInstance : public FuncDelegate<RETURN, ARG>
    {
        typedef RETURN (T::* FUNC)(ARG);

        friend class CEventHandlerHelper;
        friend class CDelegateFactory;

        IZ_DECL_PLACEMENT_NEW();

    public:
        FuncDelegateInstance(O* object, FUNC func) : FuncDelegate(object, &func), m_Func(func) {}
        FuncDelegateInstance(const FuncDelegateInstance& rhs) : FuncDelegate(rhs) { m_Func = rhs.m_Func; }
        virtual ~FuncDelegateInstance() {}

        virtual Delegate Clone()
        {
            return FuncDelegateInstance(*this);
        }

    protected:
        virtual CValue Execute(ARG arg1)
        {
            O* object = (O*)GetObject();
            IZ_ASSERT(object != IZ_NULL);

            RETURN tmp = (object->*m_Func)(arg1);
            return CValue(tmp); 
        }

    private:
        FUNC m_Func;
    };

    /** C#のActionに相当
     * 内部ではstaticな関数を呼び出す
     *
     * @tparam RETURN 返り値の型
     * @tparam ARG 引数の型 
     */
    template <typename RETURN, typename ARG>
    class FuncDelegateStatic : public FuncDelegate<RETURN, ARG>
    {
        typedef RETURN (*FUNC)(ARG);

        friend class CEventHandlerHelper;
        friend class CDelegateFactory;

        IZ_DECL_PLACEMENT_NEW();

    public:
        FuncDelegateStatic(FUNC func) : FuncDelegate(func) {}
        FuncDelegateStatic(const FuncDelegateStatic& rhs) : FuncDelegate(rhs) {}
        virtual ~FuncDelegateStatic() {}

        virtual Delegate Clone()
        {
            return FuncDelegateStatic(*this);
        }

    protected:
        virtual CValue Execute(ARG arg1)
        {           
            FUNC func = (FUNC)GetFunc();
            IZ_ASSERT(func != IZ_NULL);

            RETURN tmp = (*func)(arg1);
            return CValue(tmp);
        }
    };

    /** デリゲートファクトリ
     */
    class CDelegateFactory
    {
    public:
        /** デリゲート作成
         *
         * ActionDelegateInstanceを作成する
         *
         * @tparam O オブジェクトの型
         * @tparam T 呼び出すメソッドが定義されたクラスの型
         * @tparam ARG 引数の型
         * @tparam DELEGATE 作成するデリゲートの型
         */
        template <typename O, typename T, typename ARG, typename DELEGATE>
        static DELEGATE* Create(IMemoryAllocator* allocator, O* object, void (T::* func)(ARG))
        {
            void* buf = ALLOC(allocator, sizeof(DELEGATE));
            DELEGATE* ret = new(buf) DELEGATE(object, func);
            ret->m_Allocator = allocator;
            return ret;
        }

        /** デリゲート作成
         *
         * ActionDelegateStaticを作成する
         *
         * @tparam ARG 引数の型
         * @tparam DELEGATE 作成するデリゲートの型
         */
        template <typename ARG, typename DELEGATE>
        static DELEGATE* Create(IMemoryAllocator* allocator, void (* func)(ARG))
        {
            void* buf = ALLOC(allocator, sizeof(DELEGATE));
            DELEGATE* ret = new(buf) DELEGATE(func);
            ret->m_Allocator = allocator;
            return ret;
        }

        /** デリゲート作成
         *
         * FuncDelegateInstanceを作成する
         *
         * @tparam O オブジェクトの型
         * @tparam T 呼び出すメソッドが定義されたクラスの型
         * @tparam RETURN 返り値の型
         * @tparam ARG 引数の型
         * @tparam DELEGATE 作成するデリゲートの型
         */
        template <typename O, typename T, typename RETURN, typename ARG, typename DELEGATE>
        static DELEGATE* Create(IMemoryAllocator* allocator, O* object, RETURN (T::* func)(ARG))
        {
            void* buf = ALLOC(allocator, sizeof(DELEGATE));
            DELEGATE* ret = new(buf) DELEGATE(object, func);
            ret->m_Allocator = allocator;
            return ret;
        }

         /** デリゲート作成
         *
         * FuncDelegateStaticを作成する
         *
         * @tparam RETURN 返り値の型
         * @tparam ARG 引数の型
         * @tparam DELEGATE 作成するデリゲートの型
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

    /** イベントハンドラ作成
     *
     * ActionDelegateInstanceを作成する
     *
     * @tparam O オブジェクトの型
     * @tparam T 呼び出すメソッドが定義されたクラスの型
     * @tparam ARG 引数の型
     */
    template <typename O, typename T, typename ARG>
    ActionDelegateInstance<O, T, ARG>& EventHandler(IMemoryAllocator* allocator, O* object, void (T::*func)(ARG))
    {
        return *CDelegateFactory::Create<O, T, ARG, ActionDelegateInstance<O, T, ARG> >(allocator, object, func);
    }

    /** イベントハンドラ作成
     *
     * ActionDelegateStaticを作成する
     *
     * @tparam ARG 引数の型
     */
    template <typename ARG>
    ActionDelegateStatic<ARG>& EventHandler(IMemoryAllocator* allocator, void(*func)(ARG))
    {
        return *CDelegateFactory::Create<ARG, ActionDelegateStatic<ARG> >(allocator, func);
    }

    /** イベントハンドラ作成
     *
     * FuncDelegateInstanceを作成する
     *
     * @tparam O オブジェクトの型
     * @tparam T 呼び出すメソッドが定義されたクラスの型
     * @tparam RETURN 返り値の型
     * @tparam ARG 引数の型
     */
    template <typename O, typename T, typename RETURN, typename ARG>
    FuncDelegateInstance<O, T, RETURN, ARG>& EventHandler(IMemoryAllocator* allocator, O* object, RETURN (T::*func)(ARG))
    {
        return *CDelegateFactory::Create<O, T, RETURN, ARG, FuncDelegateInstance<O, T, RETURN, ARG> >(
            allocator, object, func);
    }

    /** イベントハンドラ作成
     *
     * FuncDelegateStaticを作成する
     *
     * @tparam RETURN 返り値の型
     * @tparam ARG 引数の型
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
        typedef DelgateArg1<RETURN, ARG> DELEGATE;

    public:
        virtual ~CStdEventBase() {}

        /** デリゲートを追加
         */
        void operator+=(DELEGATE& delegate)
        {
            if (!Find(delegate))
            {
                Add(delegate);
            }
        }

        /** 指定されたデリゲートをイベントから削除
         */
        void operator-=(DELEGATE& delegate)
        {
            if (Find(delegate))
            {
                Remove(delegate);
            }
        }

        /** 登録されているデリゲート数を取得
         */
        virtual IZ_UINT GetCount() = 0;

        /** 指定されたデリゲートを取得
         */
        virtual DELEGATE* Get(IZ_UINT idx) = 0;

    protected:
        // 指定されたデリゲートを探す
        virtual IZ_BOOL Find(DELEGATE& delegate) = 0;

        // デリゲートを追加
        virtual IZ_BOOL Add(DELEGATE& delegate) = 0;

        // 指定されたデリゲートをイベントから削除
        virtual IZ_BOOL Remove(DELEGATE& delegate) = 0;
    };

    /**
     */
    template <typename RETURN, typename ARG>
    class CStdEventBaseProxy : public CStdEventBase<RETURN, ARG>
    {
    public:
        virtual ~CStdEventBaseProxy() {}

        RETURN operator()(ARG arg1)
        {
            IZ_UINT count = GetCount();
            for (IZ_UINT i = 0; i < count; i++)
            {
                DELEGATE* delegate = Get(i);
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
    class CStdEventBaseProxy<void, ARG> : public CStdEventBase<void, ARG>
    {
    public:
        virtual ~CStdEventBaseProxy() {}

        void operator()(ARG arg1)
        {
            IZ_UINT count = GetCount();
            for (IZ_UINT i = 0; i < count; i++)
            {
                DELEGATE* delegate = Get(i);
                (*delegate)(arg1);
            }
        }
    };

    /**
     */
    template <typename RETURN, typename ARG>
    class CStdEvent : public CStdEventBaseProxy<RETURN, ARG>
    {
    public:
        virtual ~CStdEvent()
        {
            CStdList<DELEGATE>::Item* item = m_List.GetTop();
            while (item != IZ_NULL)
            {
                CStdList<DELEGATE>::Item* next = item->GetNext();
                DELEGATE* data = item->GetData();
                m_List.Remove((CStdListEx<DELEGATE>::Item*)item);
                delete data;
                item = next;
            }
        }

        void Init(IMemoryAllocator* allocator)
        {
            m_List.Init(allocator);
        }

        virtual IZ_UINT GetCount()
        {
            return m_List.GetItemNum();
        }

        virtual DELEGATE* Get(IZ_UINT idx)
        {
            CStdList<DELEGATE>::Item* item = m_List.GetAt(idx);
            IZ_ASSERT(item != IZ_NULL);
            return item->GetData();
        }

    private:
        virtual IZ_BOOL Find(DELEGATE& delegate)
        {
            IZ_INT idx = m_List.Find(&delegate);
            return (idx >= 0);
        }

        virtual IZ_BOOL Add(DELEGATE& delegate)
        {
            return m_List.AddTail(&delegate);
        }

        virtual IZ_BOOL Remove(DELEGATE& delegate)
        {
            return m_List.Remove(&delegate);
        }

    private:
        CStdListEx<DELEGATE> m_List;
    };

    /**
     */
    template <typename RETURN, typename ARG>
    class CStdEventSTL : public CStdEventBaseProxy<RETURN, ARG>
    {
    public:
        virtual ~CStdEventSTL()
        {
            IZ_UINT count = GetCount();
            for (IZ_UINT i = 0; i < count; i++)
            {
                DELEGATE* delegate = Get(i);
                delete delegate;
            }
        }

        virtual IZ_UINT GetCount()
        {
            return m_List.size();
        }

        virtual DELEGATE* Get(IZ_UINT idx)
        {
            return &m_List[idx];
        }

    private:
        template <typename T>
        class FindFunc
        {
        public:
            FindFunc(const T* obj) : m_Obj(obj) {}

            bool operator()(const T& obj)
            {
                return ((*m_Obj) == obj);
            }

        private:
            const T* m_Obj;
        };

        virtual IZ_BOOL Find(DELEGATE& delegate)
        {
            return (std::find_if(m_List.begin(), m_List.end(), FindFunc(&delegate)) != m_List.end());
        }

        virtual IZ_BOOL Add(DELEGATE& delegate)
        {
            m_List.push_back(delegate);
            return IZ_TRUE;
        }

        virtual IZ_BOOL Remove(DELEGATE& delegate)
        {
            std::list<DELEGATE, izanagi::STLMemoryAllocator<DELEGATE> >::iterator it = std::find_if(
                m_List.begin(),
                m_List.end(),
                FindFunc(&delegate));

            if (it != m_List.end())
            {
                m_List.erase(it);
            }
        }

    private:
        std::list<DELEGATE, izanagi::STLMemoryAllocator<DELEGATE> > m_List;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STD_EVENT_H__)
