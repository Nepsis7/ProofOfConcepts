#define DECLARE_TESTER(name) ___Tester_##name()
#define CALL_TESTER(name) ___Tester_##name()

#pragma region Not

template <typename T>
struct Not
{
    enum { Value = !T::Value };
};

void DECLARE_TESTER(Not)
{
    struct SomeTrait
    {
        enum { Value = false };
    };
    static_assert(Not<SomeTrait>::Value);
}
#pragma endregion Not
#pragma region IsLValueReference
template <typename T>
struct IsLValueReference
{
    enum { Value = false };
};

template <typename T>
struct IsLValueReference<T&>
{
    enum { Value = true };
};

void DECLARE_TESTER(IsLValueReference)
{
    static_assert(IsLValueReference<int&>::Value);
    static_assert(Not<IsLValueReference<int>>::Value);
}
#pragma endregion IsLValueReference
#pragma region IsRValueReference
template <typename T>
struct IsRValueReference
{
    enum { Value = false };
};

template <typename T>
struct IsRValueReference<T&>
{
    enum { Value = false };
};

template <typename T>
struct IsRValueReference<T&&>
{
    enum { Value = true };
};

void DECLARE_TESTER(IsRValueReference)
{
    static_assert(Not<IsRValueReference<int>>::Value);
    static_assert(Not<IsRValueReference<int&>>::Value);
    static_assert(IsRValueReference<int&&>::Value);
}
#pragma endregion IsRValueReference
#pragma region IsReference
template <typename T>
struct IsReference
{
    enum { Value = false };
};

template <typename T>
struct IsReference<T&>
{
    enum { Value = true };
};

template <typename T>
struct IsReference<T&&>
{
    enum { Value = true };
};

void DECLARE_TESTER(IsReference)
{
    static_assert(Not<IsReference<int>>::Value);
    static_assert(IsReference<int&>::Value);
    static_assert(IsReference<int&&>::Value);
}
#pragma endregion IsReference
#pragma region IsPointer
template <typename T>
struct IsPointer
{
    enum { Value = false };
};

template <typename T>
struct IsPointer<T*>
{
    enum { Value = true };
};

void DECLARE_TESTER(IsPointer)
{
    static_assert(Not<IsPointer<int>>::Value);
    static_assert(IsPointer<int*>::Value);
    static_assert(IsPointer<int**>::Value);
}
#pragma endregion IsPointer
#pragma region IsMultiPointer
template <typename T>
struct IsMultiPointer
{
    enum { Value = false };
};

template <typename T>
struct IsMultiPointer<T*>
{
    enum { Value = false };
};

template <typename T>
struct IsMultiPointer<T**>
{
    enum { Value = true };
};

void DECLARE_TESTER(IsMultiPointer)
{
    static_assert(Not<IsMultiPointer<int>>::Value);
    static_assert(Not<IsMultiPointer<int*>>::Value);
    static_assert(IsMultiPointer<int**>::Value);
    static_assert(IsMultiPointer<int***>::Value);
}
#pragma endregion IsMultiPointer
#pragma region IsSame
template <typename T1, typename T2>
struct IsSame
{
    enum { Value = false };
};

template <typename T>
struct IsSame<T, T>
{
    enum { Value = true };
};

void DECLARE_TESTER(IsSame)
{
    static_assert(IsSame<int, int>::Value);
    static_assert(Not<IsSame<int, float>>::Value);
}
#pragma endregion IsSame
#pragma region RemovePointer
template <typename T>
struct RemovePointer
{
    using Type = T;
};

template <typename T>
struct RemovePointer<T*>
{
    using Type = T;
};

void DECLARE_TESTER(RemovePointer)
{
    static_assert(IsSame<int, RemovePointer<int>::Type>::Value);
    static_assert(IsSame<int, RemovePointer<int*>::Type>::Value);
    static_assert(Not<IsSame<int, RemovePointer<int**>::Type>>::Value);
}
#pragma endregion RemovePointer
#pragma region RemoveMultiPointer
template <typename T>
struct RemoveMultiPointer
{
    using Type = T;
};

