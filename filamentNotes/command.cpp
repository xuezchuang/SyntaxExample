#include "filamentNote.h"
#include <iostream>
//#include "command.h"
#include <tuple>

#pragma region Test__Command
#define EXPAND(x) x

#define APPLY0(M,...)
#define APPLY1(M, A, ...) EXPAND(M(A))
#define APPLY2(M, A, ...) EXPAND(M(A)), EXPAND(APPLY1(M, __VA_ARGS__))
#define APPLY3(M, A, ...) EXPAND(M(A)), EXPAND(APPLY2(M, __VA_ARGS__))
#define APPLY4(M, A, ...) EXPAND(M(A)), EXPAND(APPLY3(M, __VA_ARGS__))
#define APPLY5(M, A, ...) EXPAND(M(A)), EXPAND(APPLY4(M, __VA_ARGS__))
#define APPLY6(M, A, ...) EXPAND(M(A)), EXPAND(APPLY5(M, __VA_ARGS__))
#define APPLY7(M, A, ...) EXPAND(M(A)), EXPAND(APPLY6(M, __VA_ARGS__))
#define APPLY8(M, A, ...) EXPAND(M(A)), EXPAND(APPLY7(M, __VA_ARGS__))
#define APPLY9(M, A, ...) EXPAND(M(A)), EXPAND(APPLY8(M, __VA_ARGS__))
#define APPLY10(M, A, ...) EXPAND(M(A)), EXPAND(APPLY9(M, __VA_ARGS__))
#define APPLY11(M, A, ...) EXPAND(M(A)), EXPAND(APPLY10(M, __VA_ARGS__))
#define APPLY12(M, A, ...) EXPAND(M(A)), EXPAND(APPLY11(M, __VA_ARGS__))
#define APPLY_N__(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, X, ...) APPLY##X
#define APPLY(M, ...) EXPAND(EXPAND(APPLY_N__(M, __VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))(M, __VA_ARGS__))

//using namespace snow;

class Driver
{
public:
	Driver() = default;
	virtual ~Driver() noexcept;
	virtual void t1(int a ,int b) {}
	virtual void t2(int n) {}

};

class OpenglDriver
{
public:
	OpenglDriver() = default;
	virtual ~OpenglDriver() noexcept;

};

template<typename... ARGS>
struct CommandType2;

template<typename... ARGS>
struct CommandType2<void (Driver::*)(ARGS...)>
{
	template<void(Driver::*)(ARGS...)>
	class Command
	{
		// We use a std::tuple<> to record the arguments passed to the constructor
		using SavedParameters = std::tuple<std::remove_reference_t<ARGS>...>;
		SavedParameters mArgs;
	public:
		template<typename... A>
		inline explicit constexpr Command(A&& ... args)
			:mArgs(std::forward<A>(args)...) {
		}
	};
};

//#define COMMAND_TYPE(method) CommandType2<&Driver::method>::Command<&Driver::method>
#define COMMAND_TYPE(method) CommandType2<decltype(&Driver::method)>::Command<&Driver::method>

void TestCommand()
{
	using cmd = COMMAND_TYPE(t1);
	int a = 0; int b = 0;
	void* p = new cmd(std::move(a), std::move(b));
	int aa = 0;
}
#pragma endregion Test__Command


#pragma region TemplateConvertBase
class HandleBase
{
public:
	using HandleId = uint32_t;
	constexpr HandleBase() noexcept : object(0) {}
protected:
	HandleBase(HandleBase const& rhs) noexcept = default;
	HandleBase& operator=(HandleBase const& rhs) noexcept = default;

private:
	HandleId object;
};
template<typename T>
struct Handle : public HandleBase
{

	Handle() noexcept = default;
	template<typename B, typename = std::enable_if_t<std::is_base_of<T, B>::value> >
	Handle(Handle<B> const& base) noexcept : HandleBase(base) { }
};

template<typename D, typename ... ARGS>
Handle<D> initHandle(ARGS&& ... args) noexcept
{
	Handle<D> h;
	return h;
};

struct HwBufferObject
{
	int a;
};

#include <vector>
struct GLBufferObject : public HwBufferObject
{
	int b;
};
void TemplateConvertBase()
{
	std::vector<int> aVec;
	aVec.push_back(3);
	//aVec.
	Handle<HwBufferObject> vbo = initHandle<GLBufferObject>();
	int a = 0;
}
#pragma endregion TemplateConvertBase

