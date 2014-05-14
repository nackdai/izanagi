#if !defined(__IZANAGI_STD_DPENDENCY_PROPERTY_H__)
#define __IZANAGI_STD_DPENDENCY_PROPERTY_H__

#include "izDefs.h"
#include "std/StdPlacementNew.h"
#include "std/StdString.h"

namespace izanagi {
    /** 依存関係プロパティ
     */
    class DependencyProperty
    {
        friend class DependencyPropertyChangedEventArgs;

        IZ_DECL_PLACEMENT_NEW();

    public:
        DependencyProperty() {}

        ~DependencyProperty() {}

        NO_COPIABLE(DependencyProperty);
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_DPENDENCY_PROPERTY_H__)
