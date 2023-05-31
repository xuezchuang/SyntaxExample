#include "filamentNote.h"
#include <iostream>
//#include "command.h"
#include <tuple>

#pragma region Test__Command
//using namespace snow;

class Driver
{
public:
	Driver() = default;
	virtual ~Driver() noexcept;
	virtual void t1() {}
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
		//inline Command(Command&& rhs) noexcept = default;
	};
};

//#define COMMAND_TYPE(method) CommandType2<&Driver::method>::Command<&Driver::method>
#define COMMAND_TYPE(method) CommandType2<decltype(&Driver::method)>::Command<&Driver::method>

void TestCommand()
{
	using cmd = COMMAND_TYPE(t1);
	//cmd c;
	int a = 0;
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

struct GLBufferObject : public HwBufferObject
{
	int b;
};
void TemplateConvertBase()
{
	Handle<HwBufferObject> vbo = initHandle<GLBufferObject>();
	int a = 0;
}
#pragma endregion TemplateConvertBase

