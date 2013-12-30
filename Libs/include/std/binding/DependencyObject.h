#if !defined(__IZANAGI_STD_DPENDENCY_OBJECT_H__)
#define __IZANAGI_STD_DPENDENCY_OBJECT_H__

#include "izDefs.h"
#include "DependencyPropertyChangedEventArgs.h"
#include "DependencyProperty.h"
#include "std/collection/StdHash.h"

namespace izanagi {

    /** 空のクラス
     */
    class NullClass {};

    /** 依存関係プロパティを持つオブジェクトのベースクラス
     */
    class DependencyObjectBase
    {
        friend class BindingExpression;

        static const IZ_UINT HASH_NUM = 8;

        // プロパティの情報を覚えておく用
        class Element
        {
        public:
            // プロパティ名
            DependencyProperty::PropertyName name;

            // プロパティ値
            CValue value;

            CStdHash<IZ_UINT, Element, HASH_NUM>::Item hashItem;

            IZ_DECL_PLACEMENT_NEW();
        };

    public:
        DependencyObjectBase();

        virtual ~DependencyObjectBase()
        {
            // NOTE
            // DependencyObjectBase のデストラクタで ClearAll を呼び出すべきだが
            // ClearAll 内で呼び出される FreeForDependencyObject が仮想関数なのでデストラクタなので呼び出せない
            // そこで m_Flags.isCalledClearAll で呼び出されたかどうかをチェックする

            // TODO
            IZ_ASSERT(m_Flags.isCalledClearAll);
        }

        /** 依存関係プロパティに値をセット
         */
        void SetValue(const DependencyProperty& prop, const CValue& value)
        {
            SetValueInternal(prop, value);
        }

        /** 依存関係プロパティに値をセット
         */
        template <typename V>
        void SetValue(const DependencyProperty& prop, V value)
        {
            IZ_ASSERT(IZ_FALSE);
        }

        /** 依存関係プロパティから値を取得
         */
        IZ_BOOL GetValue(const DependencyProperty& prop, CValue& value)
        {
            return GetValueInternal(prop, value);
        }

    protected:
        /** 内部処理用のメモリを確保する
         */
        virtual void* AllocForDependencyObject(size_t size) = 0;

        /** 内部処理用のメモリを解放する
         */
        virtual void FreeForDependencyObject(void* p) = 0;

        virtual void OnPropertyChanged(const DependencyPropertyChangedEventArgs& e) = 0;

        // プロパティ情報をすべてクリア
        void ClearAll();

    private:
        // プロパティ情報を取得
        CStdHash<IZ_UINT, Element, HASH_NUM>::Item* Find(const DependencyProperty& prop);

        // 依存関係プロパティに値をセット
        void SetValueInternal(const DependencyProperty& prop, const CValue& value);

        // 依存関係プロパティから値を取得
        IZ_BOOL GetValueInternal(const DependencyProperty& prop, CValue& ret);

        IZ_BOOL IsValueChagned() const { return m_Flags.isValueChanged; }

        void ClearValueChagned() { m_Flags.isValueChanged = IZ_FALSE; }

    private:
        CStdHash<IZ_UINT, Element, HASH_NUM> m_Dictionary;

        struct
        {
            IZ_UINT32 isCalledClearAll  : 1;    // ClearAllが呼ばれたかどうか
            IZ_UINT32 isValueChanged    : 1;    // 値の変化が起きたかどうか
        } m_Flags;
    };

    /** 依存関係プロパティに値をセット
     */
    template <>
    inline void DependencyObjectBase::SetValue(const DependencyProperty& prop, IZ_INT8 value)
    {
        SetValueInternal(prop, CValue(value));
    }

    /** 依存関係プロパティに値をセット
     */
    template <>
    inline void DependencyObjectBase::SetValue(const DependencyProperty& prop, IZ_UINT8 value)
    {
        SetValueInternal(prop, CValue(value));
    }

    /** 依存関係プロパティに値をセット
     */
    template <>
    inline void DependencyObjectBase::SetValue(const DependencyProperty& prop, IZ_INT16 value)
    {
        SetValueInternal(prop, CValue(value));
    }

    /** 依存関係プロパティに値をセット
     */
    template <>
    inline void DependencyObjectBase::SetValue(const DependencyProperty& prop, IZ_UINT16 value)
    {
        SetValueInternal(prop, CValue(value));
    }

    /** 依存関係プロパティに値をセット
     */
    template <>
    inline void DependencyObjectBase::SetValue(const DependencyProperty& prop, IZ_INT32 value)
    {
        SetValueInternal(prop, CValue(value));
    }

    /** 依存関係プロパティに値をセット
     */
    template <>
    inline void DependencyObjectBase::SetValue(const DependencyProperty& prop, IZ_UINT32 value)
    {
        SetValueInternal(prop, CValue(value));
    }

    /** 依存関係プロパティに値をセット
     */
    template <>
    inline void DependencyObjectBase::SetValue(const DependencyProperty& prop, IZ_FLOAT value)
    {
        SetValueInternal(prop, CValue(value));
    }

    /** 依存関係プロパティに値をセット
     */
    template <>
    inline void DependencyObjectBase::SetValue(const DependencyProperty& prop, void* value)
    {
        SetValueInternal(prop, CValue(value));
    }

    /** 依存関係プロパティを持つオブジェクト
     */
    template <typename T = NullClass>
    class DependencyObject : public DependencyObjectBase, T
    {
    public:
        DependencyObject() {}
        virtual ~DependencyObject() {}
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_DPENDENCY_OBJECT_H__)
