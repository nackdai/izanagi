#include "CrossMapProxy.h"

CCrossMapProxy::CCrossMapProxy(
	izanagi::tool::CTextureLite* tex,
	EnvMapType type)
{
	m_Tex = tex;

    m_Pitch = 0;
	m_Data = IZ_NULL;

    m_Width = m_Tex->GetWidth();
	m_Height = m_Tex->GetHeight();

    m_WidthPerFace = GetWidthPerFace(m_Width);
    m_HeightPerFace = GetHeightPerFace(m_Height);

    // TODO
    IZ_ASSERT(m_WidthPerFace == m_HeightPerFace);

	m_DivW = 1.0f / (m_WidthPerFace - 1);
	m_DivH = 1.0f / (m_HeightPerFace - 1);

	m_IsFloat = izanagi::CGraphUtil::IsFloatPixelFormat(m_Tex->GetPixelFormat());
	m_Bpp = izanagi::CGraphUtil::GetBPP(m_Tex->GetPixelFormat());

    m_CurFace = izanagi::E_GRAPH_CUBE_TEX_FACE_NUM;
	m_PrevFace = izanagi::E_GRAPH_CUBE_TEX_FACE_NUM;
	m_IsChangedFace = IZ_FALSE;
}

CCrossMapProxy::~CCrossMapProxy()
{
	if (m_Data != IZ_NULL) {
		m_Tex->Unlock(0);
		m_Data = IZ_NULL;
	}
}

#define IS_1ST_COLUMN(x, w) (w * 0 <= x && x < w * 1)
#define IS_2ND_COLUMN(x, w) (w * 1 <= x && x < w * 2)
#define IS_3RD_COLUMN(x, w) (w * 2 <= x && x < w * 3)

#define IS_1ST_ROW(y, h)    (h * 0 <= y && y < h * 1)
#define IS_2ND_ROW(y, h)    (h * 1 <= y && y < h * 2)
#define IS_3RD_ROW(y, h)    (h * 2 <= y && y < h * 3)
#define IS_4TH_ROW(y, h)    (h * 3 <= y && y < h * 4)

#define _1ST_COLUMN_X(x, w) (x)
#define _2ND_COLUMN_X(x, w) (x - w)
#define _3RD_COLUMN_X(x, w) (x - w * 2)

#define _1ST_ROW_Y(y, h)    (y)
#define _2ND_ROW_Y(y, h)    (y - h )
#define _3RD_ROW_Y(y, h)    (y - h * 2)
#define _4TH_ROW_Y(y, h)    (y - h * 3)

