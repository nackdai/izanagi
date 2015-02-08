#include <node.h>
#include <v8.h>

#include "NodeJsBinding.h"

v8::Handle<v8::Value> Method(const v8::Arguments& args)
{
    v8::HandleScope scope;

    int ret = Hoge();

    return scope.Close(v8::Number::New(ret));
}

v8::Handle<v8::Value> Loop(const v8::Arguments& args)
{
    v8::HandleScope scope;

    Loop();

    return scope.Close(v8::Number::New(0));
}

v8::Handle<v8::Value> SetRootPath(const v8::Arguments& args)
{
    v8::HandleScope scope;

    v8::String::Utf8Value str(args[0]);
    SetRootPath(*str);

    return scope.Close(v8::Number::New(0));
}

void init(v8::Handle<v8::Object> target)
{
    target->Set(
        v8::String::NewSymbol("Render2D"),
        v8::FunctionTemplate::New(Method)->GetFunction());

    target->Set(
        v8::String::NewSymbol("Loop"),
        v8::FunctionTemplate::New(Loop)->GetFunction());

    target->Set(
        v8::String::NewSymbol("SetRootPath"),
        v8::FunctionTemplate::New(SetRootPath)->GetFunction());
}

NODE_MODULE(Render2D, init)