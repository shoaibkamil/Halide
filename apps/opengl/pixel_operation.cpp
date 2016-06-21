#include "../../build/include/Halide.h"
#include <stdio.h>
#include "../../src/Lerp.h"

using namespace Halide;
const int _number_of_channels = 4;

int main(int argc, char** argv)
{
	ImageParam input8(UInt(8), 3);
	ImageParam image_x(UInt(8), 3);

	input8
        .set_stride(0, _number_of_channels) // stride in dimension 0 (x) is three
        .set_stride(2, 1); // stride in dimension 2 (c) is one
    image_x
     	.set_stride(0, _number_of_channels) // stride in dimension 0 (x) is three
     	.set_stride(2, 1); // stride in dimension 2 (c) is one

    Var x("x"), y("y"), c("c");

	// algorithm
    Func input;
    input(x, y, c) = cast<float>(input8(clamp(x, input8.left(), input8.right()),
                                 clamp(y, input8.top(), input8.bottom()),
                                 clamp(c, 0, _number_of_channels))) / 255.0f;

	Expr x1("x1"), x2("x2"), x3("x3"), x4("x4");
	x1 = image_x(clamp(x, image_x.left(), image_x.right()), clamp(y, image_x.top(), image_x.bottom()), c);
	x2 = image_x(clamp(x + 1, image_x.left(), image_x.right()), clamp(y, image_x.top(), image_x.bottom()) , c);
	x3 = image_x(clamp(x, image_x.left(), image_x.right()), clamp(y + 1, image_x.top(), image_x.bottom()), c);
	x4 = image_x(clamp(x + 1, image_x.left(), image_x.right()), clamp(y + 1, image_x.top(), image_x.bottom()), c);
	Func average_x("average_x");
    Expr div("divisor");
	average_x(x, y, c) = cast<float>(x1 + x2 + x3 + x4) / (4.0f * 255.0f);

	Func pixel_operation;

	pixel_operation(x, y, c) = lerp(input(x, y, c), average_x(x, y, c), 0.3f);

	Func out;
	out(x, y, c) = cast<uint8_t>(pixel_operation(x, y, c) * 255.0f + 0.5f);
	out.output_buffer()
    	.set_stride(0, _number_of_channels)
    	.set_stride(2, 1);
    input8.set_bounds(2, 0, _number_of_channels); // Dimension 2 (c) starts at 0 and has extent _number_of_channels.
    image_x.set_bounds(2, 0, _number_of_channels);
    out.output_buffer().set_bounds(2, 0, _number_of_channels);

    // schedule

     out.compute_root();
     out.reorder(c, x, y)
	     .bound(c, 0, _number_of_channels)
	     .unroll(c);

    // Schedule for GLSL

    out.glsl(x, y, c);

    Target target = get_target_from_environment();
    target.set_feature(Target::OpenGL);
    target.set_feature(Target::Debug);

    std::vector<Argument> args = {input8, image_x};
    out.compile_to_file("pixel_operation", args, target);

	return 0;
}
