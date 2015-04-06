#include "render.h"
#include "console.h"
#include "math/sm4/sm4.h"
using namespace bazinga;

void render::init () {
	console << LINEINFO << outline;

	sm4 mat = sm4::translate({10, 0, 0});
	vec4 vec = vec4{10, 10, 10, 1};
	vec4 vec2 = mat*vec;

	console << vec.str() << outline;
	console << mat.str() << outline;
	console << vec2.str() << outline;
}

void render::deinit () {
	console << LINEINFO << outline;
}