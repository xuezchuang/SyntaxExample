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
int main()
{
    Driver* pa = new glDriver();
    a _a(*pa);
    char* p = new char[22];
    new(p) a(*pa);

    
}
