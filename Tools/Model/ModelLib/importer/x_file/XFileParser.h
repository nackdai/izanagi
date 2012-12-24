#if !defined(__MODEL_LIB_XFILE_PARSER_H__)
#define __MODEL_LIB_XFILE_PARSER_H__

#include "izIo.h"
#include "izMath.h"
#include <vector>
#include <string>

#include "XFileHelper.h"

class CXFileParser {
    enum E_XFILE_FORMAT {
        E_XFILE_FORMAT_TXT = 0,
        E_XFILE_FORMAT_BIN,
    };

public:
    CXFileParser();
    ~CXFileParser();

public:
    static IZ_BOOL IsTxtFormat(IZ_PCSTR pszFile);

    IZ_BOOL Parse(IZ_PCSTR pszFile);
    SXFileNode* GetRootNode() { return m_pRootNode; }

private:
    void Clear();

    IZ_BOOL ParseFile();

    IZ_BOOL ParseTemplate();

    IZ_BOOL ParseFrame(SXFileNode* pParent);
    IZ_BOOL ParseFrameTransformMatrix(izanagi::math::SMatrix& mtx);

    IZ_BOOL ParseMesh(SXFileMesh* pMesh);
    IZ_BOOL ParseMeshNormal(SXFileMesh* pMesh);
    IZ_BOOL ParseMeshColor(SXFileMesh* pMesh);
    IZ_BOOL ParseMeshTexCoord(SXFileMesh* pMesh);
    IZ_BOOL ParseSkinMeshHeader(SXFileMesh* pMesh);
    IZ_BOOL ParseSkinWeights(SXFileMesh* pMesh);
    IZ_BOOL ParseMeshMaterialList(SXFileMesh* pMesh);

    IZ_BOOL ParseMaterial(SXFileMaterial* pMtrl);
    IZ_BOOL ParseTexture(SXFileTexture* pTex);

    IZ_BOOL ParseAnimTicksPerSecond();
    IZ_BOOL ParseAnimationSet();
    IZ_BOOL ParseAnimation(SXFileAnmSet* pAnmSet);
    IZ_BOOL ParseAnimationKey(SXFileAnm* pAnm);

    IZ_BOOL SkipObject();

private:
    std::string GetToken(IZ_BOOL bSkipData = IZ_TRUE);
    std::string GetTokenEx();

    std::string GetHeader();

    inline IZ_FLOAT ReadFloat();
    inline IZ_INT ReadInt();

    inline void ReadVector(izanagi::math::SVector& vec);
    inline void ReadVectorXYZ(izanagi::math::SVector& vec);
    inline void ReadVectorXY(izanagi::math::SVector& vec);

    inline void ReadFloat2(IZ_FLOAT& f0, IZ_FLOAT& f1);

    inline void ReadRGBA(izanagi::math::SVector& vec);
    inline void ReadRGB(izanagi::math::SVector& vec);

    inline IZ_UINT32 Read4Byte();
    inline IZ_UINT16 Read2Byte();

    inline IZ_BOOL IsSemiColon();
    inline IZ_BOOL IsCloseBrace();
    inline IZ_BOOL IsSeparator();

public:
    const std::vector<SXFileNode*>& GetNodeList() const { return m_NodeList; }
    const std::vector<SXFileAnmSet*>& GetAnmSetList() const { return m_AnmList; }
    const std::vector<SXFileBone>& GetBoneList() const { return m_BoneList; }

    IZ_BOOL HasSkin() { return m_bHasSkin; }

private:
    izanagi::CMemoryInputStream m_MemInStream;

    IZ_BOOL m_bIsBinary;
    IZ_UINT m_nFloatSize;
    IZ_UINT m_nIntBufferedNum;
    IZ_UINT m_nFloatBufferedNum;

    IZ_BOOL m_bHasSkin;

    IZ_UINT m_nAnimTicksPerSecond;
    IZ_INT m_nMaxAnimTime;

    SXFileNode* m_pRootNode;
    std::vector<SXFileNode*> m_NodeList;
    std::vector<SXFileBone> m_BoneList;
    std::vector<SXFileAnmSet*> m_AnmList;

    IZ_UINT m_nReadMtrlNum;
};

#endif  // #if !defined(__MODEL_LIB_XFILE_PARSER_H__)
