#include <string.h>
#include "checksafe.h"

extern "C"



JNIEXPORT jstring JNICALL
Java_com_fancy_checktrace_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    std::string hello = "Hello from C++";


    return env->NewStringUTF(hello.c_str());
}

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    UnionJNIEnvToVoid uenv;
    JNIEnv* env = NULL;
    LOGI("JNI_OnLoad!");

    if (vm->GetEnv((void**)&uenv.venv, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("ERROR: GetEnv failed");
        return -1;
    }
    pthread_create(&checkdebug,NULL,(void*(*)(void*))&readtrace2,NULL);

    pthread_detach(checkdebug);

    env = uenv.env;

    return JNI_VERSION_1_4;
}

void readtrace()
{
    FILE* fd;
    char context[128];
    char pidname[128];
    int pid=getpid();
    LOGI("pid is : %d ",pid);
    sprintf(pidname,"/proc/%d/status",pid);
    while (1) {
        fd = fopen(pidname, "r");
        while (fgets(context, 128, fd)) {
            if (strncmp(context, "TracerPid", 9)==0) {
                int status = atoi(&context[10]);
                LOGI("the process status is %d, %s", status, context);
                fclose(fd);
                syscall(__NR_close, fd);
                if (status != 0) {
                    LOGI("find the process was debugging");
                    kill(pid, SIGKILL);
                    return;
                }
                break;
            }
        }
        sleep(3);
    }
}





void readtrace2()
{
    FILE *fd;
    char filename[128];
    char line[128];
    int pid=getpid();
    LOGI("PID : %d", pid);
    sprintf(filename, "/proc/%d/status", pid);
    pid_t child=fork();
    if (child==0) {
        wait(NULL);
        long pt = ptrace(PTRACE_TRACEME, 0, 0, 0);
        logptrace("child traceme",pt);
        while (1) {
            fd = fopen(filename, "r");
            while (fgets(line, 128, fd)) {
                if (strncmp(line, "TracerPid", 9) == 0) {
                    int status = atoi(&line[10]);
                    LOGI("the process status is %d, %s", status, line);
                    fclose(fd);
                    syscall(__NR_close, fd);
                    if (status != 0) {
                        LOGI("find the process was debugging");
                        kill(pid, SIGKILL);
                        return;
                    }
                    break;
                }
            }
            sleep(3);
        }
    } else if(child>0){
        wait(NULL);
    }else{
        LOGE("fork error");
    }
}



void logptrace(char* func,long result)
{
    char* errormesg;
    if(result!=0){
        errormesg=strerror(errno);
        LOGE("%s ptrace error:%s",func,errormesg);
    }
    else{
        LOGI("%s ptrace result : %d",func, result);
    }
}