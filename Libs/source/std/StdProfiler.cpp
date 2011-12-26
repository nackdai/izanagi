#include "std/StdProfiler.h"
#include "std/allocator/MemoryAllocator.h"
#include "std/StdStack.h"

using namespace izanagi;

namespace {
	IMemoryAllocator* s_Allocator = IZ_NULL;
	IStdProfilePrinter* s_Printer = IZ_NULL;
	IStdProfilerTimer* s_Timer = IZ_NULL;

	IZ_BOOL s_Enable = IZ_FALSE;

	struct S_PROFILE_ITEM {
		char* name;
		IZ_UINT indentLevel;
		IZ_UINT calledNum;
		IZ_FLOAT maxTime;
		IZ_FLOAT minTime;
		IZ_FLOAT aveTime;
		struct {
			IZ_UINT isBegin	: 1;
		} flags;
		IZ_INT64 time;
	}* s_Items = IZ_NULL;

	IZ_UINT s_ItemMax = 0;
	IZ_UINT s_ItemPos = 0;

	CAllocatedStack<S_PROFILE_ITEM*> s_Stack;

	inline IZ_BOOL IsValid()
	{
		return ((s_Printer != IZ_NULL)
				&& (s_Timer != IZ_NULL)
				&& (s_Items != IZ_NULL));
	}
}

// 初期化.
IZ_BOOL CStdProfiler::Init(
	IMemoryAllocator* allocator,
	IZ_UINT maxStackNum,
	IStdProfilerTimer* timer,
	IStdProfilePrinter* printer)
{
	VRETURN(s_Items == NULL);

	// TODO
	maxStackNum = IZ_MIN(maxStackNum, STACK_MAX);
	maxStackNum = IZ_MAX(maxStackNum, 1);

	s_Items = (S_PROFILE_ITEM*)ALLOC_ZERO(allocator, sizeof(S_PROFILE_ITEM) * maxStackNum);
	VRETURN(s_Items != IZ_NULL);

	s_ItemMax = maxStackNum;
	s_ItemPos = 0;

	s_Allocator = allocator;
	s_Printer = printer;
	s_Timer = timer;

	Reset();

	VRETURN(s_Stack.Init(allocator, s_ItemMax));

	return IZ_TRUE;
}

// 後始末.
void CStdProfiler::Release()
{
	for (IZ_UINT i = 0; i < s_ItemMax; i++) {
		FREE(s_Allocator, s_Items[i].name);
	}

	FREE(s_Allocator, s_Items);
}

// 内部データをリセット
void CStdProfiler::Reset()
{
	for (IZ_UINT i = 0; i < s_ItemMax; i++) {
		FREE(s_Allocator, s_Items[i].name);
		s_Items[i].indentLevel = 0;
		s_Items[i].calledNum = 0;
		s_Items[i].maxTime = IZ_FLOAT_MIN;
		s_Items[i].minTime = IZ_FLOAT_MAX;
		s_Items[i].aveTime = 0.0f;
		s_Items[i].flags.isBegin = IZ_FALSE;
		s_Items[i].time = 0;
	}

	s_Stack.Reset();
}

// プロファイル結果を出力.
void CStdProfiler::Dump()
{
	static char buf[256];

	if (!s_Enable) {
		return;
	}

	// ひとつでも何かあれば・・・
	if (s_Items[0].calledNum == 0) {
		return;
	}

	s_Printer->Print("+-------+--------+--------+------+\n");
	s_Printer->Print("| Max   | Min    | Ave    | name |\n");
	s_Printer->Print("+-------+--------+--------+------+\n");

	for (IZ_UINT i = 0; i < s_ItemMax; i++) {
		if (s_Items[i].name == NULL) {
			// ここから先は未登録なので、終了
			break;
		}

		if (s_Items[i].calledNum == 0) {
			continue;
		}

		IZ_SPRINTF(
			buf,
			sizeof(buf),
			" %6.3f | %6.3f | %6.3f | %*s\n\0",
			s_Items[i].maxTime,
			s_Items[i].minTime,
			s_Items[i].aveTime / s_Items[i].calledNum,
			strlen(s_Items[i].name) + (s_Items[i].indentLevel << 2),
			s_Items[i].name);

		s_Printer->Print(buf);
	}
}

// プロファイル開始.
IZ_BOOL CStdProfiler::Begin(const char* name)
{
	if (!s_Enable) {
		return IZ_TRUE;
	}

	if (!IsValid()) {
		return IZ_FALSE;
	}

	VRETURN(s_ItemPos < s_ItemMax);
	VRETURN(!s_Stack.IsFull());

	S_PROFILE_ITEM* newItem = IZ_NULL; 

	size_t nameLen = strlen(name);

	for (IZ_UINT i = 0; i < s_ItemMax; i++) {
		if (s_Items[i].name != NULL) {
			size_t len = strlen(s_Items[i].name);

			if (len == nameLen
				&& strncmp(s_Items[i].name, name, len) == 0)
			{
				IZ_ASSERT(!s_Items[i].flags.isBegin);

				s_Items[i].flags.isBegin = IZ_TRUE;

				s_Items[i].time = s_Timer->GetTime();

				newItem = &s_Items[i];

				break;
			}
		}
		else {
			s_ItemPos = i;

			size_t len = strlen(name);
			len += 1;	// '\0' の分

			len = (len + 3) & ~3;
			s_Items[s_ItemPos].name = (char*)ALLOC(s_Allocator, len);
			IZ_SPRINTF(s_Items[s_ItemPos].name, len, "%s\0", name);

			s_Items[s_ItemPos].indentLevel = s_Stack.GetNum();
			s_Items[s_ItemPos].flags.isBegin = IZ_TRUE;

			s_Items[s_ItemPos].time = s_Timer->GetTime();

			newItem = &s_Items[i];

			break;
		}
	}

	S_PROFILE_ITEM*& item = s_Stack.Get();
	item = newItem;
	VRETURN(s_Stack.Push());

	return IZ_TRUE;
}

/// プロファイル終了.
IZ_BOOL CStdProfiler::End()
{
	if (!s_Enable) {
		return IZ_TRUE;
	}

	if (!IsValid()) {
		return IZ_FALSE;
	}

	VRETURN(!s_Stack.IsEmpty());

	VRETURN(s_Stack.Pop());

	S_PROFILE_ITEM*& item = s_Stack.Get();
	{
		IZ_INT64 end = s_Timer->GetTime();

		IZ_FLOAT time = s_Timer->Compute(item->time, end);

		item->aveTime += time;

		item->calledNum++;
		item->minTime = IZ_MIN(item->minTime, time);
		item->maxTime = IZ_MAX(item->maxTime, time);

		item->flags.isBegin = IZ_FALSE;
	}

	return IZ_TRUE;
}

// プロファイル有効・無効設定.
void CStdProfiler::Enable(IZ_BOOL enable)
{
	s_Enable = enable;
}
