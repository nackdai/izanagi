#if !defined(__IZANAGI_MATH_NUMERICAL_ANALYSIS_H__)
#define __IZANAGI_MATH_NUMERICAL_ANALYSIS_H__

#include <math.h>
#include <memory.h>
#include "izDefs.h"

namespace izanagi
{
namespace math
{
    /** ���l���.
     */
    class CNumericalAnalysis
    {
    private:
        CNumericalAnalysis();
        ~CNumericalAnalysis();

        NO_COPIABLE(CNumericalAnalysis);

    public:
        struct Func
        {
            virtual IZ_FLOAT operator()(IZ_FLOAT t) = 0;
        };

        /** �񕪖@.
         */
        static IZ_FLOAT ComputeBisectionMethod(
            Func& func,
            IZ_FLOAT a,
            IZ_FLOAT b,
            IZ_FLOAT threshold);

        /** �j���[�g���@.
         */
        static IZ_FLOAT ComputeNewtonMethod(
            Func& func,
            Func& funcd,
            IZ_FLOAT init,
            IZ_FLOAT threshold,
            IZ_UINT loopCnt);
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_NUMERICAL_ANALYSIS_H__)
