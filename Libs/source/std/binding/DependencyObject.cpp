#include "std/binding/DependencyObject.h"

namespace izanagi
{
    DependencyObjectBase::DependencyObjectBase()
    {
        m_Flags.isCalledClearAll = IZ_FALSE;
        m_Flags.isValueChanged = IZ_FALSE;
    }

    // プロパティ情報をすべてクリア
    void DependencyObjectBase::ClearAll()
    {
        // NOTE
        // DependencyObjectBase のデストラクタで呼び出すべきだが
        // FreeForDependencyObject が仮想関数なのでデストラクタなので呼び出せない
        // そこで m_Flags.isCalledClearAll で呼び出されたかどうかをチェックする

        if(!m_Flags.isCalledClearAll)
        {
            CStdList<HashItem>::Item* item = m_Dictionary.GetOrderTop();
            while (item != IZ_NULL)
            {
                CStdList<HashItem>::Item* next = item->GetNext();
                Element* element = item->GetData()->GetData();
                delete element;
                FreeForDependencyObject(element);
                item = next;
            }

            m_Flags.isCalledClearAll = IZ_TRUE;
        }
    }   

    DependencyObjectBase::HashItem* DependencyObjectBase::Find(const DependencyProperty& prop)
    {
        HashItem* item = m_Dictionary.Find(const_cast<DependencyProperty*>(&prop));
        return item;
    }

    // 依存関係プロパティに値をセット
    void DependencyObjectBase::SetValueInternal(const DependencyProperty& prop, const CValue& value)
    {
        // プロパティに該当するハッシュアイテムを探す
        HashItem* item = Find(prop);

        m_Flags.isValueChanged = IZ_FALSE;

        // 元の値を覚えておく
        CValue oldVal;
        
        if (item != IZ_NULL)
        {
            // 登録済み

            oldVal = item->GetData()->value;

            m_Flags.isValueChanged = (item->GetData()->value != value);
            if (m_Flags.isValueChanged)
            {
                item->GetData()->value = value;
            }
        }
        else
        {
            // 初登録

            //m_Flags.isValueChanged = IZ_TRUE;

            // 登録用の要素を作成
            void* p = AllocForDependencyObject(sizeof(Element));
            IZ_ASSERT(p != IZ_NULL);

            Element* element = new(p) Element;
            {
                element->value = value;
                element->hashItem.Init(
                    const_cast<DependencyProperty*>(&prop),
                    element);
            }

            // 登録
            m_Dictionary.Add(&element->hashItem);
        }

        if (m_Flags.isValueChanged)
        {
            // 値が変更された

            DependencyPropertyChangedEventArgs e(
                oldVal,
                value,
                prop);

            OnPropertyChanged(e);
        }
    }

    // 依存関係プロパティから値を取得
    IZ_BOOL DependencyObjectBase::GetValueInternal(const DependencyProperty& prop, CValue& ret)
    {
        // 
        HashItem* item = Find(prop);

        if (item != IZ_NULL)
        {
            ret = item->GetData()->value;
            return IZ_TRUE;
        }
#if 0
        else if (prop.GetMetaData().IsValidDefaultValue())
        {
            ret = prop.GetMetaData().GetDefaultValue();
            return IZ_TRUE;
        }
#endif

        return IZ_FALSE;
    }
}   // namespace izanagi
