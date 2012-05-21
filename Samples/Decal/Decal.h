#if !defined(__DECAL_H__)
#define __DECAL_H__

#include "izGraph.h"
#include "izMath.h"

#include <vector>

#define ENABLE_STL

class CDecal : public izanagi::CObject
{
public:
	static CDecal* Create(
		izanagi::IMemoryAllocator* allocator,
		const izanagi::SVector& point,
		const izanagi::SVector& normal,
		IZ_FLOAT rectangleLengthX,
		IZ_FLOAT rectangleLengthZ);

private:
	CDecal();
	virtual ~CDecal();

	NO_COPIABLE(CDecal);

	IZ_DEFINE_INTERNAL_RELEASE();

public:
	void SetRectangle(
		const izanagi::SVector& point,
		const izanagi::SVector& normal,
		IZ_FLOAT rectangleLengthX,
		IZ_FLOAT rectangleLengthZ);

	void DoScissoring(
		const izanagi::CTriangle tri[],
		IZ_UINT triNum);

	void CreateGraphicsObject(izanagi::CGraphicsDevice* device);

	void Draw(izanagi::CGraphicsDevice* device);

private:
	void ComputeNewTriNumByScissoring(
		const izanagi::CTriangle tri[],
		IZ_UINT triNum);

private:
	typedef std::vector<izanagi::CTriangle, izanagi::STLMemoryAllocator<izanagi::CTriangle> > TriangleVector;

	struct STriListProxy
	{
		TriangleVector triList_0;
		TriangleVector triList_1;

		TriangleVector& Get(IZ_UINT idx)
		{
			if (idx == 0)
			{
				return triList_0;
			}
			else
			{
				return triList_1;
			}
		}

		void Clear()
		{
			triList_0.clear();
			triList_1.clear();
		}
	};

	izanagi::IMemoryAllocator* m_Allocator;

	izanagi::CRectangle m_Rectangle;

	enum {
		PLANE_LEFT,
		PLANE_RIGHT,
		PLANE_BOTTOM,
		PLANE_TOP,
		PLANE_FRONT,
		PLANE_BACK,

		PLANE_NUM,
	};

	izanagi::CPlane m_Planes[PLANE_NUM];

	IZ_UINT m_TriNum;
	izanagi::CTriangle* m_Triangles;

	struct SVtx {
		izanagi::SVector point;
		IZ_COLOR color;
		IZ_FLOAT uv[2];
	};

	IZ_BOOL m_NeedCreateGraphicsObject;

	izanagi::CVertexBuffer* m_VB;
	izanagi::CVertexDeclaration* m_VD;
};


#endif	// #if !defined(__DECAL_H__)