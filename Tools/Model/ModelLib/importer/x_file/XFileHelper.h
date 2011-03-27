#if !defined(__MODEL_LIB_XFILE_HELPER_H__)
#define __MODEL_LIB_XFILE_HELPER_H__

#include "izMath.h"
#include "izSceneGraph.h"
#include <vector>
#include <string>

struct SXFileFace {
	std::vector<IZ_UINT> indices;
};

struct SXFileUV {
	IZ_FLOAT u;
	IZ_FLOAT v;
};

struct SXFileTexture {
	std::string name;
	IZ_BOOL isNormalMap;

	SXFileTexture() { isNormalMap = IZ_FALSE; }
};

struct SXFileMaterial {
	std::string name;
	izanagi::SVector diffuse;
	IZ_FLOAT specularExp;
	izanagi::SVector specular;
	izanagi::SVector emisssion;

	std::vector<SXFileTexture> textures;
};

struct SXFileBone {
	std::string name;
	izanagi::SMatrix mtx;
};

struct SXFileSkin {
	SXFileBone bone;

	std::vector<IZ_UINT> vertex;
	std::vector<IZ_FLOAT> weight;

	IZ_BOOL IsEmpty() { return vertex.empty(); }

	bool operator==(const std::string& strName) const
	{
		return (bone.name.compare(strName) == 0);
	}
};

struct SXFileNode;

struct SXFileMesh {
	SXFileNode* node;

	std::string name;

	std::vector<izanagi::SVector> positions;
	std::vector<izanagi::SVector> normals;
	std::vector<SXFileUV> texcoords;
	std::vector<IZ_COLOR> colors;

	std::vector<SXFileFace> pos_faces;
	std::vector<SXFileFace> nml_faces;
	std::vector<IZ_UINT> mtrl_idx;

	std::vector<SXFileMaterial> mtrls;

	std::vector<SXFileSkin> skins;

	SXFileMesh() { node = IZ_NULL; }
};

struct SXFileNode {
	std::string name;
	SXFileNode* parent;
	izanagi::SMatrix mtxTransform;

	std::vector<SXFileMesh*> meshes;

	std::vector<SXFileNode*> children;

	SXFileNode() { parent = IZ_NULL; }
	SXFileNode(SXFileNode* pParent) { parent = pParent; }

	~SXFileNode()
	{
		for (size_t i = 0; i < meshes.size(); i++) {
			delete meshes[i];
		}
	}
};

///////////////////////////////////////////////

struct SXFileAnmKey {
	IZ_INT time;

	union {
		struct {
			IZ_UINT quat	: 1;
			IZ_UINT scale	: 1;
			IZ_UINT trans	: 1;
			IZ_UINT mtx		: 1;
		};
		IZ_UINT val;
	} type;

	union {
		izanagi::S_SKL_JOINT_POSE pose;
		izanagi::SMatrix mtx;
	} data;

	SXFileAnmKey()
	{
		time = -1;
		type.val = 0;
	}
};

struct SXFileAnm {
	std::string name;

	std::vector<SXFileAnmKey> keys;
};

struct SXFileAnmSet {
	std::string name;

	std::vector<SXFileAnm*> anm;

	~SXFileAnmSet()
	{
		for (size_t i = 0; i < anm.size(); i++) {
			delete anm[i];
		}
	}
};

///////////////////////////////////////////////

class CXFileMathUtil {
public:
	static IZ_BOOL BreakDownMatrix(
		const izanagi::SMatrix& mtx,
		izanagi::S_SKL_JOINT_POSE& sPose);

	static IZ_BOOL HasScale(const izanagi::SMatrix& mtx);
};

#endif	// #if !defined(__MODEL_LIB_XFILE_HELPER_H__)
