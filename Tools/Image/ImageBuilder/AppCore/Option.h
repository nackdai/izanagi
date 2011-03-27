#if !defined(__OPTION_H__)
#define __OPTION_H__

struct SOption {
	CString in;
	CString out;
};

class COption : public SOption {
public:
	BOOL Analysis(int argc, TCHAR* argv[]);

	BOOL IsValid() const;

private:
	BOOL AfterAnalysis();
};

#endif	// #if !defined(__OPTION_H__)
