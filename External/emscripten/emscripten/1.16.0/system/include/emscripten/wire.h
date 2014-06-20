#pragma once

// A value moving between JavaScript and C++ has three representations:
// - The original JS value: a String
// - The native on-the-wire value: a stack-allocated char*, say
// - The C++ value: std::string
//
// We'll call the on-the-wire type WireType.

#include <stdio.h>
#include <cstdlib>
#include <memory>
#include <string>

#define EMSCRIPTEN_ALWAYS_INLINE __attribute__((always_inline))

namespace emscripten {
    #ifndef EMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES
    #define EMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES 1
    #endif


    #if EMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES
    constexpr bool has_unbound_type_names = true;
    #else
    constexpr bool has_unbound_type_names = false;
    #endif

    namespace internal {
        typedef void (*GenericFunction)();

        typedef const struct _TYPEID {}* TYPEID;


        // We don't need the full std::type_info implementation.  We
        // just need a unique identifier per type and polymorphic type
        // identification.
        
        template<typename T>
        struct CanonicalizedID {
            static TYPEID get() {
                static _TYPEID c;
                return &c;
            }
        };

        template<typename T>
        struct Canonicalized {
            typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
        };

        template<typename T>
        struct LightTypeID {
            static TYPEID get() {
                typedef typename Canonicalized<T>::type C;
                if (has_unbound_type_names || std::is_polymorphic<C>::value) {
                    return reinterpret_cast<TYPEID>(&typeid(C));
                } else {
                    return CanonicalizedID<C>::get();
                }
            }
        };

        template<typename T>
        const TYPEID getLightTypeID(const T& value) {
            typedef typename Canonicalized<T>::type C;
            if (has_unbound_type_names || std::is_polymorphic<C>::value) {
                return reinterpret_cast<TYPEID>(&typeid(value));
            } else {
                return LightTypeID<T>::get();
            }
        }

        template<typename T>
        struct TypeID {
            static TYPEID get() {
                return LightTypeID<T>::get();
            }
        };

        template<typename T>
        struct TypeID<std::unique_ptr<T>> {
            static TYPEID get() {
                return TypeID<T>::get();
            }
        };

        template<typename T>
        struct TypeID<T*> {
            static_assert(!std::is_pointer<T*>::value, "Implicitly binding raw pointers is illegal.  Specify allow_raw_pointer<arg<?>>");
        };

        template<typename T>
        struct AllowedRawPointer {
        };

        template<typename T>
        struct TypeID<AllowedRawPointer<T>> {
            static TYPEID get() {
                return LightTypeID<T*>::get();
            }
        };
        
        // ExecutePolicies<>

        template<typename... Policies>
        struct ExecutePolicies;

        template<>
        struct ExecutePolicies<> {
            template<typename T, int Index>
            struct With {
                typedef T type;
            };
        };
        
        template<typename Policy, typename... Remaining>
        struct ExecutePolicies<Policy, Remaining...> {
            template<typename T, int Index>
            struct With {
                typedef typename Policy::template Transform<
                    typename ExecutePolicies<Remaining...>::template With<T, Index>::type,
                    Index
                >::type type;
            };
        };

        // ArgTypes<>

        template<int Index, typename... Args>
        struct ArgTypes;

        template<int Index>
        struct ArgTypes<Index> {
            template<typename... Policies>
            static void fill(TYPEID* argTypes) {
            }
        };

        template<int Index, typename T, typename... Remaining>
        struct ArgTypes<Index, T, Remaining...> {
            template<typename... Policies>
            static void fill(TYPEID* argTypes) {
                typedef typename ExecutePolicies<Policies...>::template With<T, Index>::type TransformT;
                *argTypes = TypeID<TransformT>::get();
                return ArgTypes<Index + 1, Remaining...>::template fill<Policies...>(argTypes + 1);
            }
        };

        // WithPolicies<...>::ArgTypeList<...>
        template<typename... Policies>
        struct WithPolicies {
            template<typename... Args>
            struct ArgTypeList {
                ArgTypeList() {
                    count = sizeof...(Args);
                    ArgTypes<0, Args...>::template fill<Policies...>(types);
                }

                unsigned count;
                TYPEID types[sizeof...(Args)];
            };
        };

        // BindingType<T>

        template<typename T>
        struct BindingType;

#define EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(type)                 \
        template<>                                                  \
        struct BindingType<type> {                                  \
            typedef type WireType;                                  \
            constexpr static WireType toWireType(const type& v) {   \
                return v;                                           \
            }                                                       \
            constexpr static type fromWireType(WireType v) {        \
                return v;                                           \
            }                                                       \
        }

        EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(char);
        EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(signed char);
        EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(unsigned char);
        EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(signed short);
        EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(unsigned short);
        EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(signed int);
        EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(unsigned int);
        EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(signed long);
        EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(unsigned long);
        EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(float);
        EMSCRIPTEN_DEFINE_NATIVE_BINDING_TYPE(double);

        template<>
        struct BindingType<void> {
            typedef void WireType;
        };

        template<>
        struct BindingType<bool> {
            typedef bool WireType;
            static WireType toWireType(bool b) {
                return b;
            }
            static bool fromWireType(WireType wt) {
                return wt;
            }
        };

