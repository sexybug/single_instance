#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PID_FILE_DIR "/var/run/"

// 将该文件锁定
// 锁定后的文件将不能够再次锁定
int lockfile(int fileDescriptor){
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    fl.l_pid = getpid();  //当前进程号，可写可不写，写了后在用F_GETLK时则可以获取到pid
 
    int ret = fcntl(fileDescriptor, F_SETLK, &fl);
    //printf("file lock ret = %d\n", ret);
    return ret;
}

/**************************************************************************************************
**  功能描述:  判断系统中是否存在该进程
**  输入参数:  processName: 进程名,用于产生.pid文件名
**  返回参数:  返回1表示系统中已经存在该进程了；返回0表示系统中不存在该进程
**  注意事项:  此处加锁完后无需对文件进行close，而是进程退出后由系统来释放；否则无法起到保护的作用
**************************************************************************************************/
int isRunning(std::string processName){
    std::string pidFilePath="";
    pidFilePath=pidFilePath+PID_FILE_DIR+processName+".pid";
    //创建文件时如果指定了 O_CREAT，则必须添加权限，否则可能无法读取文件而导致失败
    int fileDescriptor=open(pidFilePath.c_str(),O_CREAT|O_RDWR,S_IRWXU);
    if(fileDescriptor<0){
        printf("open file %s failed !\n", pidFilePath.c_str());
        return 1;
    }
    int lockRet=lockfile(fileDescriptor);
    if(-1==lockRet){
        /*这里获取一下已经运行的实例的进程号*/
        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        if(fcntl(fileDescriptor, F_GETLK, &fl) == 0)
        {
            printf("%s is already running in another window, pid = %d\n", processName.c_str(), fl.l_pid);
        }
        //关闭当前进程的文件描述符
        close(fileDescriptor);
        return 1;
    }else{
        return 0;
    }
}
