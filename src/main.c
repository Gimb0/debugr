#include "debugr.h"

int main()
{
	char *argv[1] = { 0 };
	dbg_inferior_exec("./test", argv);
}