template <typename T>
struct RemoveMultiPointer<T*>
{
    using Type = T;
};

template <typename T>
struct RemoveMultiPointer<T**>
{
    using Type = typename RemoveMultiPointer<T>::Type;
};

void DECLARE_TESTER(RemoveMultiPointer)
{
    static_assert(IsSame<int, RemoveMultiPointer<int>::Type>::Value);
    static_assert(IsSame<int, RemoveMultiPointer<int*>::Type>::Value);
    static_assert(IsSame<int, RemoveMultiPointer<int**>::Type>::Value);
    static_assert(IsSame<int, RemoveMultiPointer<int***>::Type>::Value);
}
#pragma endregion RemoveMultiPointer
#pragma region And

template <bool, typename T2>
struct AndValue
{
    enum { Value = T2::Value };
};

template <typename T2>
struct AndValue<false, T2>
{
    enum { Value = false };
};

template <typename T1, typename T2>
struct And : AndValue<T1::Value, T2> {};

void DECLARE_TESTER(And)
{
    struct SomeTrueTrait
    {
        enum { Value = true };
    };
    struct SomeFalseTrait
    {
        enum { Value = false };
    };
    //true && true
    static_assert(And<SomeTrueTrait, SomeTrueTrait>::Value);
    //true && false
    static_assert(Not<And<SomeTrueTrait, SomeFalseTrait>>::Value);
    //false && true
    static_assert(Not<And<SomeFalseTrait, SomeTrueTrait>>::Value);
    //false && false
    static_assert(Not<And<SomeFalseTrait, SomeFalseTrait>>::Value);
}
#pragma endregion And
#pragma region Or
template <bool, typename T2>
struct OrValue
{
    enum { Value = T2::Value };
};

template <typename T2>
struct OrValue<true, T2>
{
    enum { Value = true };
};

template <typename T1, typename T2>
struct Or : OrValue<T1::Value, T2> {};

void DECLARE_TESTER(Or)
{
    struct SomeTrueTrait
    {
        enum { Value = true };
    };
    struct SomeFalseTrait
    {
        enum { Value = false };
    };
    //true && true
    static_assert(Or<SomeTrueTrait, SomeTrueTrait>::Value);
    //true && false
    static_assert(Or<SomeTrueTrait, SomeFalseTrait>::Value);
    //false && true
    static_assert(Or<SomeFalseTrait, SomeTrueTrait>::Value);
    //false && false
    static_assert(Not<Or<SomeFalseTrait, SomeFalseTrait>>::Value);
}
#pragma endregion Or
#pragma region XOr
template <bool, typename T2>
struct XOrValue
{
    enum { Value = !T2::Value };
};

template <typename T2>
struct XOrValue<false, T2>
{
    enum { Value = T2::Value };
};

template <typename T1, typename T2>
struct XOr : XOrValue<T1::Value, T2> {};

void DECLARE_TESTER(XOr)
{
    struct SomeTrueTrait
    {
        enum { Value = true };
    };
    struct SomeFalseTrait
    {
        enum { Value = false };
    };
    //true && true
    static_assert(Not<XOr<SomeTrueTrait, SomeTrueTrait>>::Value);
    //true && false
    static_assert(XOr<SomeTrueTrait, SomeFalseTrait>::Value);
    //false && true
    static_assert(XOr<SomeFalseTrait, SomeTrueTrait>::Value);
    //false && false
    static_assert(Not<XOr<SomeFalseTrait, SomeFalseTrait>>::Value);
}
#pragma endregion XOr
#pragma region TypeTernary
template <bool, typename TTrueType, typename TFalseType>
struct TypeTernary
{
    using Type = TTrueType;
};

template <typename TTrueType, typename TFalseType>
struct TypeTernary<false, TTrueType, TFalseType>
{
    using Type = TFalseType;
};

void DECLARE_TESTER(TypeTernary)
{
    {
        enum { UseInt = true };
        using Type = TypeTernary<UseInt, int, float>::Type;
        static_assert(IsSame<int, Type>::Value);
    }
    {
        enum { UseInt = false };
        using Type = TypeTernary<UseInt, int, float>::Type;
        static_assert(Not<IsSame<int, Type>>::Value);
    }
}
#pragma endregion TypeTernary
#pragma region TransferCVQualifiers
// ReSharper disable CppInconsistentNaming
template <typename TFrom, typename TTo>
struct TransferCVQualifiers
{
    using Type = TTo;
};

