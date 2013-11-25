/** @file dox.h
 *  @brief General documentation built from a doxygen comment
 */

/**
@mainpage FGL - Fox Graphics Library

@section intro Introduction

FGL is high performance 3D graphics application framework based on OpenGL, SOIL (Simple OpenGL Image Library), Assimp, Freetype and OpenMesh.

FGL is aimed for VFP developers who interested in adding interactive 3D visualization features in their applications.

FGL supports:
- easy to use scene graph object-oriented interface
- user interaction (mouse, keyboard events)
- rendering performance optimization features like spatial culling, Level Of Details (LOD), vertex buffers and instancing
- material and light system
- text rendering with TrueType and OpenType fonts
- multiple viewports
- skeletal animation
- loading 3D models and textures in wide variety of formats, see <a href="http://assimp.sourceforge.net/main_features_formats.html">http://assimp.sourceforge.net/main_features_formats.html</a> and <a href="http://www.lonesock.net/soil.html">http://www.lonesock.net/soil.html</a>


@section main_install Installation

1) Download latest FGL version from <a href="http://fgl.codeplex.com/SourceControl/latest">http://fgl.codeplex.com/SourceControl/latest</a>

2) Copy file <b>fgl/build/fgl.fll</b> to the root folder of your project. 

3) Download and install "Microsoft Visual Studio C++ 2012 Redistributable Package" (or just run fgl/build/vcredist_x86.exe)

4) Copy <b>sample/_FGL</b> folder to the root folder of your project.
- fgl.fll : FGL API library
- _FGL/fgl.vcx : wrapper around fgl.fll
- _FGL/fglc.vcx : FGL controls library

@section main_usage Usage
1) Integrate library to your project

- Open FGL libraries<br>
SET LIBRARY TO "fgl.fll" ADDITIVE &&FGL API library<br>
SET CLASSLIB TO "_FGL/fgl.vcx" ADDITIVE &&wrapper around fgl.fll<br>
SET CLASSLIB TO "_FGL/fglc.vcx" ADDITIVE &&FGL controls library<br>
<br>

- Create new form and put <b>FGLC.fglc_main</b> container on it

2) Compile your project and try to execute it. If there is no any errors you can start to use FGL. 
Of cause, you can also take a look at samples applications in "samples" folder.


@section ext Extending the library
<br>

@section main_support Support & Feedback

If you have any questions or bug reports you are welcome to post them in <a href="http://fgl.codeplex.com/">http://fgl.codeplex.com/</a>.

@section todo Todo
- API documetation
- transparent materials rendering
- allow define custom shaders for materials
- layered textures
- bump mapping and parallax occlusion mapping
- heightmaps
*/

