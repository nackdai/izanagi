# Builds all the projects in the solution...
.PHONY: all_projects
all_projects: Debug_GLUT 

.PHONY: Debug_GLUT
Debug_GLUT: Graph_Debug_GLUT Math_Debug_GLUT Std_Debug_GLUT System_Debug_GLUT DebugUtil_Debug_GLUT Text_Debug_GLUT PostEffect_Debug_GLUT Shader_Debug_GLUT Resource_Debug_GLUT Animation_Debug_GLUT ThreadModel_Debug_GLUT SceneGraph_Debug_GLUT Terra_Debug_GLUT UI_Debug_GLUT 

# Builds project 'Graph'...
.PHONY: Graph_Debug_GLUT
Graph_Debug_GLUT: 
	make --directory="." --file=Graph.makefile Debug_GLUT

# Builds project 'Math'...
.PHONY: Math_Debug_GLUT
Math_Debug_GLUT: 
	make --directory="." --file=Math.makefile Debug_GLUT

# Builds project 'Std'...
.PHONY: Std_Debug_GLUT
Std_Debug_GLUT: 
	make --directory="." --file=Std.makefile Debug_GLUT

# Builds project 'System'...
.PHONY: System_Debug_GLUT
System_Debug_GLUT: 
	make --directory="." --file=System.makefile Debug_GLUT

# Builds project 'DebugUtil'...
.PHONY: DebugUtil_Debug_GLUT
DebugUtil_Debug_GLUT: 
	make --directory="." --file=DebugUtil.makefile Debug_GLUT

# Builds project 'Text'...
.PHONY: Text_Debug_GLUT
Text_Debug_GLUT: 
	make --directory="." --file=Text.makefile Debug_GLUT

# Builds project 'PostEffect'...
.PHONY: PostEffect_Debug_GLUT
PostEffect_Debug_GLUT: 
	make --directory="." --file=PostEffect.makefile Debug_GLUT

# Builds project 'Shader'...
.PHONY: Shader_Debug_GLUT
Shader_Debug_GLUT: 
	make --directory="." --file=Shader.makefile Debug_GLUT

# Builds project 'Resource'...
.PHONY: Resource_Debug_GLUT
Resource_Debug_GLUT: 
	make --directory="." --file=Resource.makefile Debug_GLUT

# Builds project 'Animation'...
.PHONY: Animation_Debug_GLUT
Animation_Debug_GLUT: 
	make --directory="." --file=Animation.makefile Debug_GLUT

# Builds project 'ThreadModel'...
.PHONY: ThreadModel_Debug_GLUT
ThreadModel_Debug_GLUT: 
	make --directory="." --file=ThreadModel.makefile Debug_GLUT

# Builds project 'SceneGraph'...
.PHONY: SceneGraph_Debug_GLUT
SceneGraph_Debug_GLUT: 
	make --directory="." --file=SceneGraph.makefile Debug_GLUT

# Builds project 'Terra'...
.PHONY: Terra_Debug_GLUT
Terra_Debug_GLUT: 
	make --directory="." --file=Terra.makefile Debug_GLUT

# Builds project 'UI'...
.PHONY: UI_Debug_GLUT
UI_Debug_GLUT: 
	make --directory="." --file=UI.makefile Debug_GLUT

# Cleans all projects...
.PHONY: clean
clean:
	make --directory="." --file=Graph.makefile clean
	make --directory="." --file=Math.makefile clean
	make --directory="." --file=Std.makefile clean
	make --directory="." --file=System.makefile clean
	make --directory="." --file=DebugUtil.makefile clean
	make --directory="." --file=Text.makefile clean
	make --directory="." --file=PostEffect.makefile clean
	make --directory="." --file=Shader.makefile clean
	make --directory="." --file=Resource.makefile clean
	make --directory="." --file=Animation.makefile clean
	make --directory="." --file=ThreadModel.makefile clean
	make --directory="." --file=SceneGraph.makefile clean
	make --directory="." --file=Terra.makefile clean
	make --directory="." --file=UI.makefile clean

