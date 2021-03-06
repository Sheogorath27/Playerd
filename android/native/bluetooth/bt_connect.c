#include <stdio.h>
#include <jni.h>
#include "org_copyleftgames_playerd_bluetooth_Helper.h"
#include <string.h>

char *classpath; //path to jar
JavaVM *libjvm;
JNIEnv *libenv;
jclass libcls;
jobject libobj;
int isStart = 0;

void vm_create()
{
	
	JavaVMInitArgs args;
	JavaVMOption opt[1];
	char *cp_classpath = "-Djava.class.path="; 
	int err;

	strcat( cp_classpath, classpath);
	opt[0].optionstring = cp_classpath;
	//opt[1].optionstring = "-verbose:jni";

	args.version = JNI_VERSION_1_6;
	args.nOptions = 1;
	atgs.options = opt;
	args.ignoreUnrecognized = JNI_TRUE;

	err = JNI_CreateJavaVM( libjvm, (void**)&libenv, &args);
	if (ret<0)
	{
		fprintf(stderr, "Unable to launch JVM\n");
		exit(1);
	}

	if (libenv == NULL)
	{
		fprintf(stderr, "Unable to create environment\n");
		exit(1);
	}

}

void init_class()
{	
	jmethodID initMethod;

	libcls = (*libenv)->FindClass(libenv, "org/copyleftgames/playerd/bluetooth/Helper")
	if (libcls == NULL)
	{
		fprintf(stderr, "Unable to find class\n");
		exit(1);
	}

	initMethod = (*libenv)->GetMethodID(libenv, libcls, "<init>", "()V");
	if (initMethod == NULL)
	{
		fprintf(stderr, "Constructor not found\n");
		exit(1);
	}

	libobj = (*libenv)->NewObject(libenv, libcls, initMethod);
	if (libobj == NULL)
	{
		fprintf(stderr, "Object not initialized\n");
		exit(1);
	}
}

//This method is required for creating JVM.
//Used for preventing unnecessary creation of JVM for each function call.
void startLibrary()
{
	vm_create();
	init_class();
	isStart = 1;
}

void enable_bt()
{
	jmethodID btenableMethod;

	if (!isStart)
		startLibrary();

	btenableMethod = (*libenv)->GetMethodID(libenv, libcls, "bt_enable", "()V");
	if (btenableMethod == NULL)
	{
		fprintf(stderr, "Enable function not found\n");
		exit(1);
	}
	(*libenv)->CallVoidMethod(libenv, libobj, btenableMethod);
}

void enable_discovery()
{
	jmethodID discEnableMethod;

	if (!isStart)
		startLibrary();

	discEnableMethod = (*libenv)->GetMethodID(libenv, libcls, "bt_discoverable", "()V");
	if (discEnableMethod == NULL)
	{
		fprintf(stderr, "Discoverable function not found.\n");
		exit(1);
	}
	(*libenv)->CallVoidMethod(libenv, libobj, discEnableMethod);
}

void search_device()
{
	jmethodID discoveryMethod;

	if (!isStart)
		startLibrary();

	discoveryMethod = (*libenv)->GetMethodID(libenv, libcls, "start_discovery", "()V");
	if (discoveryMethod == NULL)
	{
		fprintf(stderr, "Device discovery method not found.\n");
		exit(1);
	}
	(*libenv)->CallVoidMethod(libenv, libobj, discEnableMethod);

//todo Add proper interface with linked list.
}

void browse_sdp(const char *dvaddr)
{
	jmethodID browseMethod;
	jstring jstrdvaddr;

	if (!isStart)
		startLibrary();

	jstrdvaddr = (*env)->NewStringUTF(env, dvaddr);

	browseMethod = (*libenv)->GetMethodID(libenv, libcls, "start_sdp", "(Ljava/lang/String;)V");
	if (browseMethod == NULL)
	{
		fprintf(stderr, "Browse SDP method not found.\n");
		exit(1);
	}
	(*libenv)->CallVoidMethod(libenv, libobj, browseMethod, jstrdvaddr);

//todo add proper interface.	
}

void device_pair(const char *dvaddr)
{
	jmethodID pairMethod;
	jstring jstrdvaddr;

	if (!isStart)
		startLibrary();

	jstrdvaddr = (*env)->NewStringUTF(env, dvaddr);

	pairMethod = (*libenv)->GetMethodID(libenv, libcls, "start_pairing", "(Ljava/lang/String;)V");
	if (pairMethod == NULL)
	{
		fprintf(stderr, "Pairing method not found\n");
		exit(1);
	}
	(*libenv)->CallVoidMethod(libenv, libobj, pairMethod, jstrdvaddr);
}

//Native method to create UUID required.
//Native socket object required.
void connect_rfcomm