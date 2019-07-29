#include "winglib.h"
#include "../glib.h"
#if _MSC_VER
struct GString * bnGStringAppendC(struct GString * str, char c) {
	const char* a[2] = {
		c, '\0'
	};
	g_string_append(str, a);
	return str;
}
#endif