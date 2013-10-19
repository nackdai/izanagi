#include "highgraph/GraphMath.h"

namespace izanagi {
namespace highgraph {

    // Clip - Screen 座標変換マトリクス計算
    void CGraphMath::ComputeC2S(
        math::SMatrix& mtxC2S,
        IZ_INT screenWidth,
        IZ_INT screenHeight,
        IZ_FLOAT minZ,
        IZ_FLOAT maxZ)
    {
        math::SMatrix::SetUnit(mtxC2S);

        mtxC2S.m[0][0] = screenWidth * 0.5f;;
        mtxC2S.m[1][1] = -screenHeight * 0.5f;;

        mtxC2S.m[2][2] = maxZ - minZ;
        mtxC2S.m[3][2] = minZ;

        mtxC2S.m[3][0] = screenWidth * 0.5f;;
        mtxC2S.m[3][1] = screenHeight * 0.5f;
    }

    // スクリーン距離計算
    IZ_FLOAT CGraphMath::ComputeScreenDistance(
        IZ_FLOAT screenHeight,
        IZ_FLOAT fov)
    {
        // NOTE
        // (H/2)/S = tan(θ/2) <=> S = (H/2)/tan(θ/2)
        IZ_FLOAT ret = screenHeight * 0.5f / tanf(fov * 0.5f);
        return ret;
    }

    // クリップ座標取得
    void CGraphMath::Screen2Clip(
        math::SVector& vClip,
        const graph::SViewport& vp,
        IZ_INT nX, IZ_INT nY,
        IZ_FLOAT fZ)
    {
        IZ_INT nW = vp.width;
        IZ_INT nH = vp.height;
        IZ_INT nCx = nW >> 1;
        IZ_INT nCy = nH >> 1;

        vClip.Set(
            2.0f * (nX - nCx) / nW,
            -2.0f * (nY - nCy) / nH,
            (fZ - vp.minZ) / (vp.maxZ - vp.minZ),
            1.0f);
    }

    inline IZ_BOOL _IsCross(
        const math::CPlane& scissorPlane,
        const math::STriangle& tri,
        IZ_UINT pos0, IZ_UINT pos1)
    {
        math::SVector dir;
        math::SVector::SubXYZ(
            dir,
            tri.pt[pos1],
            tri.pt[pos0]);

        math::CRay ray(tri.pt[pos0], dir);

        IZ_BOOL isCross = scissorPlane.IsCross(ray);
        return isCross;
    }

    // シザリングで作成される三角形の数を計算する.
    IZ_UINT CScissoring::ComputeTriNumByScissoring(
        const math::CPlane& scissorPlane,
        const math::CTriangle triangle[],
        IZ_UINT triNum)
    {
        IZ_UINT newTriNum = 0;

        // 交差する点と面の法線側にある点の数を数える
        for (IZ_UINT i = 0; i < triNum; i++)
        {
            IZ_UINT pointNum = 0;

            if (scissorPlane.IsPositive(triangle[i].pt[0]))
            {
                pointNum++;
            }
            if (scissorPlane.IsPositive(triangle[i].pt[1]))
            {
                pointNum++;
            }
            if (scissorPlane.IsPositive(triangle[i].pt[2]))
            {
                pointNum++;
            }

            // ひとつでも面の負の側に点が存在するかどうか
            if ((0 < pointNum) && (pointNum < 3))
            {
                // 平面と交差する点の数を計算する
                IZ_UINT crossNum = GetCrossNum(scissorPlane, triangle[i]);

                if (crossNum > 0)
                {
                    pointNum += crossNum;

                    // 作成する三角形の数を計算
                    newTriNum += pointNum - 2;
                }
            }
        }

        return newTriNum;
    }

    // 平面と交差する点の数を計算する.
    IZ_UINT CScissoring::GetCrossNum(
        const math::CPlane& scissorPlane,
        const math::CTriangle& triangle)
    {
#if 0
        IZ_UINT crossNum = 0;

        if (_IsCross(scissorPlane, triangle, 0, 1))
        {
            crossNum++;
        }
        if (_IsCross(scissorPlane, triangle, 1, 2))
        {
            crossNum++;
        }
        if (_IsCross(scissorPlane, triangle, 2, 0))
        {
            crossNum++;
        }

        return crossNum;
#else
        // １点でも交差すれば
        // 平面に対しては必ず２点の交差点が存在する

        if (_IsCross(scissorPlane, triangle, 0, 1))
        {
            return 2;
        }
        if (_IsCross(scissorPlane, triangle, 1, 2))
        {
            return 2;
        }
        if (_IsCross(scissorPlane, triangle, 2, 0))
        {
            return 2;
        }

        return 0;
#endif
    }

    // 指定された三角形の辺と面の交点を計算する
    //
    // @param[out] ret 交点
    // @param[in] scissorPlane 面
    // @param[in] tri 三角形
    // @param[in] pos0 三角形の頂点インデックス
    // @param[in] pos1 三角形の頂点インデックス
    // @return 辺と面が交わるかどうか
    inline IZ_BOOL _GetCrossPoint(
        math::SVector& ret,
        const math::CPlane& scissorPlane,
        const math::STriangle& tri,
        IZ_UINT pos0, IZ_UINT pos1)
    {
#if 0
        math::SVector dir;
        math::SVector::SubXYZ(
            dir,
            tri.pt[pos1],
            tri.pt[pos0]);

        math::CRay ray(tri.pt[pos0], dir);

        IZ_BOOL isCross = scissorPlane.GetCrossPoint(ray, ret);
        return isCross;
#else
        IZ_BOOL isCross = scissorPlane.GetCrossPoint(
            tri.pt[pos0],
            tri.pt[pos1],
            ret);
        return isCross;
#endif
    }

