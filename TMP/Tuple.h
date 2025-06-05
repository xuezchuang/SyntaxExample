#pragma once
#include <..\..\14.34.31933\include\type_traits>

typedef unsigned int		uint32;

#define UE_CONSTRAINTS_BEGIN , std::enable_if_t<
#define UE_CONSTRAINT(...) (__VA_ARGS__) &&
#define UE_CONSTRAINTS_END true, int> = 0

template <typename T, T... Indices>
struct TIntegerSequence
{
};

template <typename T, T N>
using TMakeIntegerSequence = __make_integer_seq<TIntegerSequence, T, N>;



namespace UE::Core::Private::Tuple
{
	enum EForwardingConstructor { ForwardingConstructor };
	enum EOtherTupleConstructor { OtherTupleConstructor };

	template <typename... ArgTypes>
	void ConceptCheckingHelper(ArgTypes&&...);

	template <typename T, typename... Types>
	constexpr uint32 TTypeCountInParameterPack_V = 0;

	template <typename T, typename U, typename... Types>
	constexpr uint32 TTypeCountInParameterPack_V<T, U, Types...> = TTypeCountInParameterPack_V<T, Types...> +(std::is_same_v<T, U> ? 1 : 0);


	template <typename T, uint32 Index, uint32 TupleSize>
	struct TTupleBaseElement
	{
		template <
			typename ArgType
			UE_CONSTRAINTS_BEGIN
			UE_CONSTRAINT(std::is_constructible_v<T, ArgType&&>)
			UE_CONSTRAINTS_END
		>
		explicit TTupleBaseElement(EForwardingConstructor, ArgType&& Arg)
			: Value(Forward<ArgType>(Arg))
		{
		}

		TTupleBaseElement()
			: Value()
		{
		}

		TTupleBaseElement(TTupleBaseElement&&) = default;
		TTupleBaseElement(const TTupleBaseElement&) = default;
		TTupleBaseElement& operator=(TTupleBaseElement&&) = default;
		TTupleBaseElement& operator=(const TTupleBaseElement&) = default;

		T Value;
	};

	template <typename T>
	struct TTupleBaseElement<T, 0, 2>
	{
		template <
			typename ArgType
			UE_CONSTRAINTS_BEGIN
			UE_CONSTRAINT(std::is_constructible_v<T, ArgType&&>)
			UE_CONSTRAINTS_END
		>
		explicit TTupleBaseElement(EForwardingConstructor, ArgType&& Arg)
			: Key(Forward<ArgType>(Arg))
		{
		}

		TTupleBaseElement()
			: Key()
		{
		}

		TTupleBaseElement(TTupleBaseElement&&) = default;
		TTupleBaseElement(const TTupleBaseElement&) = default;
		TTupleBaseElement& operator=(TTupleBaseElement&&) = default;
		TTupleBaseElement& operator=(const TTupleBaseElement&) = default;

		T Key;
	};

	template <uint32 Index, uint32 TupleSize>
	struct TTupleElementGetterByIndex
	{
		template <typename DeducedType, typename TupleType>
		static decltype(auto) GetImpl(const volatile TTupleBaseElement<DeducedType, Index, TupleSize>&, TupleType&& Tuple)
		{
			// Brackets are important here - we want a reference type to be returned, not object type
			decltype(auto) Result = (ForwardAsBase<TupleType, TTupleBaseElement<DeducedType, Index, TupleSize>>(Tuple).Value);

			// Keep tuple rvalue references to rvalue reference elements as rvalues, because that's how std::get() works, not how C++ struct member access works.
			return static_cast<std::conditional_t<TAnd<TNot<TIsReferenceType<TupleType>>, TIsRValueReferenceType<DeducedType>>::Value, DeducedType, decltype(Result)>>(Result);
		}

		template <typename TupleType>
		static decltype(auto) Get(TupleType&& Tuple)
		{
			return GetImpl(Tuple, Forward<TupleType>(Tuple));
		}
	};

	template <typename Type, uint32 TupleSize>
	struct TTupleElementGetterByType
	{
		template <uint32 DeducedIndex, typename TupleType>
		static decltype(auto) GetImpl(const volatile TTupleBaseElement<Type, DeducedIndex, TupleSize>&, TupleType&& Tuple)
		{
			return TTupleElementGetterByIndex<DeducedIndex, TupleSize>::Get(Forward<TupleType>(Tuple));
		}

