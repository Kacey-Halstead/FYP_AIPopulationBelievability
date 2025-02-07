#include "FYP_AIBelievability.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif



void MainLoop(void* args)
{
	FYP_AIBelievability& program = *static_cast<FYP_AIBelievability*>(args);
	program.MainLoop();
}

int main(int argc, char** argv)
{
	FYP_AIBelievability program{};
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(MainLoop, &program, -1, 1);
#else
	while (program.MainLoop());
#endif
}