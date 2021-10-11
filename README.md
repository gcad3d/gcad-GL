# gcad-GL
- is a hi-level library connecting CAD-applications with OpenGL
- written in C, under Gpl3 license.
- create objects by calling C-functions
- with attributes for each individual object:  
> persistance-attributes (permanent or temporay)  
> grafic-attributes (color or line thickness or line-stipple)  
> and operational-attributes (normal or hilited or dimmed);


#### OBJECTS:  
 - points,polygons  
 - surface-triangles  triangles, fan, stripe  
 - surface-indexed    indexed triangles, normals  
 - surface-textured   triangle-mesh, image  
 - symbols-2D         eg 2D-Text, dimensions, tag, balloon ..  
always normal to eyevector, rotated  
 - symbols-3D         eg vectors, arrow(head)s, axis, ..


#### ATTRIBUTES:  

###### persistance-attribute:

- permanent  - objects have DL-record
- dynamic    - have no DL-record, unlimited nr,  
	exist only until creation of next permanent object
- temporary  - have no DL-record, limited nr,
	can be created, remove individually


###### grafic-attributes:

- surfaces: ColRGB - color,texture,transparency;  
	style (shaded | symbolic)
- point,curves: Att_ln - color,thickness,stipple  

> grafic-attributes are stored in the DL,  
> can be modified for individual objects,  
> next redraw updates this objects


###### operational-attributes (only for permanent-objects):

  - normal | hilite | dimmed | hidden  
  - operational-attributes are stored in the DL
  - can be modified for individual objects,  
  	next redraw updates this objects


#### Modules of gcad-GL:
~~~
  GUI    user interface independent from drivers (Gtk, OpenGL)
         callbacks from mouse, keys, ..
    Gtk    menus, OpenGL-binding
    
  GR     grafic-functions independent from drivers (Gtk, OpenGL)
    GL     OpenGL V3.3 or V1
    
  DL     DisplayList (control of grafical attributes)
~~~


#### SHADERS:
- shadSUn  vertices  
           triangles (triangles, triangle-loops, triangle-fans)

- shadSUi  vertices + normals + indices  
           surfaces with boundaries

- shadSUt  vertices + texture-uv + indices  (textured quads)  
           surfaces with texture

- shadCV   vertices only  
           points, curves

- shadSY2  vertices (3D WCS)  
           2D-symbols (points, polygons, rotated)  
           text, dimensions, ballons, leaderlines, ..  
           tags - colored 2D-rectangles [with text]  
           images

- shadSY3  vertices (3D WCS)  
           3D-symbols (text, plane, axis, arrow[head], ..)


#### EXAMPLE:
~~~
	int GUI_CB_win_is_up () {
	  // window has started; display a triangle
	  GR_perm_sur (..);
	  // redraw all active objects
	  GR_redraw ();
	  return 0;
	}

	int GUI_CB_key (int iKey) {
	  // callback - user has pressed a key
	  if(iKey == GUI_KeyEsc) exit (0);
	  if(iKey == 't') {
	    // add permanent surface ..
	    GR_perm_sur (..);
	    GR_redraw ();
	  }
	  return 0;
	}
~~~
Full code see app.c


#### Build:
Prerequisites:  
gcc with tools (ctags)  

On debian-OS:  
sudo apt install libgtk-3-dev mesa-common-dev  

On redhat-OS:  
sudo yum install gtk3-devel mesa-libGL-devel  

- in console goto directory ~/gcad-GL/app/
- build list-tool ../bin/GUI_dlg1_gtk3 with:  
./GUI_dlg1.sh  
- build and execute demoprogram a.out with:  
make && ./a.out  
For using see Help-file using.txt and console-output  


#### TODO: (not yet implemented, help wanted)
- shaded_faces - geometry-shader ?  
    get angle normal-eyeVector = shading-factor
- selection of objects
- select-box (display box, select all objects in box)
- line-stipple (dashed, dashed-long, dash-dot)
- tesselate with shader (may not modify boundary - to be tested)
- perspectivic projection
- make shared overlay (libgcad-GL-gtk3-gl3.so)

#### Already working:
- orthographic projection with zoom, pan, rotate
- create points, curves, triangles, 2D-buttons
- change colors, width of curves, points
- hide / redisplay objects


#### History:
2021-10-11  dynamic-symbols, directory-structure, cursors, blocking ..
2021-09-10  First release.


~~~
Report bugs, improvements to support at gcad3d.org
~~~

// EOF
