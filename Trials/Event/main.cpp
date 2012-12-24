#include <windows.h>

#include "izStd.h"

static void OnEvent(IZ_UINT x)
{
    IZ_PRINTF("static OnEvent [%d]\n", x);
}

static IZ_BOOL OnEventEx(IZ_UINT x)
{
    IZ_PRINTF("static OnEventEx [%d]\n", x);
    return IZ_TRUE;
}

class Test
{
public:
    void OnEvent(IZ_UINT x)
    {
        IZ_PRINTF("Test::OnEvent [%d]\n", x);
    }

    IZ_BOOL OnEventEx(IZ_UINT x)
    {
        IZ_PRINTF("Test::OnEventEx [%d]\n", x);
        return IZ_TRUE;
    }
};

class DPTest : izanagi::DependencyObject<>
{
public:
    static izanagi::DependencyProperty sProp;

public:
    DPTest::DPTest()
    {
        SetTestVal(0);
    }
    DPTest::~DPTest()
    {
        ClearAll();
    }

    void SetTestVal(IZ_UINT val)
    {
        SetValue(sProp, val);
    }

    IZ_UINT GetTestVal()
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

private:
    IZ_UINT m_TestVal;
};

class BindingTest
{
public:
    BindingTest() {}
    ~BindingTest() {}

    IZ_UINT m_Val;
};

class Binding : public izanagi::Binding
{
public:
    Binding() {}
    virtual ~Binding() {}

    void SetSource(BindingTest* source)
    {
        m_Source = source;
    }

    virtual void GetValue(izanagi::CValue& ret)
    {
        ret.SetValue(m_Source->m_Val);
    }

    virtual void SetValue(const izanagi::CValue& value)
    {
        m_Source->m_Val = value.GetValueAsInt32();
    }

private:
    BindingTest* m_Source;
};

izanagi::DependencyProperty DPTest::sProp("TestVal");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    izanagi::CSimpleMemoryAllocator allocator;

    Test test;

    izanagi::CStdEvent<void, IZ_UINT> event1;
    event1.Init(&allocator);
    event1 += izanagi::EventHandler(&allocator, OnEvent);
    event1 += izanagi::EventHandler(&allocator, &test, &Test::OnEvent);
    event1(1);

    izanagi::CStdEvent<IZ_BOOL, IZ_UINT> event2;
    event2.Init(&allocator);
    event2 += izanagi::EventHandler(&allocator, OnEventEx);
    event2 += izanagi::EventHandler(&allocator, &test, &Test::OnEventEx);
    event2(1);

    DPTest dpTest;
    dpTest.SetTestVal(100);
    IZ_UINT val = dpTest.GetTestVal();
    IZ_PRINTF("%d\n", val);

    Binding* binding = izanagi::Binding::CreateBinding<Binding>(&allocator, "m_Val");

    BindingTest bindTest;
    binding->SetSource(&bindTest);
    
    izanagi::BindingOperations::SetBindings(
        &allocator,
        *(izanagi::DependencyObjectBase*)&dpTest,
        DPTest::sProp,
        binding);

    bindTest.m_Val = 200;
    izanagi::BindingOperations::GetBindingExpression(DPTest::sProp)->Update();
    val = dpTest.GetTestVal();
    IZ_PRINTF("%d\n", val);

    return 0;
}
