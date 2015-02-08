#ifndef __NODE_JS_BINDING_H__
#define __NODE_JS_BINDING_H__

extern "C" {
    int Hoge();

    int Loop();

    void SetRootPath(const char* path);

    typedef void (*Callback)();
    void SetCallback(Callback func);
}

#endif