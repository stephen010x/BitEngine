

1. Efficient way to set shaders through shader variable positions.
2. Abstract all shader input and output


Remember compute shader.


I need a really tight way to handle shader variables.


To upload structs, in debug mode have the struct upload function enforce alignment to the largest member, if possible.
https://community.khronos.org/t/sending-an-array-of-structs-to-shader-via-an-uniform-buffer-object/75092



https://github.com/microsoft/DirectXShaderCompiler/blob/main/docs/SPIR-V.rst

https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml23
https://registry.khronos.org/vulkan/specs/1.1-extensions/html/vkspec.html#renderpass



Oooh. I should have the camera define the size and color type of the framebuffer
for efficiency, cameras who render to the screen shouldnt render to their own framebuffer, but instead directly to the screen framebuffer. I should hash camera properties, and if the properties change when the camera changes, then I modify the glx framebuffer to match those attributes

Also, glx supposedly doesn't support indexed color framebuffers anymore, and that the buffer should be truecolor, and handled by the shaders. So we will mainly be modifying the size of the buffer.
It is worth noting that the shaders work with fragments, so the bit size of the buffer probably doesnt actually matter for the actual processing. I think multiple passes are unavoidable, but if it is minimal shader processing, then it is supposedly low overhead.
But this makes me think that cameras using their own framebuffers is ultimately the way to go.
So I guess that is the play then. To each camera their own.



I should have cameras specify a size and location on the screen, and then just use stencil buffers to be able to draw multiple cameras to the screen based off that.

Just deallocate the buffers for any inactive camera.
