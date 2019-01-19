#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "json_parser.h"

#define VERSION "0.0.1"

void usage(char *name);

int
main(int argc, char **argv)
{
    int option_index = 0;
    char path[4096] = "";
    char c;
    int fd = -1;
    struct stat stabuf;
    static struct option long_options[] =
    {
        {"config",  required_argument, 0,  0 },
        {"version", no_argument,       0,  0 },
        {0,         0,                 0,  0 }
    };

    if (argc == 1)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        c = getopt_long(argc, argv, "c:v", long_options, &option_index);
        if (c == -1)
            break;

        switch (c)
        {
            case 'v':
                printf("version: %s\n", VERSION);
                exit(EXIT_SUCCESS);

            case 'c':
                strncpy(path, optarg, sizeof(path));
                break;

            default:
                usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }

    printf("Configuration file: %s\n", path);

    if (access(path, F_OK) == -1)
    {
        perror(path);
        exit(EXIT_FAILURE);
    }

    Mqtt_Conf mqtt;
    Ttn_Conf ttn;

    memset(&mqtt, 0, sizeof(Mqtt_Conf));
    memset(&ttn, 0, sizeof(Ttn_Conf));

    if(parse_configuration(path, &mqtt, &ttn))
    {
        printf("Error parsing configuration, exiting\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void
usage(char *name)
{
    printf("Usage: %s [-c config_file.json] [-v]\n", name);
}

