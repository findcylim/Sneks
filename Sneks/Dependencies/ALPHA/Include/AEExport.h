/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AEExport.h

\author     Antoine Abi Chacra
\date       March 21, 2013

\brief      Header file for the library settings.

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef AE_EXPORT_H
#define AE_EXPORT_H

#ifndef EPSILON
	#define	EPSILON	0.00001f
#endif

#ifndef PI
	#define	PI		3.1415926f
#endif

#define	HALF_PI	(PI * 0.5f)
#define	TWO_PI	(PI * 2.0f)


#define EXPORT_WINDOWS					1
#define EXPORT_ANDROID					0

#if(1 == EXPORT_WINDOWS)

	#ifdef AE_FINAL
		#define PRINT(...)
	#else
		#define PRINT(...)	printf(__VA_ARGS__)
	#endif

	#ifdef AE_FINAL
		#define PRINT_INFO(...)
	#else
		#define PRINT_INFO(...)	PRINT("File: %s\nLine: %d\nFunc: %s\n\n",				\
											__FILE__,__LINE__, __FUNCTION__);	
	#endif



//	#ifdef AE_EXPORTS
		#define AE_API __declspec(dllexport)
//	#else
//		#define AE_API __declspec(dllimport)
		//#define AE_API
//	#endif

	#define DECLARE_FUNCTION_FOR_ANDROID(functionName)
	#define IMPLEMENT_FUNCTION_FOR_ANDROID(functionName)

	#define DECLARE_FUNCTION_FOR_ANDROID_2_INT(functionName, paramType1, paramType2)
	#define IMPLEMENT_FUNCTION_FOR_ANDROID_2_INT(functionName, paramType1, paramType2)							
	
#elif(1 == EXPORT_ANDROID)

	#include <jni.h>
	#include <android/log.h>
	#define AE_API 
	#define  LOG_TAG    "AlphaEngine-Lib"
	#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
	#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

	#ifdef AE_FINAL
		#define PRINT(...)
	#else
		#define PRINT(...)	LOGI(__VA_ARGS__)
	#endif



	#define DECLARE_FUNCTION_FOR_ANDROID(functionName) \
		JNIEXPORT void Java_com_example_alphaengine_AEGLRenderer_##functionName(JNIEnv * env, jobject obj);

	#define IMPLEMENT_FUNCTION_FOR_ANDROID(functionName) \
			JNIEXPORT void Java_com_example_alphaengine_AEGLRenderer_##functionName(JNIEnv * env, jobject obj)		\
			{		\
				functionName();		\
			}	


	#define DECLARE_FUNCTION_FOR_ANDROID_2_INT(functionName, paramType1, paramType2) \
			JNIEXPORT s32 Java_com_example_alphaengine_AEGLRenderer_##functionName(JNIEnv * env, jobject obj, paramType1, paramType2);
		
	#define IMPLEMENT_FUNCTION_FOR_ANDROID_2_INT(functionName, paramType1, paramType2) \
			JNIEXPORT s32 Java_com_example_alphaengine_AEGLRenderer_##functionName(JNIEnv * env, jobject obj, paramType1 param1, paramType2 param2)		\
			{		\
				return functionName(param1, param2);		\
			}

#endif



#endif		// File