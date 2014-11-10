#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static char *install_path;
static char *config_path;

static bool should_verify;

void usage(char *prog_name) {
	printf("usage: %s [--verify] [--config file] path\n", prog_name);
	printf("\t--verify\t\tVerify that the installation is configuration properly after setup\n");
	printf("\t--config file\t\tSpecifies the path to the configuration file\n");
}

bool handle_option(char *option, char **arg_ptr) {
	if (strcmp("--verify", option) == 0) {
		should_verify = true;
		return true;
	} else if (strcmp("--config", option) == 0) {
		if (*(arg_ptr + 1) != 0) {
			if (**(arg_ptr + 1) == '-') {
				fprintf(stderr, "Error: --config expects argument\n");
				exit(1);
			}
			
			if (config_path) free(config_path);
			
			config_path = malloc(sizeof(char) * strlen(*(arg_ptr + 1)));
			if (!config_path) {
				fprintf(stderr, "Error: failed to allocate memory\n");
				exit(1);
			}
			
			strcpy(config_path, *(arg_ptr + 1));
			arg_ptr++;
		} else {
			fprintf(stderr, "Error: --config expects argument\n");
			exit(1);
		}
		
		return true;
	} else if (strcmp("--help", option) == 0) {
		usage("install-enterprise");
		exit(0);
	}
	
	return false;
}

void parse_args(int argc, char **argv) {
	if (argc < 2) {
		usage(*argv);
		exit(1);
	}
	
	char **arg_ptr = argv;
	while (arg_ptr != argv + argc) {
		if (**arg_ptr == '-') {
			// Parse an option.
			bool result = handle_option(*arg_ptr, arg_ptr);
			if (!result) {
				fprintf(stderr, "Error: unrecognized option: %s\n", *arg_ptr);
				exit(1);
			}
		} else {
			// We have the user's desired path
			if (install_path) free(install_path);
			
			install_path = malloc(sizeof(char) * strlen(*arg_ptr));
			if (!install_path) {
				fprintf(stderr, "Error: failed to allocate memory\n");
				exit(1);
			}
		}
		
		arg_ptr++; // Move and parse the next argument.
	}
}

int main(int argc, char **argv) {
	// Handle command line arguments
	parse_args(argc, argv);
	
	free(install_path);
	free(config_path);
}
