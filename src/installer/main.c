/*
 * Tool intended to help facilitate the process of booting Linux on Intel
 * Macintosh computers made by Apple from a USB stick or similar.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Copyright (C) 2015 SevenBits
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>

static char *install_path;
static char *config_path;
static char *program_name; // Stores the name the program was invoked with

static bool should_verify;
static bool should_configure = true; // Whether or not to write data to the config file

void usage(char *prog_name) {
	printf("usage: %s [--verify] [--config file] path\n", prog_name);
	printf("\t--verify\t\tVerify that the installation is configured properly after setup\n");
	printf("\t--blank\t\t\tWrite an empty file\n");
	printf("\t--config file\t\tSpecifies the path to the configuration file\n");
}

bool handle_option(char *option, char **arg_ptr) {
	if (strcmp("--verify", option) == 0) {
		should_verify = true;
		return true;
	} else if (strcmp("--blank", option) == 0) {
		should_configure = false;
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
		usage(program_name);
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
			
			strcpy(install_path, *arg_ptr);
		}
		
		arg_ptr++; // Move and parse the next argument.
	}
}

bool is_directory(const char *path) {
	struct stat s;
	if (stat(path, &s) == 0) {
		if (s.st_mode & S_IFDIR) {
			return true;
		}
	}
	
	return false;
}

bool perform_setup() {
	// Get ready to copy the necessary files to the chosen path.
	if (!is_directory(install_path)) {
		fprintf(stderr, "Error: %s is not a directory.\n", install_path);
		return false;
	}

	return true;
}

int main(int argc, char **argv) {
	// Handle command line arguments
	program_name = strdup(*argv);
	parse_args(argc, argv);
	if (!perform_setup()) {
		return 1;
	}
	
	free(install_path);
	free(config_path);
}
