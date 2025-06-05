#pragma once

template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T& > { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };

template <typename T> struct TRemovePointer { typedef T Type; };
template <typename T> struct TRemovePointer<T*> { typedef T Type; };

template <typename T>
typename TRemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
	typedef typename TRemoveReference<T>::Type CastType;

	return (CastType&&)Obj;
}
/**
* Forward will cast a reference to an rvalue reference.
* This is UE's equivalent of std::forward.
*/
template <typename T>
T&& Forward(typename TRemoveReference<T>::Type& Obj)
{
	return (T&&)Obj;
}

template <typename T>
T&& Forward(typename TRemoveReference<T>::Type&& Obj)
{
	return (T&&)Obj;
}

template <typename FuncType, typename... ArgTypes>
auto Invoke(FuncType&& Func, ArgTypes&&... Args) //-> decltype(Forward<FuncType>(Func)(Forward<ArgTypes>(Args)...))
{
	return Forward<FuncType>(Func)(Forward<ArgTypes>(Args)...);
}

template <typename T>
struct TIdentity
{
	typedef T Type;
};

/**
* Uses implicit conversion to create an instance of a specific type.
* Useful to make things clearer or circumvent unintended type deduction in templates.
* Safer than C casts and static_casts, e.g. does not allow down-casts
*
* @param Obj  The object (usually pointer or reference) to convert.
*
* @return The object converted to the specified type.
*/
template <typename T>
T ImplicitConv(typename TIdentity<T>::Type Obj)
{
	return Obj;
}

template <typename From, typename To> struct TCopyQualifiersFromTo { typedef                To Type; };
template <typename From, typename To> struct TCopyQualifiersFromTo<const          From, To> { typedef const          To Type; };
template <typename From, typename To> struct TCopyQualifiersFromTo<      volatile From, To> { typedef       volatile To Type; };
template <typename From, typename To> struct TCopyQualifiersFromTo<const volatile From, To> { typedef const volatile To Type; };

/**
* Copies the cv-qualifiers and references from one type to another, e.g.:
*
* TCopyQualifiersFromTo_T<const T1, T2> == const T2
* TCopyQualifiersFromTo_T<T1&, const T2> == const T2&
*/
template <typename From, typename To> struct TCopyQualifiersAndRefsFromTo               { using Type = typename TCopyQualifiersFromTo<From, To>::Type;   };
template <typename From, typename To> struct TCopyQualifiersAndRefsFromTo<From,   To& > { using Type = typename TCopyQualifiersFromTo<From, To>::Type&;  };
template <typename From, typename To> struct TCopyQualifiersAndRefsFromTo<From,   To&&> { using Type = typename TCopyQualifiersFromTo<From, To>::Type&&; };
template <typename From, typename To> struct TCopyQualifiersAndRefsFromTo<From&,  To  > { using Type = typename TCopyQualifiersFromTo<From, To>::Type&;  };
template <typename From, typename To> struct TCopyQualifiersAndRefsFromTo<From&,  To& > { using Type = typename TCopyQualifiersFromTo<From, To>::Type&;  };
template <typename From, typename To> struct TCopyQualifiersAndRefsFromTo<From&,  To&&> { using Type = typename TCopyQualifiersFromTo<From, To>::Type&;  };
template <typename From, typename To> struct TCopyQualifiersAndRefsFromTo<From&&, To  > { using Type = typename TCopyQualifiersFromTo<From, To>::Type&&; };
template <typename From, typename To> struct TCopyQualifiersAndRefsFromTo<From&&, To& > { using Type = typename TCopyQualifiersFromTo<From, To>::Type&;  };
template <typename From, typename To> struct TCopyQualifiersAndRefsFromTo<From&&, To&&> { using Type = typename TCopyQualifiersFromTo<From, To>::Type&&; };

template <typename From, typename To>
using TCopyQualifiersAndRefsFromTo_T = typename TCopyQualifiersAndRefsFromTo<From, To>::Type;

/**
* ForwardAsBase will cast a reference to an rvalue reference of a base type.
* This allows the perfect forwarding of a reference as a base class.
*/
template <
	typename T,
	typename Base,
	decltype(ImplicitConv<const volatile Base*>((typename TRemoveReference<T>::Type*)nullptr))* = nullptr
>
decltype(auto) ForwardAsBase(typename TRemoveReference<T>::Type& Obj)
{
	return (TCopyQualifiersAndRefsFromTo_T<T&&, Base>)Obj;
}

template <
	typename T,
	typename Base,
	decltype(ImplicitConv<const volatile Base*>((typename TRemoveReference<T>::Type*)nullptr))* = nullptr
>
decltype(auto) ForwardAsBase(typename TRemoveReference<T>::Type&& Obj)
{
	return (TCopyQualifiersAndRefsFromTo_T<T&&, Base>)Obj;
}

/**
* Does a boolean AND of the ::Value static members of each type, but short-circuits if any Type::Value == false.
*/
template <typename... Types>
struct TAnd;

template <bool LHSValue, typename... RHS>
struct TAndValue
{
	static constexpr bool Value = TAnd<RHS...>::value;
	static constexpr bool value = TAnd<RHS...>::value;
};

template <typename... RHS>
struct TAndValue<false, RHS...>
{
	static constexpr bool Value = false;
	static constexpr bool value = false;
};

template <typename LHS, typename... RHS>
struct TAnd<LHS, RHS...> : TAndValue<LHS::Value, RHS...>
{
};

template <>
struct TAnd<>
{
	static constexpr bool Value = true;
	static constexpr bool value = true;
};

/**
* Does a boolean OR of the ::Value static members of each type, but short-circuits if any Type::Value == true.
*/
template <typename... Types>
struct TOr;

template <bool LHSValue, typename... RHS>
struct TOrValue
{
	static constexpr bool Value = TOr<RHS...>::value;
	static constexpr bool value = TOr<RHS...>::value;
};

template <typename... RHS>
struct TOrValue<true, RHS...>
{
	static constexpr bool Value = true;
	static constexpr bool value = true;
};

template <typename LHS, typename... RHS>
struct TOr<LHS, RHS...> : TOrValue<LHS::Value, RHS...>
{
};

template <>
struct TOr<>
{
	static constexpr bool Value = false;
	static constexpr bool value = false;
};

/**
* Does a boolean NOT of the ::Value static members of the type.
*/
template <typename Type>
struct TNot
{
	static constexpr bool Value = !Type::Value;
	static constexpr bool value = !Type::Value;
};

/**
* TIsReferenceType
*/
template<typename T> struct TIsReferenceType      { enum { Value = false }; };
template<typename T> struct TIsReferenceType<T&>  { enum { Value = true  }; };
template<typename T> struct TIsReferenceType<T&&> { enum { Value = true  }; };

/**
* TIsLValueReferenceType
*/
template<typename T> struct TIsLValueReferenceType     { enum { Value = false }; };
template<typename T> struct TIsLValueReferenceType<T&> { enum { Value = true  }; };

/**
* TIsRValueReferenceType
*/
template<typename T> struct TIsRValueReferenceType      { enum { Value = false }; };
template<typename T> struct TIsRValueReferenceType<T&&> { enum { Value = true  }; };

template <typename T>
T&& DeclVal();