        template<>
        struct BindingType<std::string> {
            typedef struct {
                size_t length;
                char data[1]; // trailing data
            }* WireType;
            static WireType toWireType(const std::string& v) {
                WireType wt = (WireType)malloc(sizeof(size_t) + v.length());
                wt->length = v.length();
                memcpy(wt->data, v.data(), v.length());
                return wt;
            }
            static std::string fromWireType(WireType v) {
                return std::string(v->data, v->length);
            }
        };

        template<>
        struct BindingType<std::wstring> {
            typedef struct {
                size_t length;
                wchar_t data[1]; // trailing data
            }* WireType;
            static WireType toWireType(const std::wstring& v) {
                WireType wt = (WireType)malloc(sizeof(size_t) + v.length() * sizeof(wchar_t));
                wt->length = v.length();
                wmemcpy(wt->data, v.data(), v.length());
                return wt;
            }
            static std::wstring fromWireType(WireType v) {
                return std::wstring(v->data, v->length);
            }
        };

        template<typename T>
        struct BindingType<const T> : public BindingType<T> {
        };

        template<typename T>
        struct BindingType<T&> : public BindingType<T> {
        };

        template<typename T>
        struct BindingType<const T&> : public BindingType<T> {
        };

        template<typename T>
        struct BindingType<T&&> {
            typedef typename BindingType<T>::WireType WireType;
            static WireType toWireType(const T& v) {
                return BindingType<T>::toWireType(v);
            }
            static T fromWireType(WireType wt) {
                return BindingType<T>::fromWireType(wt);
            }
        };

        template<typename T>
        struct BindingType<T*> {
            typedef T* WireType;
            static WireType toWireType(T* p) {
                return p;
            }
            static T* fromWireType(WireType wt) {
                return wt;
            }
        };

        template<typename T>
        struct GenericBindingType {
            typedef typename std::remove_reference<T>::type ActualT;
            typedef ActualT* WireType;

            static WireType toWireType(const T& v) {
                return new T(v);
            }

            static WireType toWireType(T&& v) {
                return new T(std::forward<T>(v));
            }

            static ActualT& fromWireType(WireType p) {
                return *p;
            }
        };

        // Is this necessary?
        template<typename T>
        struct GenericBindingType<std::unique_ptr<T>> {
            typedef typename BindingType<T>::WireType WireType;

            static WireType toWireType(std::unique_ptr<T> p) {
                return BindingType<T>::toWireType(*p);
            }
        };

        template<typename Enum>
        struct EnumBindingType {
            typedef Enum WireType;

            static WireType toWireType(Enum v) {
                return v;
            }
            static Enum fromWireType(WireType v) {
                return v;
            }
        };

        // catch-all generic binding
        template<typename T>
        struct BindingType : std::conditional<
            std::is_enum<T>::value,
            EnumBindingType<T>,
            GenericBindingType<T> >::type
        {};

        template<typename T>
        auto toWireType(T&& v) -> typename BindingType<T>::WireType {
            return BindingType<T>::toWireType(std::forward<T>(v));
        }
    }

    struct memory_view {
        enum class Type {
            Int8Array,
            Uint8Array,
            Int16Array,
            Uint16Array,
            Int32Array,
            Uint32Array,
            Float32Array,
            Float64Array,
        };

        memory_view() = delete;
        explicit memory_view(size_t size, const void* data)
            : type(Type::Uint8Array)
            , size(size)
            , data(data)
        {}
        explicit memory_view(Type type, size_t size, const void* data)
            : type(type)
            , size(size)
            , data(data)
        {}

        const Type type;
        const size_t size; // in elements, not bytes
        const void* const data;
    };

    inline memory_view typed_memory_view(size_t size, const int8_t* data) {
        return memory_view(memory_view::Type::Int8Array, size, data);
    }

    inline memory_view typed_memory_view(size_t size, const uint8_t* data) {
        return memory_view(memory_view::Type::Uint8Array, size, data);
    }

    inline memory_view typed_memory_view(size_t size, const int16_t* data) {
        return memory_view(memory_view::Type::Int16Array, size, data);
    }

    inline memory_view typed_memory_view(size_t size, const uint16_t* data) {
        return memory_view(memory_view::Type::Uint16Array, size, data);
    }

    inline memory_view typed_memory_view(size_t size, const int32_t* data) {
        return memory_view(memory_view::Type::Int32Array, size, data);
    }

    inline memory_view typed_memory_view(size_t size, const uint32_t* data) {
        return memory_view(memory_view::Type::Uint32Array, size, data);
    }

    inline memory_view typed_memory_view(size_t size, const float* data) {
        return memory_view(memory_view::Type::Float32Array, size, data);
    }

    inline memory_view typed_memory_view(size_t size, const double* data) {
        return memory_view(memory_view::Type::Float64Array, size, data);
    }

    namespace internal {
        template<>
        struct BindingType<memory_view> {
            // This non-word-sized WireType only works because I
            // happen to know that clang will pass aggregates as
            // pointers to stack elements and we never support
            // converting JavaScript typed arrays back into
            // memory_view.  (That is, fromWireType is not implemented
            // on the C++ side, nor is toWireType implemented in
            // JavaScript.)
            typedef memory_view WireType;
            static WireType toWireType(const memory_view& mv) {
                return mv;
            }
        };
    }
}
