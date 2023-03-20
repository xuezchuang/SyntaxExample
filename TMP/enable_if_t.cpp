#include <iostream>
#include <memory>
#include <random>
using namespace std;

template<typename T, typename E = void>
class uniform_distribution
{
	static_assert("kek i shrek bratya na vek");
	//std::uniform_int_distribution<T> distribution;
public:
	//uniform_distribution(T min, T mmax) {}
};

template<typename T>
class uniform_distribution<T, typename std::enable_if_t<std::is_floating_point_v<T>>>
{
	std::uniform_real_distribution<T> distribution;
public:
	uniform_distribution(T min, T max) : distribution{ min, max } {}
};

template<typename T>
class uniform_distribution<T, typename std::enable_if_t<std::is_integral_v<T>>>
{
	std::uniform_int_distribution<T> distribution;
public:
	uniform_distribution(T min, T max) : distribution{ min, max } {}
};


template<typename T>
class RangeDistribution
{
private:
	T min, max;
	mutable uniform_distribution<T> distribution;
public:
	RangeDistribution(const T& _min, const T& _max) noexcept
		: min(_min)
		, max(_max)
		, distribution(min,max)
	{

	}
	virtual ~RangeDistribution() = default;
};

int main() 
{
	//S s(1, 1.0f);     // match #2, output: 0
	RangeDistribution<int> abc(2.0,3.0);
}