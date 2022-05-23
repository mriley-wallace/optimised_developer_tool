# Optimised Developer Tool: Farm Escape<br/>
---

There are two exectubales:<br/>
	Make sure you have the folders:<br/>
		*Dependancies<br/>
		*helper<br/>
		*media<br/>
		*shader<br/>
---

##Executable 1 (Main Scene with Particles)<br/>
This executable has changed from the original prototype.<br/>
---
This scene now contains smoke above the barn's chimney<br/>
![Alt Text](https://imgur.com/a/IB7n9oY.gif)
---

##Executable 2 (Edge Detection)<br/>
This executable outlines the edges of all the objects in the scene<br/>
![Alt Text](https://imgur.com/a/HzwJZL6.gif)<br/>

---
How it works?<br/>
The two executables were built by simply changing the "int sceneChanger = 0;" in the scenebasic_uniform.cpp<br/>
0 = Main Scene<br/>
1 = Edge Detection<br/>

---

##Program Code<br/>
###Main Scene Particles<br/>
The particles uses its own separate program called partProg to specify the shaders being used.<br/>
The particles use a vector 3 of gravity to specify where in the world space the end goal is (which is y -2)<br/>
They also contain parameters in the shaders for lifetime and size.<br/>
After rendering a grid, the shader calculates the quads for the offset position and texture coordinates.<br/>
Over a length of time (t) the angle of descent to Gravity corresponds to two pi.<br/>
The lookAt view was also extended backwards to show more of the terrain to see the whole barn.<br/>

---
###Edge Detection<br/>


---
## Unique<br/>
My shader program is special because it utilises the landscape around the scene to improve upon.<br/>
It also provides that next level of detail towards the ambience which helps bring everything alive. <br/>


---

##Start?<br/>
I started with the original program from the prototype which included all the 3D objects and the plane.<br/>
Since then I have added alot more shaders, refactored the code, created multiple programs to run the shaders,<br/>
as well as bug testing to ensure everything worked correctly.<br/>


##Video<br/>

• How does the program code work? How do the classes and functions fit together and who
does what?


