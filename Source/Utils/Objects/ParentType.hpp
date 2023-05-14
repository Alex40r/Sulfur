#pragma once

#include "Definitions.hpp"

#include "Parent.hpp"

template <class P>
class Utils::ParentType : public Static {
public:
	using Type = typename Utils::Parent<P>::Type;
};

