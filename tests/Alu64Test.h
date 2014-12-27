#pragma once

#include "Test.h"
#include "MemoryFunction.h"

class CAlu64Test : public CTest
{
public:
	void				Run();
	void				Compile(Jitter::CJitter&);

private:
	struct CONTEXT
	{
		uint64			value0;
		uint64			value1;

		uint64			resultAdd;
		uint64			resultSub;
	};

	CONTEXT				m_context;
	CMemoryFunction		m_function;
};
