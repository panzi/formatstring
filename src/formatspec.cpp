#include "formatstring/formatspec.h"

using namespace formatstring;

template<> const FormatSpec    FormatSpec::DEFAULT    = FormatSpec();
template<> const U16FormatSpec U16FormatSpec::DEFAULT = U16FormatSpec();
template<> const U32FormatSpec U32FormatSpec::DEFAULT = U32FormatSpec();
template<> const WFormatSpec   WFormatSpec::DEFAULT   = WFormatSpec();
