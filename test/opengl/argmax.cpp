#include "Halide.h"
#include <stdio.h>

using namespace Halide;

int main() {
    // This test must be run with an OpenGL target.
    const Target target = get_jit_target_from_environment().with_feature(Target::OpenGL);

    const int width = 10, height = 10, channels = 3;
    Buffer<float> input(width, height, channels);

    Var x("x"), y("y"), c("c");

    Func g;
    RDom k(0, 2, "k");

    g(x, y, c) = cast<float>(argmax(k, input(x, y, k))[0]);

    g.bound(c, 0, 3).glsl(x, y, c);
    g.realize(width, height, channels, target); // Should not get any errors when compiling

    printf("Success!\n");
    return 0;
}