template <typename TFrom, typename TTo>
struct TransferCVQualifiers<const TFrom, TTo>
{
    using Type = const TTo;
};

template <typename TFrom, typename TTo>
struct TransferCVQualifiers<volatile TFrom, TTo>
{
    using Type = volatile TTo;
};

template <typename TFrom, typename TTo>
struct TransferCVQualifiers<const volatile TFrom, TTo>
{
    using Type = const volatile TTo;
};

void DECLARE_TESTER(TransferCVQualifiers)
{
    static_assert(IsSame<int, TransferCVQualifiers<float, int>::Type>::Value);
    static_assert(IsSame<const int, TransferCVQualifiers<const float, int>::Type>::Value);
    static_assert(IsSame<volatile int, TransferCVQualifiers<volatile float, int>::Type>::Value);
    static_assert(IsSame<const volatile int, TransferCVQualifiers<const volatile float, int>::Type>::Value);
}

// ReSharper restore CppInconsistentNaming
#pragma endregion TransferCVQualifiers
#pragma region TransferRefQualifiers
template <typename TFrom, typename TTo>
struct TransferRefQualifiers
{
    using Type = TTo;
};

template <typename TFrom, typename TTo>
struct TransferRefQualifiers<TFrom&, TTo>
{
    using Type = TTo&;
};

template <typename TFrom, typename TTo>
struct TransferRefQualifiers<TFrom&&, TTo>
{
    using Type = TTo&&;
};

void DECLARE_TESTER(TransferRefQualifiers)
{
    static_assert(IsSame<int, TransferRefQualifiers<float, int>::Type>::Value);
    static_assert(IsSame<int&, TransferRefQualifiers<float&, int>::Type>::Value);
    static_assert(IsSame<int&&, TransferRefQualifiers<float&&, int>::Type>::Value);
    static_assert(IsSame<const int&, TransferRefQualifiers<const float&, const int>::Type>::Value);
}
#pragma endregion TransferRefQualifiers
#pragma region GlobalConstExprValue
template <typename T, T Val>
struct GlobalConstExprValue
{
    static constexpr T Value = Val;
};

void DECLARE_TESTER(GlobalConstExprValue)
{
    //use cpp20 if you want floating point values
#if __cplusplus >= 202002L
    using PI = GlobalConstExprValue<float, 3.14f>;
#endif
    //instead of having to constants of the save value at different places in your code, you can have that :
    //somewhere
    const int& _refTo83 = GlobalConstExprValue<int, 83>::Value;
    //somewhere else
    const int& _otherRefTo83 = GlobalConstExprValue<int, 83>::Value;
}
#pragma endregion GlobalConstExprValue
#pragma region IsClassOrStruct
template <typename T>
struct IsClassOrStruct
{
private:
    typedef unsigned char uint8;
    typedef unsigned short int uint16;
    template <typename U> static uint16 Func(int U::*);
    template <typename U> static uint8 Func(...);
public:
    enum { Value = !__is_union(T) && sizeof(Func<T>(0)) - 1 };
};
//TODO Tester
#pragma endregion IsClassOrStruct

int main()
{
    CALL_TESTER(Not);
    CALL_TESTER(IsLValueReference);
    CALL_TESTER(IsRValueReference);
    CALL_TESTER(IsReference);
    CALL_TESTER(IsPointer);
    CALL_TESTER(IsMultiPointer);
    CALL_TESTER(IsSame);
    CALL_TESTER(RemovePointer);
    CALL_TESTER(RemoveMultiPointer);
    CALL_TESTER(And);
    CALL_TESTER(Or);
    CALL_TESTER(XOr);
    CALL_TESTER(TypeTernary);
    CALL_TESTER(TransferCVQualifiers);
    CALL_TESTER(TransferRefQualifiers);
    CALL_TESTER(GlobalConstExprValue);
}
