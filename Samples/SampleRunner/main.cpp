#include <string>
#include <vector>

#include "izSampleKit.h"

class CSampleRunner : public izanagi::sample::CSampleApp {
public:
    CSampleRunner();
    virtual ~CSampleRunner();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);

private:
    void SearchDir(
        const char* filename,
        const char* tag = IZ_NULL);

    void CloseProcess();

private:
    struct ExeInfo {
        std::string tag;
        std::string path;
    };

    static const IZ_UINT Range = 20;
    static const IZ_UINT Size = 20;
    static const IZ_UINT Left = 100;
    static const IZ_UINT CursorLeft = Left - 30;
    static const IZ_UINT Top = 100;

    static const IZ_UINT Diff = 10;
    static const IZ_UINT Column = 300;

    std::vector< std::vector<ExeInfo> > m_InfoList;

    IZ_UINT m_InfoNum;

    IZ_INT m_ColumnIdx;
    IZ_INT m_RowIdx;

    IZ_BOOL m_RunProcess;

    PROCESS_INFORMATION m_ProcInfo;
};

CSampleRunner::CSampleRunner()
{
    m_ColumnIdx = 0;
    m_RowIdx = 0;

    m_RunProcess = IZ_FALSE;

    memset(&m_ProcInfo, 0, sizeof(m_ProcInfo));
}

CSampleRunner::~CSampleRunner()
{
}

// 初期化.
IZ_BOOL CSampleRunner::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    m_Allocator = allocator;

    SearchDir("..\\*");

    m_ColumnIdx = 0;
    m_RowIdx = 0;

    return IZ_TRUE;
}

//#ifdef __IZ_DEBUG__
#if 1
    #define BASE_DIR    "..\\"
#else
    #define BASE_DIR    ".\\"
#endif

void CSampleRunner::SearchDir(
    const char* filename,
    const char* tag/*= IZ_NULL*/)
{
    HANDLE hFind;
	WIN32_FIND_DATA fd;

	hFind = FindFirstFile(filename, &fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

    do {
        std::string file(fd.cFileName);

        if (file.compare("SampleRunner") == 0
            || file.compare("Bin") == 0)
        {
            continue;
        }

		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (file.compare(".") == 0 || file.compare("..") == 0) {
                continue;
            }

            std::string path(BASE_DIR);
            path.append(file.c_str());
            path.append("\\*.exe");

            SearchDir(path.c_str(), file.c_str());
		}
        else if (file.find(".exe", 0) != std::string::npos) {
            ExeInfo info;
            info.tag = tag;

            info.path.append(BASE_DIR);
            info.path.append(tag);
            info.path.append("\\");
            info.path.append(file.c_str());

            if (m_InfoList.size() == 0) {
                m_InfoList.push_back(std::vector<ExeInfo>());
            }
            else if (m_InfoList[m_ColumnIdx].size() >= Range) {
                m_InfoList.push_back(std::vector<ExeInfo>());
                m_ColumnIdx++;
            }

            m_InfoList[m_ColumnIdx].push_back(info);
            m_InfoNum++;
        }
	} while (FindNextFile(hFind, &fd));
}

void CSampleRunner::CloseProcess()
{
    if (m_ProcInfo.hProcess != NULL) {
        HWND hWnd = GetTopWindow(NULL);

	    do {
		    if (GetWindowLong(hWnd, GWL_HWNDPARENT) != 0
                || !IsWindowVisible(hWnd))
            {
			    continue;
            }

		    DWORD processID;
		    GetWindowThreadProcessId(hWnd, &processID);

            if (m_ProcInfo.dwProcessId == processID) {
                PostMessage(hWnd, WM_CLOSE, 0, 0);

			    break;
            }
	    } while((hWnd = GetNextWindow(hWnd, GW_HWNDNEXT)) != NULL);
    }
}

// 解放.
void CSampleRunner::ReleaseInternal()
{
    CloseProcess();
}

// 更新.
void CSampleRunner::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (m_RunProcess) {
        CloseProcess();

        memset(&m_ProcInfo, 0, sizeof(m_ProcInfo));

        STARTUPINFO startUpInfo;
        memset(&startUpInfo, 0, sizeof(startUpInfo));
        startUpInfo.cb = sizeof(startUpInfo);

        const ExeInfo& info = m_InfoList[m_ColumnIdx][m_RowIdx];

        LPSTR cmd = const_cast<LPSTR>(info.path.c_str());

        if (CreateProcess(
                NULL,
                cmd,
                NULL,
                NULL,
                FALSE,
                NORMAL_PRIORITY_CLASS,
                NULL,
                NULL,
                &startUpInfo,
                &m_ProcInfo))
        {
            CloseHandle(m_ProcInfo.hThread);
        }

        m_RunProcess = IZ_FALSE;
    }
}

// 描画.
void CSampleRunner::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::CDebugFont* debugFont = GetDebugFont();

    device->Begin2D();

    debugFont->Begin(device);

    IZ_UINT cursorPos = 0;

    debugFont->DBPrint(
        device,
        CursorLeft + Column * m_ColumnIdx,
        Top + Size * m_RowIdx + Diff,
        0xffffffff,
        ">");

    for (IZ_UINT y = 0; y < m_InfoList.size(); y++) {
        for (IZ_UINT i = 0; i < Range; i++) {
            IZ_UINT idx = y * Range + i;

            if (idx >= m_InfoNum) {
                break;
            }

            const ExeInfo& info = m_InfoList[y][i];

            debugFont->DBPrint(
                device,
                Left + Column * y,
                Top + Size * i + Diff,
                0xffffffff,
                "%s",
                info.tag.c_str());
        }
    }

    debugFont->End();

    device->End2D();
}

IZ_BOOL CSampleRunner::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    switch (key) {
    case izanagi::sys::E_KEYBOARD_BUTTON_UP:
        m_RowIdx--;
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_DOWN:
        m_RowIdx++;
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_LEFT:
        m_ColumnIdx--;
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_RIGHT:
        m_ColumnIdx++;
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_RETURN:
        m_RunProcess = IZ_TRUE;
        break;
    }

    if (m_RowIdx < 0) {
        m_RowIdx = 0;
        m_ColumnIdx--;

        if (m_ColumnIdx == 0) {
            m_RowIdx = m_InfoList[m_ColumnIdx].size() - 1;
        }
    }
    else if (m_RowIdx >= Range) {
        m_RowIdx = 0;
        m_ColumnIdx++;
    }

    if (m_ColumnIdx < 0) {
        m_ColumnIdx = 0;
    }
    else if (m_ColumnIdx >= m_InfoList.size()) {
        m_ColumnIdx = m_InfoList.size() - 1;
    }

    if (m_RowIdx >= m_InfoList[m_ColumnIdx].size()) {
        m_RowIdx = m_InfoList[m_ColumnIdx].size() - 1;
    }

    return IZ_TRUE;
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CSampleRunner app;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int ret = SampleMain(
        hInstance,
        &app,
        "SampleRunner",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
