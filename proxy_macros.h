#ifdef NDEBUG
#define ___PRO_DEBUG(...)
#else
#define ___PRO_DEBUG(...) __VA_ARGS__
#endif  // NDEBUG

#ifdef _MSC_VER
#define ___PRO_ENFORCE_EBO __declspec(empty_bases)
#else
#define ___PRO_ENFORCE_EBO
#endif  // _MSC_VER

#define ___PRO_GEN_DEBUG_SYMBOL_FOR_MEM_ACCESSOR(...) \
    ___PRO_DEBUG( \
        accessor() noexcept { ::std::ignore = &accessor::__VA_ARGS__; })

#define ___PRO_DEF_MEM_ACCESSOR(__Q, __SELF, ...) \
    template <class __F, bool __IsDirect, class __D, class __R, \
        class... __Args> \
    struct accessor<__F, __IsDirect, __D, __R(__Args...) __Q> { \
      ___PRO_GEN_DEBUG_SYMBOL_FOR_MEM_ACCESSOR(__VA_ARGS__) \
      __R __VA_ARGS__(__Args... __args) __Q { \
        return ::pro::proxy_invoke<__IsDirect, __D, __R(__Args...) __Q>( \
            __SELF, ::std::forward<__Args>(__args)...); \
      } \
    }

#define ___PRO_DIRECT_FUNC_IMPL(...) \
    noexcept(noexcept(__VA_ARGS__)) requires(requires { __VA_ARGS__; }) \
    { return __VA_ARGS__; }

#define ___PRO_DEF_MEM_ACCESSOR_TEMPLATE(__MACRO, ...) \
    template <class __F, bool __IsDirect, class __D, class... __Os> \
    struct ___PRO_ENFORCE_EBO accessor { accessor() = delete; }; \
    template <class __F, bool __IsDirect, class __D, class... __Os> \
        requires(sizeof...(__Os) > 1u && (::std::is_constructible_v< \
            accessor<__F, __IsDirect, __D, __Os>> && ...)) \
    struct accessor<__F, __IsDirect, __D, __Os...> \
        : accessor<__F, __IsDirect, __D, __Os>... \
        { using accessor<__F, __IsDirect, __D, __Os>::__VA_ARGS__...; }; \
    __MACRO(, ::pro::access_proxy<__F>(*this), __VA_ARGS__); \
    __MACRO(noexcept, ::pro::access_proxy<__F>(*this), __VA_ARGS__); \
    __MACRO(&, ::pro::access_proxy<__F>(*this), __VA_ARGS__); \
    __MACRO(& noexcept, ::pro::access_proxy<__F>(*this), __VA_ARGS__); \
    __MACRO(&&, ::pro::access_proxy<__F>(::std::move(*this)), __VA_ARGS__); \
    __MACRO(&& noexcept, ::pro::access_proxy<__F>(::std::move(*this)), \
        __VA_ARGS__); \
    __MACRO(const, ::pro::access_proxy<__F>(*this), __VA_ARGS__); \
    __MACRO(const noexcept, ::pro::access_proxy<__F>(*this), __VA_ARGS__); \
    __MACRO(const&, ::pro::access_proxy<__F>(*this), __VA_ARGS__); \
    __MACRO(const& noexcept, ::pro::access_proxy<__F>(*this), __VA_ARGS__); \
    __MACRO(const&&, ::pro::access_proxy<__F>(::std::move(*this)), \
        __VA_ARGS__); \
    __MACRO(const&& noexcept, ::pro::access_proxy<__F>(::std::move(*this)), \
        __VA_ARGS__);

#define ___PRO_EXPAND_IMPL(__X) __X
#define ___PRO_EXPAND_MACRO_IMPL(__MACRO, __1, __2, __3, __NAME, ...) \
    __MACRO##_##__NAME
#define ___PRO_EXPAND_MACRO(__MACRO, ...) \
    ___PRO_EXPAND_IMPL(___PRO_EXPAND_MACRO_IMPL( \
        __MACRO, __VA_ARGS__, 3, 2)(__VA_ARGS__))

#define ___PRO_DEF_MEM_DISPATCH_IMPL(__NAME, __FUNC, __FNAME) \
    struct __NAME { \
      template <class __T, class... __Args> \
      decltype(auto) operator()(__T&& __self, __Args&&... __args) const \
          ___PRO_DIRECT_FUNC_IMPL(::std::forward<__T>(__self) \
              .__FUNC(::std::forward<__Args>(__args)...)) \
      ___PRO_DEF_MEM_ACCESSOR_TEMPLATE(___PRO_DEF_MEM_ACCESSOR, __FNAME) \
    }
