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
	cmd c;
	int a = 0;
}
#pragma endregion Test__Command