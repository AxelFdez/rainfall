#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    int num;
    char *str;
    gid_t egid;
    uid_t euid;

    num = atoi(argv[1]);

    if (num == 423) {
        str = strdup("/bin/sh");
        egid = getegid();
        euid = geteuid();
        setresgid(egid, egid, egid);
        setresuid(euid, euid, euid);
        execv(str, argv + 1);
        free(str);
    } else {
        const char *error_msg = "No !";
        fwrite(error_msg, 1, strlen(error_msg), stderr);
    }

    return 0;
}
