#include <cstdio> // printf
#include <functional> // std::bind, std::mem_fn

class Test {
public:
	Test() {}
	void Func1(int a, int b) { printf("%d %d\n", a, b); }
};

template<int I>
class placeholder{};
placeholder<1> _1;
placeholder<2> _2;
placeholder<3> _3;

// argument list
template<typename A1, typename A2, typename A3>
class list3
{
public:
	list3(A1 a1, A2 a2, A3 a3) : a1_(a1), a2_(a2),
		a3_(a3) {}

	template<typename F, typename List>
	void operator()(F f, List list)
	{
		f(list[a1_], list[a2_], list[a3_]); // -- #3-a
	}

	A1 operator[](placeholder<1>) const { return a1_; }
	A2 operator[](placeholder<2>) const { return a2_; }
	A3 operator[](placeholder<3>) const { return a3_; }

	template<typename T>
	T operator[](T v) const { return v; }

private:
	A1 a1_;
	A2 a2_;
	A3 a3_;
};

template<typename A1>
class list1
{
public:
	list1(A1 a1) : a1_(a1) {}
	template<typename F, typename List>
	void operator()(F f, List list)
	{
		f(list[a1_]);
	}
	A1 operator[](placeholder<1>) const { return a1_; } // --#3-c
	template<typename T>
	T operator[](T v) const { return v; } // --#3-b
private:
	A1 a1_;
};
// rewritten function(Functor)
template<typename F, typename List>
class binder
{
public:
	binder(F f, List list) : f_(f), list_(list)
	{}
private:
	F f_;
	List list_;
public:
	template<typename A1>
	void operator()(A1 a1)
	{
		list1<A1> list(a1); // -- #2
		list_(f_, list);
	}
};

namespace hello {
// simple implementation - std::bind function
template<typename F, typename A1, typename A2, 
		 typename A3>
	binder<F, list3<A1,A2,A3> > bind(F f, A1 a1, A2 a2, 
									A3 a3)
{
	typedef typename list3<A1, A2, A3> list_type;
	list_type list(a1, a2, a3);
	return binder<F, list_type>(f, list); // -- #1
}

};

int main()
{
	Test t;
#if 1
	using namespace std::placeholders; // _1
	auto f =  std::bind(&Test::Func1, &t, 100, (std::placeholders::_1)); // a<-100
#else
	auto f =  hello::bind(std::mem_fn(&Test::Func1), &t, 100, _1); // a<-100
#endif
	f(50);

	// output:
	// 100 50
}