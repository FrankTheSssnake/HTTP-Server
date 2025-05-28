#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#include "../include/util.h"
#include "../include/server.h"

int main(int argc, char *argv[]) {
    
    int ipv4 = 1;
    int ipv6 = 0;
    int port = 8080;

    int c;
    while ((c = getopt (argc, argv, "46p:")) != -1) {
        switch (c) {
            case '4':
                ipv4 = 1;
                ipv6 = 0;
                break;

            case '6':
                ipv4 = 0;
                ipv6 = 1;
                break;

            case 'p':
                port = str_to_int(optarg);
                if (port == -1) {
                    fprintf(stderr, "Invalid Value provided for port: %s. Using default value `8080`\n", optarg); 
                    port = 8080;
                }
                break;

            case '?':
                if (optopt == 'p')
                    fprintf(stderr, "Option -%c  requires an argument.\n", optopt);
                else if (isprint(optopt))
                  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                  fprintf (stderr,
                           "Unknown option character `\\x%x'.\n",
                           optopt);
        }
    }
    
    start(ipv4, ipv6, port);
}
