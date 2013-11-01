#include <windows.h>

#include "izStd.h"

class SampleObject : izanagi::DependencyObject<>
{
public:
    static izanagi::DependencyProperty sProp;

public:
    SampleObject::SampleObject()
    {
        SetProperty(0);
    }
    SampleObject::~SampleObject()
    {
        // NOTE
        // 実装側で呼び出す必要がある
        ClearAll();
    }

    void SetProperty(IZ_UINT val)
    {
        SetValue(sProp, val);
    }

    IZ_UINT GetProperty()
    {
        izanagi::CValue value;
        GetValue(sProp, value);
        return value.GetValue<IZ_UINT>();
    }

    virtual void OnPropertyChanged(const izanagi::DependencyPropertyChangedEventArgs& e)
    {
        IZ_PRINTF(
            "value [%d] -> [%d]\n",
            e.GetOldValue().GetValueAsUInt32(),
            e.GetNewValue().GetValueAsUInt32());
    }

protected:
    virtual void* AllocForDependencyObject(size_t size)
    {
        return malloc(size);
    }

    virtual void FreeForDependencyObject(void* p)
    {
        free(p);
    }
};

class BindingSource
{
public:
    BindingSource()
    {
    }
    ~BindingSource()
    {
    }

    IZ_UINT Value;
};

class BindingSample : public izanagi::Binding
{
public:
    BindingSample()
    {
    }
    virtual ~BindingSample()
    {
    }

public:
    // データのモトになるオブジェクトを設定
    void SetSource(BindingSource* source)
    {
        m_Source = source;
    }

    virtual void SetValue(const izanagi::CValue& value)
    {
        m_Source->Value = value.GetValueAsUInt32();
    }

    virtual void GetValue(izanagi::CValue& value)
    {
        value.SetValue(m_Source->Value);
    }

private:
    BindingSource* m_Source;
};

izanagi::DependencyProperty SampleObject::sProp("Value");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    izanagi::CSimpleMemoryAllocator allocator;

    SampleObject object;

    IZ_PRINTF("Simple DependencyProperty ****");
    {
        object.SetProperty(100);
        IZ_UINT value = object.GetProperty();
        IZ_PRINTF("%d\n", value);
    }
    
    IZ_PRINTF("Binding ****");
    {
        BindingSample* binding = izanagi::Binding::CreateBinding<BindingSample>(&allocator, "Value");

        BindingSource source;
        binding->SetSource(&source);
    
        izanagi::BindingOperations::SetBindings(
            &allocator,
            &object,
            SampleObject::sProp,
            binding);

        SAFE_RELEASE(binding);

        source.Value = 200;
        izanagi::BindingOperations::GetBindingExpression(SampleObject::sProp)->Update();
        IZ_UINT value = object.GetProperty();
        IZ_PRINTF("%d\n", value);
    }

    return 0;
}