// XYから反射ベクトルを取得.
void CCrossMapProxy::getRef(
	IZ_UINT x, IZ_UINT y,
	izanagi::SVector& ref,
	izanagi::E_GRAPH_CUBE_TEX_FACE face/*= izanagi::E_GRAPH_CUBE_TEX_FACE_NUM*/)
{
    // NOTE
    //     1   2   3
    //   +---+---+---+
    //  1|   |+y |   |
    //   +---+---+---+
    //  2|-x |+z |+x |
    //   +---+---+---+
    //  3|   |-y |   |
    //   +---+---+---+
    //  4|   |-z |   |
    //   +---+---+---+

	// X,Yからどの面かを決める
    if (IS_1ST_COLUMN(x, m_WidthPerFace))
    {
	    IZ_FLOAT u = 2.0f * _1ST_COLUMN_X(x, m_WidthPerFace) * m_DivW - 1.0f;

        if (IS_1ST_ROW(y, m_HeightPerFace))
        {
            // Nothing
            IZ_ASSERT(IZ_FALSE);
        }
        else if (IS_2ND_ROW(y, m_HeightPerFace))
        {
            IZ_FLOAT v = 2.0f * _2ND_ROW_Y(y, m_HeightPerFace)  * m_DivH - 1.0f;

            // -X
            ref.x = -1.0f;
			ref.y = v;
			ref.z = u;
        }
        else if (IS_3RD_ROW(y, m_HeightPerFace))
        {
            // Nothing
            IZ_ASSERT(IZ_FALSE);
        }
        else if (IS_4TH_ROW(y, m_HeightPerFace))
        {
            // Nothing
            IZ_ASSERT(IZ_FALSE);
        }
    }
    else if (IS_2ND_COLUMN(x, m_WidthPerFace))
    {
        IZ_FLOAT u = 2.0f * _2ND_COLUMN_X(x, m_WidthPerFace) * m_DivW - 1.0f;

        if (IS_1ST_ROW(y, m_HeightPerFace))
        {
            IZ_FLOAT v = 2.0f * _1ST_ROW_Y(y, m_HeightPerFace)  * m_DivH - 1.0f;

            // +Y
            ref.x = u;
			ref.y = -1.0f;
			ref.z = v;
        }
        else if (IS_2ND_ROW(y, m_HeightPerFace))
        {
            IZ_FLOAT v = 2.0f * _2ND_ROW_Y(y, m_HeightPerFace)  * m_DivH - 1.0f;

            // +Z
            ref.x = u;
			ref.y = v;
			ref.z = 1.0f;
        }
        else if (IS_3RD_ROW(y, m_HeightPerFace))
        {
            IZ_FLOAT v = 2.0f * _3RD_ROW_Y(y, m_HeightPerFace)  * m_DivH - 1.0f;

            // -Y
            ref.x = u;
			ref.y = 1.0f;
			ref.z = -v;
        }
        else if (IS_4TH_ROW(y, m_HeightPerFace))
        {
            IZ_FLOAT v = 2.0f * _4TH_ROW_Y(y, m_HeightPerFace)  * m_DivH - 1.0f;

            // -Z
            ref.x = -u;
			ref.y = v;
			ref.z = -1.0f;
        }
    }
    else if (IS_3RD_COLUMN(x, m_WidthPerFace))
    {
        IZ_FLOAT u = 2.0f * _3RD_COLUMN_X(x, m_WidthPerFace) * m_DivW - 1.0f;

        if (IS_1ST_ROW(y, m_HeightPerFace))
        {
            // Nothing
            IZ_ASSERT(IZ_FALSE);
        }
        else if (IS_2ND_ROW(y, m_HeightPerFace))
        {
            IZ_FLOAT v = 2.0f * _2ND_ROW_Y(y, m_HeightPerFace)  * m_DivH - 1.0f;

            // +X
            ref.x = 1.0f;
			ref.y = v;
			ref.z = -u;
        }
        else if (IS_3RD_ROW(y, m_HeightPerFace))
        {
            // Nothing
            IZ_ASSERT(IZ_FALSE);
        }
        else if (IS_4TH_ROW(y, m_HeightPerFace))
        {
            // Nothing
            IZ_ASSERT(IZ_FALSE);
        }
    }

	izanagi::SVector::Normalize(ref, ref);
}

#define _1ST_COLUMN_REAL_X(x, w) (x)
#define _2ND_COLUMN_REAL_X(x, w) (x + w)
#define _3RD_COLUMN_REAL_X(x, w) (x + w * 2)

#define _1ST_ROW_REAL_Y(y, h)    (y)
#define _2ND_ROW_REAL_Y(y, h)    (y + h )
#define _3RD_ROW_REAL_Y(y, h)    (y + h * 2)
#define _4TH_ROW_REAL_Y(y, h)    (y + h * 3)

// UVから色を取得.
void CCrossMapProxy::getColor(
	IZ_FLOAT u, IZ_FLOAT v,
	SFloatColor& color)
{
    IZ_UINT x = static_cast<IZ_UINT>(u * (m_WidthPerFace - 1));
	IZ_UINT y = static_cast<IZ_UINT>(v * (m_HeightPerFace - 1));

	if (m_Data == IZ_NULL) {
		m_Pitch = m_Tex->Lock(0, (void**)&m_Data);
	}

    // NOTE
    //     1   2   3
    //   +---+---+---+
    //  1|   |+y |   |
    //   +---+---+---+
    //  2|-x |+z |+x |
    //   +---+---+---+
    //  3|   |-y |   |
    //   +---+---+---+
    //  4|   |-z |   |
    //   +---+---+---+

    switch (m_CurFace) {
	case izanagi::E_GRAPH_CUBE_TEX_FACE_X_P:	// +X
		x = _3RD_COLUMN_REAL_X(x, m_WidthPerFace);
        y = _2ND_ROW_REAL_Y(y, m_HeightPerFace);
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_X_N:	// -X
		x = _1ST_COLUMN_REAL_X(x, m_WidthPerFace);
        y = _2ND_ROW_REAL_Y(y, m_HeightPerFace);
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Y_N:	// -Y
		x = _2ND_COLUMN_REAL_X(x, m_WidthPerFace);
        y = _3RD_ROW_REAL_Y(y, m_HeightPerFace);
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Y_P:	// +Y
		x = _2ND_COLUMN_REAL_X(x, m_WidthPerFace);
        y = _1ST_ROW_REAL_Y(y, m_HeightPerFace);
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Z_P:	// +Z
		x = _2ND_COLUMN_REAL_X(x, m_WidthPerFace);
        y = _2ND_ROW_REAL_Y(y, m_HeightPerFace);
		break;
	case izanagi::E_GRAPH_CUBE_TEX_FACE_Z_N:	// -Z
        {
		    x = _2ND_COLUMN_REAL_X(x, m_WidthPerFace);
            y = _4TH_ROW_REAL_Y(y, m_HeightPerFace);

            // 反転
            y = (m_Height - 1) - y + (m_HeightPerFace * 3);
        }
		break;
	}

    u = x / (IZ_FLOAT)(m_Width - 1);
    v = y / (IZ_FLOAT)(m_Height - 1);

	CTexProxy::getBiLinearColor(
		m_IsFloat,
		m_Width, m_Height,
		m_Pitch, m_Bpp,
		m_Data,
		u, v,
		color);
}