    // For array
    struct FuncScissor
    {
        void operator()(
            const math::SVector point[],
            const IZ_UINT idxTbl[][3],
            IZ_UINT triNum,
            math::CTriangle* newTriArray)
        {
            for (IZ_UINT i = 0; i < triNum; i++)
            {
                for (IZ_UINT n = 0; n < 3; n++)
                {
                    IZ_UINT idx = idxTbl[i][n];

                    newTriArray[i].pt[n].Set(
                        point[idx].x,
                        point[idx].y,
                        point[idx].z);
                }
            }
        }
    };

    // For STL vector
    struct FuncScissorSTL
    {
        void operator()(
            const math::SVector point[],
            const IZ_UINT idxTbl[][3],
            IZ_UINT triNum,
            std::vector<math::CTriangle, STLMemoryAllocator<math::CTriangle> >& triList)
        {
            for (IZ_UINT i = 0; i < triNum; i++)
            {
                triList.push_back(math::CTriangle());
                math::CTriangle& tri = triList[triList.size() - 1];

                for (IZ_UINT n = 0; n < 3; n++)
                {
                    IZ_UINT idx = idxTbl[i][n];

                    tri.pt[n].Set(
                        point[idx].x,
                        point[idx].y,
                        point[idx].z);
                }
            }
        }
    };

    template <typename TriArrayType, typename Func>
    IZ_UINT _Scissoring(
        const math::CPlane& scissorPlane,
        const math::STriangle& triangle,
        TriArrayType newTriArray,
        Func& func)
    {
        IZ_UINT positivePointNum = 0;

        // 基準点を探す
        IZ_INT basePos = -1;
        for (IZ_UINT i = 0; i < 3; i++)
        {
            if (scissorPlane.IsPositive(triangle.pt[i]))
            {
                if (basePos < 0)
                {
                    basePos = i;
                }
                positivePointNum++;
            }
        }

#if 0
        if (basePos < 0 || positivePointNum == 3)
        {
            return 0;
        }
#else
        if (basePos < 0)
        {
            return 0;
        }
#endif

        // 調べていく頂点の位置
        static const IZ_UINT vtxIdxTbl[][4] =
        {
            {0, 1, 2, 0},
            {1, 2, 0, 1},
            {2, 0, 1, 2},
        };

        // 最大で４頂点まで
        math::SVector tmp[4];

        IZ_UINT vtxNum = 0;

        // 基準点
        tmp[vtxNum++].Set(
            triangle.pt[basePos].x,
            triangle.pt[basePos].y,
            triangle.pt[basePos].z);

        for (IZ_UINT i = 0; i < 3; i++)
        {
            IZ_UINT idx0 = vtxIdxTbl[basePos][i];
            IZ_UINT idx1 = vtxIdxTbl[basePos][i + 1];

            // 面の法線側にある点
            if (i > 0)
            {
                if (scissorPlane.IsPositive(triangle.pt[idx0]))
                {
                    IZ_ASSERT(vtxNum + 1 <= COUNTOF(tmp));

                    tmp[vtxNum++].Set(
                        triangle.pt[idx0].x,
                        triangle.pt[idx0].y,
                        triangle.pt[idx0].z);
                }
            }

            // 交点
            if (_GetCrossPoint(
                tmp[vtxNum],
                scissorPlane,
                triangle,
                idx0, idx1))
            {
                vtxNum++;
                IZ_ASSERT(vtxNum <= COUNTOF(tmp));
            }
        }

        if (vtxNum < 3)
        {
            // 三角形を構成できないので
            return 0;
        }

        // 最大でも三角形からは４頂点のはず
        IZ_ASSERT(vtxNum <= 4);

        // 三角形を構成
        IZ_UINT triNum = vtxNum - 2;

        // 三角形を構成する頂点のインデックス
        static const IZ_UINT triVtxTbl[][3] =
        {
            {0, 1, 2},
            {0, 2, 3},
        };

        func(
            tmp,
            triVtxTbl,
            triNum,
            newTriArray);
            
        return triNum;
    }

    // シザリング
    IZ_UINT CScissoring::Scissor(
        const math::CPlane& scissorPlane,
        const math::CTriangle triangle[],
        IZ_UINT triNum,
        math::CTriangle newTriangle[],
        IZ_UINT newTriNum)
    {
        IZ_UINT triPos = 0;
        FuncScissor func;

        for (IZ_UINT i = 0; i < triNum; i++)
        {
            triPos += _Scissoring(
                scissorPlane,
                triangle[i],
                &newTriangle[triPos],
                func);

            IZ_ASSERT(triPos <= newTriNum);
        }

        return triPos;
    }

    IZ_UINT CScissoring::Scissor(
        const math::CPlane& scissorPlane,
        const math::CTriangle triangle[],
        IZ_UINT triNum,
        std::vector<math::CTriangle, STLMemoryAllocator<math::CTriangle> >& newTriangle)
    {
        IZ_UINT triPos = 0;
        FuncScissorSTL func;

        for (IZ_UINT i = 0; i < triNum; i++)
        {
            triPos += _Scissoring<std::vector<math::CTriangle, STLMemoryAllocator<math::CTriangle> >&>(
                scissorPlane,
                triangle[i],
                newTriangle,
                func);
        }

        return triPos;
    }
}   // namespace highgraph
}   // namespace izanagi
