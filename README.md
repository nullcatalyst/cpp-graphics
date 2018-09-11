# cpp-graphics

This started as a C++ wrapper around Apple's [Metal API](https://developer.apple.com/documentation/metal), but then I wanted to expand it to also use [OpenGL](https://en.wikipedia.org/wiki/OpenGL) as a backend to compile the same application on other platforms with _(preferably)_ no code changes.

_NOTE: The OpenGL backend is **not** complete._

## Examples

I've included some examples to show usage. The `hello_metal` example must use the Xcode generator in order to compile the metal shaders. The `hello_macgl` example uses a `NSOpenGLView` for rendering, as [glfw3](https://www.glfw.org/) does not seem to create a view capable of rendering OpenGL on macOS mojave.