		template <typename TupleType>
		static decltype(auto) Get(TupleType&& Tuple)
		{
			return GetImpl(Tuple, Forward<TupleType>(Tuple));
		}
	};

	template <typename Indices, typename... Types>
	struct TTupleBase;

	template <uint32... Indices, typename... Types>
	struct TTupleBase<TIntegerSequence<uint32, Indices...>, Types...> : TTupleBaseElement<Types, Indices, sizeof...(Types)>...
	{
		template <
			typename... ArgTypes,
			decltype(ConceptCheckingHelper(TTupleBaseElement<Types, Indices, sizeof...(Types)>(ForwardingConstructor, DeclVal<ArgTypes&&>())...))* = nullptr
		>
		explicit TTupleBase(EForwardingConstructor, ArgTypes&&... Args)
			: TTupleBaseElement<Types, Indices, sizeof...(Types)>(ForwardingConstructor, Forward<ArgTypes>(Args))...
		{
		}

		template <
			typename TupleType,
			decltype(ConceptCheckingHelper(TTupleBaseElement<Types, Indices, sizeof...(Types)>(ForwardingConstructor, DeclVal<TupleType&&>().template Get<Indices>())...))* = nullptr
		>
		explicit TTupleBase(EOtherTupleConstructor, TupleType&& Other)
			: TTupleBaseElement<Types, Indices, sizeof...(Types)>(ForwardingConstructor, Forward<TupleType>(Other).template Get<Indices>())...
		{
		}

		TTupleBase() = default;
		TTupleBase(TTupleBase&& Other) = default;
		TTupleBase(const TTupleBase& Other) = default;
		TTupleBase& operator=(TTupleBase&& Other) = default;
		TTupleBase& operator=(const TTupleBase& Other) = default;



