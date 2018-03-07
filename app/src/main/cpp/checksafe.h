//
// Created by fancy shen on 2018/3/4.
//

#ifndef CHECKTRACE_CHECKSAFE_H
#define CHECKTRACE_CHECKSAFE_H

#endif //CHECKTRACE_CHECKSAFE_H

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <string>
#include <sys/ptrace.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/inotify.h>
#include <sys/wait.h>
#include <sys/queue.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#define LOG_TAG "checksafe"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

pthread_t checkdebug;
pthread_t checkchild;


void readtrace();
void readtrace2();
void logptrace(char* func,long result);