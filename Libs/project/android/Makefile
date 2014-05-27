Debug_GLUT: create_floders
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_System.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_DebugUtil.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Resource.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Graph.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_ThreadModel.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Text.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Shader.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Math.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_PostEffect.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Terra.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_UI.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Animation.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_SceneGraph.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Std.mk"
	cp ./obj/local/armeabi/lib*.a ../lib/

create_floders: 
	mkdir -p ../lib

clean: 
	rm -rf ./obj
	rm -rf ./libs
	rm -rf ../lib/lib*.a
