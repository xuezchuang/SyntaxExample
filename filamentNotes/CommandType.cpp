// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include <iostream>
#include <utility>
#include <tuple>

class Driver
{
public:
    Driver()
    {
        aa = 3;

    }

	void createRenderTargetR()
	{

	}
    int aa; 
};


class glDriver : public Driver
{
public:
    glDriver(){}

//private:
	void createRenderTargetR()
	{

    }

};



template<typename... ARGS>
struct CommandType;

template<typename... ARGS>
struct CommandType<void (Driver::*)(ARGS...)>
{

    template<void(Driver::*)(ARGS...)>
    class Command
    {
		using SavedParameters = std::tuple<std::remove_reference_t<ARGS>...>;
		SavedParameters mArgs;
    public:
        Command() {}
        
    };

};

// convert an method of "class Driver" into a Command<> type
#define COMMAND_TYPE(method) CommandType<decltype(&Driver::method)>::Command<&Driver::method>

class a
{
public:
    a(Driver& d):mDriver(d)
    {
        using Cmd = COMMAND_TYPE(createRenderTargetR);
        Cmd ab;
        int a = 0;
    }
    void* operator new(std::size_t size, void* ptr)
    {
        return ptr;
    }
protected:
private:
    Driver& __restrict mDriver;
};

#define EXPAND(x) x

#define PAIR_ARGS_0(M, ...)
#define PAIR_ARGS_1(M, X, Y, ...) M(X, Y)
#define PAIR_ARGS_2(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_1(M, __VA_ARGS__))
#define PAIR_ARGS_3(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_2(M, __VA_ARGS__))
#define PAIR_ARGS_4(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_3(M, __VA_ARGS__))
#define PAIR_ARGS_5(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_4(M, __VA_ARGS__))
#define PAIR_ARGS_6(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_5(M, __VA_ARGS__))
#define PAIR_ARGS_7(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_6(M, __VA_ARGS__))
#define PAIR_ARGS_8(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_7(M, __VA_ARGS__))
#define PAIR_ARGS_9(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_8(M, __VA_ARGS__))
#define PAIR_ARGS_10(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_9(M, __VA_ARGS__))
#define PAIR_ARGS_11(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_10(M, __VA_ARGS__))
#define PAIR_ARGS_12(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_11(M, __VA_ARGS__))

#define PAIR_ARGS_N__(_0, E1, _1, E2, _2, E3, _3, E4, _4, E5, _5, E6, _6, E7, _7, E8, _8, E9, _9, E10, _10, E11, _11, E12, _12, X, ...) PAIR_ARGS_##X

#define PAIR_ARGS_N(M, ...) \
    EXPAND(EXPAND(PAIR_ARGS_N__(0, ##__VA_ARGS__, 12, E, 11, E, 10, E, 9, E, 8, E, 7, E, 6, E, 5, E, 4, E, 3, E, 2, E, 1, E, 0))(M, __VA_ARGS__))

int main()
{
    Driver* pa = new glDriver();
    a _a(*pa);
    char* p = new char[22];
    new(p) a(*pa);

    
}
