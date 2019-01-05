
#include"Utility.h"




template <class DstType, class SrcType>
static bool IsType(const SrcType* src)
{
	return dynamic_cast<const DstType*>(src) != nullptr;
}










