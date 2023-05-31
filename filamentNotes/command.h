#pragma once
#include <tuple>
namespace snow
{

	class Driver
	{
	public:
		Driver() = default;
		virtual ~Driver() noexcept;
		virtual void t1() {}

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
		};
	};

	#define COMMAND_TYPE(method) CommandType2<&Driver::method>::Command<&Driver::method>
}
//#define COMMAND_TYPE(method) CommandType<decltype(&Driver::method)>::Command<&Driver::method>

