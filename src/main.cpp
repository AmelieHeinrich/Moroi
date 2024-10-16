// Copyright 2011 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "app.cpp"
#include "window.cpp"
#include "render_context.cpp"

int main()
{
    App app;

    emscripten_set_main_loop_arg(
		[](void *userData) {
			App& app = *reinterpret_cast<App*>(userData);
			app.OnFrame();
		},
		(void*)&app,
		0, true
	);

    return 0;
}