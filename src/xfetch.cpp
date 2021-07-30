#include <string.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <iostream>
#include <memory>
#include <cstdio>
#include "config.h"
std::string exec(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

const char* osName;


struct utsname buffer;

char* color = "\u001b[32m";

char* user(){ return getenv("USER");}
char* shell(){ char *shell = getenv("SHELL"); char *slash = strrchr(shell, '/'); if (slash) { shell = slash + 1;} return shell; }
void kernel(){ char *p; long ver[16]; int i=0; if (uname(&buffer) != 0) { perror("uname"); exit(EXIT_FAILURE); } }
char* uptime(){ FILE * uptimefile; char uptime_chr[28]; long uptime = 0; if((uptimefile = fopen("/proc/uptime", "r")) == NULL) perror("supt"), exit(EXIT_FAILURE); fgets(uptime_chr, 12, uptimefile); fclose(uptimefile); uptime = strtol(uptime_chr, NULL, 10); return (char*)std::to_string(uptime / 3600).c_str(); }
int GetRamInKB(void)
{
    FILE *meminfo = fopen("/proc/meminfo", "r");
    if(meminfo == NULL) return -1;
    char line[256];
    while(fgets(line, sizeof(line), meminfo))
    {
        int ram;
        if(sscanf(line, "MemTotal: %d kB", &ram) == 1)
        {
            fclose(meminfo);
            return ram;
        }
    }

    // If we got here, then we couldn't find the proper line in the meminfo file:
    // do something appropriate like return an error code, throw an exception, etc.
    fclose(meminfo);
    return -1;
}

int main(){
    kernel();
    osName = exec("awk -F= '$1==\"NAME\" { print $2 ;}' /etc/os-release").c_str();
    if(strcmp(osName, "Gentoo")) { color = "\u001b[35m"; } else if(strcmp(osName, "Ubuntu") || strcmp(osName, "Debian GNU/Linux")){ color = "\u001b[31m"; }  else if(strcmp(osName, "void")){ color = "\u001b[32m"; } else if(strcmp(osName, "Arch Linux")){ color = "\u001b[36;1m"; } else if(strcmp(osName, "Manjaro")){ color = "\u001b[32;1m"; }
    printf("%s%s%s \033[0;m%s", ascii[0], color, userText, user());
    printf("\n%s%s%s \033[0;m%s", ascii[1], color, osText, osName);
    printf("%s%s%s \033[0;m%s", ascii[2], color, kernelText, buffer.release);
    printf("\n%s%s%s \033[0;m%s", ascii[3], color, shellText, shell());
    printf("\n%s%s%s \033[0;m%s%s", ascii[4], color, uptimeText, uptime(), "h");
    printf("\n%s%s%s \033[0;m%s%s", ascii[5], color, ramText, std::to_string(GetRamInKB() / 1024).c_str(), "MiB \033[1;32m");
    printf("\n%s", ascii[6]);
    printf("\n%s", ascii[7]);
    return 0;
}