		template <uint32 Index> 
		decltype(auto) Get()& 
		{ 
			static_assert(Index < sizeof...(Types), "Invalid index passed to TTuple::Get"); 
			return TTupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<TTupleBase&>(*this));
		}
		template <uint32 Index> 
		decltype(auto) Get() const& 
		{ 
			static_assert(Index < sizeof...(Types), "Invalid index passed to TTuple::Get");
			return TTupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<const          TTupleBase&>(*this)); 
		}
		template <uint32 Index> 
		decltype(auto) Get()       volatile& 
		{ 
			static_assert(Index < sizeof...(Types), "Invalid index passed to TTuple::Get"); 
			return TTupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<volatile TTupleBase&>(*this)); 
		}
		template <uint32 Index> 
		decltype(auto) Get() const volatile&
		{ 
			static_assert(Index < sizeof...(Types), "Invalid index passed to TTuple::Get");
			return TTupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<const volatile TTupleBase&>(*this)); 
		}
		template <uint32 Index> 
		decltype(auto) Get()&& 
		{ 
			static_assert(Index < sizeof...(Types), "Invalid index passed to TTuple::Get"); 
			return TTupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<TTupleBase&&>(*this)); 
		}
		template <uint32 Index> 
		decltype(auto) Get() const&&
		{
			static_assert(Index < sizeof...(Types), "Invalid index passed to TTuple::Get"); 
			return TTupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<const          TTupleBase&&>(*this));
		}
		template <uint32 Index> 
		decltype(auto) Get()       volatile&&
		{
			static_assert(Index < sizeof...(Types), "Invalid index passed to TTuple::Get");
			return TTupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<volatile TTupleBase&&>(*this));
		}
		template <uint32 Index> 
		decltype(auto) Get() const volatile&& 
		{
			static_assert(Index < sizeof...(Types), "Invalid index passed to TTuple::Get");
			return TTupleElementGetterByIndex<Index, sizeof...(Types)>::Get(static_cast<const volatile TTupleBase&&>(*this));
		}

		template <typename T> 
		decltype(auto) Get()& 
		{ 
			static_assert(TTypeCountInParameterPack_V<T, Types...> == 1, "Invalid type passed to TTuple::Get"); 
			return TTupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<TTupleBase&>(*this)); 
		}
		template <typename T> 
		decltype(auto) Get() const& { static_assert(TTypeCountInParameterPack_V<T, Types...> == 1, "Invalid type passed to TTuple::Get"); return TTupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<const          TTupleBase&>(*this)); }
		template <typename T> 
		decltype(auto) Get()       volatile&
		{ 
			static_assert(TTypeCountInParameterPack_V<T, Types...> == 1, "Invalid type passed to TTuple::Get");
			return TTupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<volatile TTupleBase&>(*this));
		}
		template <typename T> 
		decltype(auto) Get() const volatile& 
		{ 
			static_assert(TTypeCountInParameterPack_V<T, Types...> == 1, "Invalid type passed to TTuple::Get");
			return TTupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<const volatile TTupleBase&>(*this));
		}
		template <typename T> 
		decltype(auto) Get()&&
		{ 
			static_assert(TTypeCountInParameterPack_V<T, Types...> == 1, "Invalid type passed to TTuple::Get"); 
			return TTupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<TTupleBase&&>(*this)); 
		}
		template <typename T> 
		decltype(auto) Get() const&&
		{ 
			static_assert(TTypeCountInParameterPack_V<T, Types...> == 1, "Invalid type passed to TTuple::Get");
			return TTupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<const          TTupleBase&&>(*this)); 
		}
		template <typename T> 
		decltype(auto) Get()       volatile&& 
		{ 
			static_assert(TTypeCountInParameterPack_V<T, Types...> == 1, "Invalid type passed to TTuple::Get");
			return TTupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<volatile TTupleBase&&>(*this)); 
		}
		template <typename T> 
		decltype(auto) Get() const volatile&& 
		{ 
			static_assert(TTypeCountInParameterPack_V<T, Types...> == 1, "Invalid type passed to TTuple::Get");
			return TTupleElementGetterByType<T, sizeof...(Types)>::Get(static_cast<const volatile TTupleBase&&>(*this)); 
		}


		template <typename FuncType, typename... ArgTypes>
		decltype(auto) ApplyAfter(FuncType&& Func, ArgTypes&&... Args)&
		{
			return ::Invoke(Func, Forward<ArgTypes>(Args)..., static_cast<TTupleBase&>(*this).template Get<Indices>()...);
		}
		template <typename FuncType, typename... ArgTypes>
		decltype(auto) ApplyAfter(FuncType&& Func, ArgTypes&&... Args) const&
		{
			//return ::Invoke(Func, Forward<ArgTypes>(Args)..., static_cast<const          TTupleBase&>(*this).template Get<Indices>()...);
			return ::Invoke(Func, Forward<ArgTypes>(Args)..., static_cast<const          TTupleBase& >(*this).template Get<Indices>()...);
		}
		template <typename FuncType, typename... ArgTypes>
		decltype(auto) ApplyAfter(FuncType&& Func, ArgTypes&&... Args)       volatile&
		{
			return ::Invoke(Func, Forward<ArgTypes>(Args)..., static_cast<volatile TTupleBase&>(*this).template Get<Indices>()...);
		}
		template <typename FuncType, typename... ArgTypes>
		decltype(auto) ApplyAfter(FuncType&& Func, ArgTypes&&... Args) const volatile&
		{
			return ::Invoke(Func, Forward<ArgTypes>(Args)..., static_cast<const volatile TTupleBase&>(*this).template Get<Indices>()...);
		}
		template <typename FuncType, typename... ArgTypes>
		decltype(auto) ApplyAfter(FuncType&& Func, ArgTypes&&... Args)&&
		{
			return ::Invoke(Func, Forward<ArgTypes>(Args)..., static_cast<TTupleBase&&>(*this).template Get<Indices>()...);
		}
		template <typename FuncType, typename... ArgTypes>
		decltype(auto) ApplyAfter(FuncType&& Func, ArgTypes&&... Args) const&&
		{
			return ::Invoke(Func, Forward<ArgTypes>(Args)..., static_cast<const TTupleBase&&>(*this).template Get<Indices>()...);
		}
		template <typename FuncType, typename... ArgTypes>
		decltype(auto) ApplyAfter(FuncType&& Func, ArgTypes&&... Args)       volatile&&
		{
			return ::Invoke(Func, Forward<ArgTypes>(Args)..., static_cast<volatile TTupleBase&&>(*this).template Get<Indices>()...);
		}
		template <typename FuncType, typename... ArgTypes>
		decltype(auto) ApplyAfter(FuncType&& Func, ArgTypes&&... Args) const volatile&&
		{
			return ::Invoke(Func, Forward<ArgTypes>(Args)..., static_cast<const volatile TTupleBase&&>(*this).template Get<Indices>()...);
		}

	};

	template <
		typename LhsType,
		typename RhsType,
		uint32... Indices,
		decltype(ConceptCheckingHelper((DeclVal<LhsType&>().template Get<Indices>() = DeclVal<RhsType&&>().template Get<Indices>(), 0)...))* = nullptr
	>
	static void Assign(LhsType& Lhs, RhsType&& Rhs, TIntegerSequence<uint32, Indices...>)
	{
		// This should be implemented with a fold expression when our compilers support it
		int Temp[] = { 0, (Lhs.template Get<Indices>() = Forward<RhsType>(Rhs).template Get<Indices>(), 0)... };
		(void)Temp;
	}

	template <typename... Given, typename... Deduced>
	std::enable_if_t<std::conjunction_v<std::is_constructible<Given, Deduced&&>...>> ConstructibleConceptCheck(Deduced&&...);

	template <typename... Given, typename... Deduced>
	decltype(ConceptCheckingHelper((DeclVal<Given>() = DeclVal<Deduced&&>(), 0)...)) AssignableConceptCheck(Deduced&&...);
}