// 指定位置に描きこみ.
void CCrossMapProxy::putColor(
	IZ_UINT x, IZ_UINT y,
	const SFloatColor& color)
{
	if (m_Data == IZ_NULL) {
		IZ_ASSERT(m_Tex);
		m_Pitch = m_Tex->Lock(0, (void**)&m_Data);
	}

    // -Z面だけ特殊処理
    if (IS_2ND_COLUMN(x, m_WidthPerFace)
        && IS_4TH_ROW(y, m_HeightPerFace))
    {
        // 反転
        y = (m_Height - 1) - y + (m_HeightPerFace * 3);
    }

#if 0
	IZ_UINT8* data = m_Data + m_Pitch * y + x * m_Bpp;

	if (m_IsFloat) {
		IZ_FLOAT* d = reinterpret_cast<IZ_FLOAT*>(data);

		d[0] = color.r;
		d[1] = color.g;
		d[2] = color.b;
		d[3] = color.a;
	}
	else {
		data[0] = color.getAsUint8_R();
		data[1] = color.getAsUint8_G();
		data[2] = color.getAsUint8_B();
		data[3] = color.getAsUint8_A();
	}
#else
	CTexProxy::putColor(
		m_IsFloat,
		x, y,
		m_Pitch, m_Bpp,
		color,
		m_Data);
#endif
}

// XYが正しい位置かどうか.
IZ_BOOL CCrossMapProxy::isValid(IZ_UINT x, IZ_UINT y) const
{
	if (IS_1ST_COLUMN(x, m_WidthPerFace))
    {
        if (IS_1ST_ROW(y, m_HeightPerFace))
        {
            // Nothing
            return IZ_FALSE;
        }
        else if (IS_2ND_ROW(y, m_HeightPerFace))
        {
            // -X
            return IZ_TRUE;
        }
        else if (IS_3RD_ROW(y, m_HeightPerFace))
        {
            // Nothing
            return IZ_FALSE;
        }
        else if (IS_4TH_ROW(y, m_HeightPerFace))
        {
            // Nothing
            return IZ_FALSE;
        }
    }
    else if (IS_2ND_COLUMN(x, m_WidthPerFace))
    {
        if (IS_1ST_ROW(y, m_HeightPerFace))
        {
            // +Y
            return IZ_TRUE;
        }
        else if (IS_2ND_ROW(y, m_HeightPerFace))
        {
            // +Z
            return IZ_TRUE;
        }
        else if (IS_3RD_ROW(y, m_HeightPerFace))
        {
            // -Y
            return IZ_TRUE;
        }
        else if (IS_4TH_ROW(y, m_HeightPerFace))
        {
            // -Z
            return IZ_TRUE;
        }
    }
    else if (IS_3RD_COLUMN(x, m_WidthPerFace))
    {
        if (IS_1ST_ROW(y, m_HeightPerFace))
        {
            // Nothing
            return IZ_FALSE;
        }
        else if (IS_2ND_ROW(y, m_HeightPerFace))
        {
            // +X
            return IZ_TRUE;
        }
        else if (IS_3RD_ROW(y, m_HeightPerFace))
        {
            // Nothing
            return IZ_FALSE;
        }
        else if (IS_4TH_ROW(y, m_HeightPerFace))
        {
            // Nothing
            return IZ_FALSE;
        }
    }

    return IZ_FALSE;
}
