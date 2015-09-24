#include "network/CallbackRegister.h"

namespace izanagi {
namespace net {
    CStdHash<CallbackRegister::Key, CalbackBase, 4> CallbackRegister::s_mapCallback;

    void CallbackRegister::Regist(CalbackBase& callback)
    {
        s_mapCallback.Add(callback.GetHashItem());
    }

    void CallbackRegister::RegistPermanently(CalbackBase& callback)
    {
        callback.EnablePermanently();
        s_mapCallback.Add(callback.GetHashItem());
    }

    void CallbackRegister::Remove(CalbackBase& callback)
    {
        callback.GetHashItem()->Leave();
    }
}    // namespace net
}    // namespace izanagi