#define ___PRO_DEF_MEM_DISPATCH_2(__NAME, __FUNC) \
    ___PRO_DEF_MEM_DISPATCH_IMPL(__NAME, __FUNC, __FUNC)
#define ___PRO_DEF_MEM_DISPATCH_3(__NAME, __FUNC, __FNAME) \
    ___PRO_DEF_MEM_DISPATCH_IMPL(__NAME, __FUNC, __FNAME)
#define PRO_DEF_MEM_DISPATCH(__NAME, ...) \
    ___PRO_EXPAND_MACRO(___PRO_DEF_MEM_DISPATCH, __NAME, __VA_ARGS__)

#define ___PRO_DEF_FREE_ACCESSOR(__Q, __NE, __SELF_ARG, __SELF, ...) \
    template <class __F, bool __IsDirect, class __D, class __R, \
        class... __Args> \
    struct accessor<__F, __IsDirect, __D, __R(__Args...) __Q> { \
      friend __R __VA_ARGS__(__SELF_ARG, __Args... __args) __NE { \
        return ::pro::proxy_invoke<__IsDirect, __D, __R(__Args...) __Q>( \
            __SELF, ::std::forward<__Args>(__args)...); \
      } \
___PRO_DEBUG( \
      accessor() noexcept { ::std::ignore = &accessor::_symbol_guard; } \
    \
     private: \
      static inline __R _symbol_guard(__SELF_ARG, __Args... __args) __NE { \
        return __VA_ARGS__(::std::forward<decltype(__self)>(__self), \
            ::std::forward<__Args>(__args)...); \
      } \
) \
    }
#define ___PRO_DEF_FREE_DISPATCH_IMPL(__NAME, __FUNC, __FNAME) \
    struct __NAME { \
      template <class __T, class... __Args> \
      decltype(auto) operator()(__T&& __self, __Args&&... __args) const \
          ___PRO_DIRECT_FUNC_IMPL(__FUNC(::std::forward<__T>(__self), \
              ::std::forward<__Args>(__args)...)) \
      ___PRO_DEF_FREE_ACCESSOR_TEMPLATE(___PRO_DEF_FREE_ACCESSOR, __FNAME) \
    }
#define ___PRO_DEF_FREE_DISPATCH_2(__NAME, __FUNC) \
    ___PRO_DEF_FREE_DISPATCH_IMPL(__NAME, __FUNC, __FUNC)
#define ___PRO_DEF_FREE_DISPATCH_3(__NAME, __FUNC, __FNAME) \
    ___PRO_DEF_FREE_DISPATCH_IMPL(__NAME, __FUNC, __FNAME)
#define PRO_DEF_FREE_DISPATCH(__NAME, ...) \
    ___PRO_EXPAND_MACRO(___PRO_DEF_FREE_DISPATCH, __NAME, __VA_ARGS__)

#define ___PRO_DEF_FREE_AS_MEM_DISPATCH_IMPL(__NAME, __FUNC, __FNAME) \
    struct __NAME { \
      template <class __T, class... __Args> \
      decltype(auto) operator()(__T&& __self, __Args&&... __args) const \
          ___PRO_DIRECT_FUNC_IMPL(__FUNC(::std::forward<__T>(__self), \
              ::std::forward<__Args>(__args)...)) \
      ___PRO_DEF_MEM_ACCESSOR_TEMPLATE(___PRO_DEF_MEM_ACCESSOR, __FNAME) \
    }
#define ___PRO_DEF_FREE_AS_MEM_DISPATCH_2(__NAME, __FUNC) \
    ___PRO_DEF_FREE_AS_MEM_DISPATCH_IMPL(__NAME, __FUNC, __FUNC)
#define ___PRO_DEF_FREE_AS_MEM_DISPATCH_3(__NAME, __FUNC, __FNAME) \
    ___PRO_DEF_FREE_AS_MEM_DISPATCH_IMPL(__NAME, __FUNC, __FNAME)
#define PRO_DEF_FREE_AS_MEM_DISPATCH(__NAME, ...) \
    ___PRO_EXPAND_MACRO(___PRO_DEF_FREE_AS_MEM_DISPATCH, __NAME, __VA_ARGS__)

#define PRO_DEF_WEAK_DISPATCH(__NAME, __D, __FUNC) \
    struct [[deprecated("'PRO_DEF_WEAK_DISPATCH' is deprecated. " \
        "Use pro::weak_dispatch<" #__D "> instead.")]] __NAME : __D { \
      using __D::operator(); \
      template <class... __Args> \
      decltype(auto) operator()(::std::nullptr_t, __Args&&... __args) const \
          ___PRO_DIRECT_FUNC_IMPL(__FUNC(::std::forward<__Args>(__args)...)) \
    }
