void __attribute__((CMSE_NS_ENTRY)) foo(uint32_t i)
{
	return i; //return value of foo()
}

void __attribute__((CMSE_NS_ENTRY)) goo(uint32_t i)
{
	return i; //return value of goo()
}