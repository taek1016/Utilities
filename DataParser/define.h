#ifndef __TAEK_DEFINE_H__
#define __TAEK_DEFINE_H__

#define BEGIN_UTILITY_NAMESPACE(name) namespace Taek { namespace Utility { namespace name {
#define END_NAMESPACE } } }

#define USING_TAEK_UTILITY_NAMESPACE(name) using namespace Taek::Utility::name
#define USING_NAMESPACE(name) using namespace name

#include <string>

typedef std::string str;

#endif // !__TAEK_DEFINE_H__