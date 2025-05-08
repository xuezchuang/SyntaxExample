// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once



template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T& > { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };

template <typename T> struct TRemovePointer { typedef T Type; };
template <typename T> struct TRemovePointer<T*> { typedef T Type; };

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


template <typename Functor, typename FuncType>
struct TFunctionRefCaller;

template <typename Functor, typename Ret, typename... ParamTypes>
struct TFunctionRefCaller<Functor, Ret(ParamTypes...)>
{
	static Ret Call(void* Obj, ParamTypes&... Params)
	{
		return Invoke(*(Functor*)Obj, Forward<ParamTypes>(Params)...);
	}
};

template <typename Functor, typename... ParamTypes>
struct TFunctionRefCaller<Functor, void(ParamTypes...)>
{
	static void Call(void* Obj, ParamTypes&... Params)
	{
		Invoke(*(Functor*)Obj, Forward<ParamTypes>(Params)...);
	}
};


namespace UE::Core::Private::Function
{
	template <typename StorageType, typename FuncType>
	struct TFunctionRefBase;

	template <typename StorageType, typename Ret, typename... ParamTypes>
	struct TFunctionRefBase<StorageType, Ret(ParamTypes...)>
	{
		TFunctionRefBase()
			: Callable(nullptr)
		{
		}

		TFunctionRefBase(TFunctionRefBase&& Other)
			: Callable(Other.Callable)
			//, Storage(MoveTemp(Other.Storage))
		{
			//if (Callable)
			//{
			//	Other.Callable = nullptr;
			//}
		}

		template <typename FunctorType>
		TFunctionRefBase(FunctorType&& InFunc)
		{
			if (auto* Binding = Storage.Bind(Forward<FunctorType>(InFunc)))
			{
				using DecayedFunctorType = typename TRemovePointer<decltype(Binding)>::Type;
				//using DecayedFunctorType = typename TRemoveReference<FunctorType>::Type;
				//using DecayedFunctorType = FunctorType;
				Callable = &TFunctionRefCaller<DecayedFunctorType, Ret(ParamTypes...)>::Call;
			}
		}

		Ret operator()(ParamTypes... Params) const
		{
			return Callable(Storage.GetPtr(), Params...);
		}
	private:
		// A pointer to a function which invokes the call operator on the callable object
		Ret(*Callable)(void*, ParamTypes&...) = nullptr;

		StorageType Storage;
	};


	struct FFunctionRefStoragePolicy
	{
		template <typename FunctorType>
		typename TRemoveReference<FunctorType>::Type* Bind(FunctorType&& InFunc)
		{
			Ptr = (void*)&InFunc;
			return &InFunc;
		}

		void* BindCopy(const FFunctionRefStoragePolicy& Other)
		{
			void* OtherPtr = Other.Ptr;
			Ptr = OtherPtr;
			return OtherPtr;
		}

		/**
		 * Returns a pointer to the callable object - needed by TFunctionRefBase.
		 */
		void* GetPtr() const
		{
			return Ptr;
		}

		/**
		 * Destroy any owned bindings - called by TFunctionRefBase only if Bind() or BindCopy() was called.
		 */
		void Unbind() const
		{
			// FunctionRefs don't own their binding - do nothing
		}

	private:
		// A pointer to the callable object
		void* Ptr = nullptr;
	};
}

// TEnableIf
template <bool Predicate, typename Result = void>
class TEnableIf;

template <typename Result>
class TEnableIf<true, Result>
{
public:
	using type = Result;
	using Type = Result;
};

template <typename Result>
class TEnableIf<false, Result>
{ };

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



template <typename FuncType>
class TFunctionRef : public UE::Core::Private::Function::TFunctionRefBase<UE::Core::Private::Function::FFunctionRefStoragePolicy, FuncType>
{
	using Super = UE::Core::Private::Function::TFunctionRefBase<UE::Core::Private::Function::FFunctionRefStoragePolicy, FuncType>;

public:
	/**
	 * Constructor which binds a TFunctionRef to a callable object.
	 */
	template <
		typename FunctorType
		//typename = typename TEnableIf<
		//TAnd<
		//TNot<TIsTFunctionRef<typename TDecay<FunctorType>::Type>>,
		//UE::Core::Private::Function::TFuncCanBindToFunctor<FuncType, typename TDecay<FunctorType>::Type>
		//>::Value
		//>::Type
	>
	TFunctionRef(FunctorType&& InFunc)
		: Super(Forward<FunctorType>(InFunc))
	{
		// This constructor is disabled for TFunctionRef types so it isn't incorrectly selected as copy/move constructors.
		int a = 0;
	}

	template <typename FunctorType>
	TFunctionRef(FunctorType& InFunc)
		: Super(Forward<FunctorType>(InFunc))
	{
		// This constructor is disabled for TFunctionRef types so it isn't incorrectly selected as copy/move constructors.
		int b = 0;
	}

	TFunctionRef(const TFunctionRef&) = default;

	// We delete the assignment operators because we don't want it to be confused with being related to
	// regular C++ reference assignment - i.e. calling the assignment operator of whatever the reference
	// is bound to - because that's not what TFunctionRef does, nor is it even capable of doing that.
	TFunctionRef& operator=(const TFunctionRef&) const = delete;
	~TFunctionRef() = default;
};


