Debug_GLUT: create_floders
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_SampleKit.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_TimerThread.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_MouseHit.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Model.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_FontBitmap.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Shader.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_TextParagraph.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Archive.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_MotionBlend.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Decal.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Binding.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Motion.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_DebugMesh.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_FontFreeType.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Material.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_ProjectedTextureShadow.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_PostEffect.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_CubeMap.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Instancing.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_GamePad.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_WaterDrop.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_ScreenShot.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_State.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Task.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_MotionBind.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_FontRenderer.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Render2D.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_MotionInterp.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Event.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_ObjectReference.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_MesssageQueue.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Profiler.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_ResourceManager.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_JobQueue.mk"
	ndk-build NDK_PROJECT_PATH=${PWD} NDK_APPLICATION_MK="./Application_Parallel.mk"
	cp ./obj/local/armeabi/lib*.a ../lib/

create_floders: 
	mkdir -p ../lib

clean: 
	rm -rf ./obj
	rm -rf ./libs
	rm -rf ../lib/lib*.a