template <typename... Types>
struct TTuple : UE::Core::Private::Tuple::TTupleBase<TMakeIntegerSequence<uint32, sizeof...(Types)>, Types...>
{
private:
	typedef UE::Core::Private::Tuple::TTupleBase<TMakeIntegerSequence<uint32, sizeof...(Types)>, Types...> Super;
public:

	template <
		typename... ArgTypes,
		decltype(UE::Core::Private::Tuple::ConstructibleConceptCheck<Types...>(DeclVal<ArgTypes&&>()...))* = nullptr
	>
	explicit(!std::conjunction_v<std::is_convertible<ArgTypes&&, Types>...>) TTuple(ArgTypes&&... Args)
		: Super(UE::Core::Private::Tuple::ForwardingConstructor, Forward<ArgTypes>(Args)...)
	{
	}


	template <
		typename... OtherTypes,
		decltype(UE::Core::Private::Tuple::ConstructibleConceptCheck<Types...>(DeclVal<OtherTypes&&>()...))* = nullptr
	>
	TTuple(TTuple<OtherTypes...>&& Other)
		: Super(UE::Core::Private::Tuple::OtherTupleConstructor, MoveTemp(Other))
	{
	}

	template <
		typename... OtherTypes,
		decltype(UE::Core::Private::Tuple::ConstructibleConceptCheck<Types...>(DeclVal<const OtherTypes&>()...))* = nullptr
	>
	TTuple(const TTuple<OtherTypes...>& Other)
		: Super(UE::Core::Private::Tuple::OtherTupleConstructor, Other)
	{
	}

	TTuple() = default;
	TTuple(TTuple&&) = default;
	TTuple(const TTuple&) = default;
	TTuple& operator=(TTuple&&) = default;
	TTuple& operator=(const TTuple&) = default;


	template <
		typename... OtherTypes,
		decltype(UE::Core::Private::Tuple::AssignableConceptCheck<Types&...>(DeclVal<const OtherTypes&>()...))* = nullptr
	>
	TTuple& operator=(const TTuple<OtherTypes...>& Other)
	{
		UE::Core::Private::Tuple::Assign(*this, Other, TMakeIntegerSequence<uint32, sizeof...(Types)>{});
		return *this;
	}

	template <
		typename... OtherTypes,
		decltype(UE::Core::Private::Tuple::AssignableConceptCheck<Types&...>(DeclVal<OtherTypes&&>()...))* = nullptr
	>
	TTuple& operator=(TTuple<OtherTypes...>&& Other)
	{
		UE::Core::Private::Tuple::Assign(*this, MoveTemp(Other), TMakeIntegerSequence<uint32, sizeof...(OtherTypes)>{});
		return *this;
	}
};