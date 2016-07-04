/*
 * libwebsockets-test-client - libwebsockets test implementation
 *
 * Copyright (C) 2011-2016 Andy Green <andy@warmcat.com>
 *
 * This file is made available under the Creative Commons CC0 1.0
 * Universal Public Domain Dedication.
 *
 * The person who associated a work with this deed has dedicated
 * the work to the public domain by waiving all of his or her rights
 * to the work worldwide under copyright law, including all related
 * and neighboring rights, to the extent allowed by law. You can copy,
 * modify, distribute and perform the work, even for commercial purposes,
 * all without asking permission.
 *
 * The test apps are intended to be adapted for use in your code, which
 * may be proprietary.  So unlike the library itself, they are licensed
 * Public Domain.
 */

#include "alconnclient.hpp"
#include <libwebsockets.h>

#include <getopt.h>
#include <iostream>

static int deny_deflate, deny_mux, longlived, mirror_lifetime;

static struct option options[] = {{"help", no_argument, NULL, 'h'},
                                  {"debug", required_argument, NULL, 'd'},
                                  {"port", required_argument, NULL, 'p'},
                                  {"ssl", no_argument, NULL, 's'},
                                  {"version", required_argument, NULL, 'v'},
                                  {"undeflated", no_argument, NULL, 'u'},
                                  {"nomux", no_argument, NULL, 'n'},
                                  {"longlived", no_argument, NULL, 'l'},
                                  {NULL, 0, 0, 0}};

int main(int argc, char **argv) {
  std::cout << "-zero" << std::endl;
  int n = 0, ret = 0, port = 7681, use_ssl = 0, ietf_version = -1;
  while (n >= 0) {

    // if (argc < 2) {
    //   // usage();
    // }
    // if (optind >= argc) {
    //   // usage();
    // }

    std::cout << "zero" << std::endl;
    n = getopt_long(argc, argv, "nuv:hsp:d:l", options, NULL);

    if (n < 0)
      continue;
    switch (n) {
    case 'd':
      lws_set_log_level(atoi(optarg), NULL);
      break;
    case 's':
      use_ssl = 2; /* 2 = allow selfsigned */
      break;
    case 'p':
      port = atoi(optarg);
      break;
    case 'l':
      longlived = 1;
      break;
    case 'v':
      ietf_version = atoi(optarg);
      break;
    case 'u':
      deny_deflate = 1;
      break;
    case 'n':
      deny_mux = 1;
      break;
    case 'h': {
      // usage();
    }
    }
  }
  std::cout << "one" << std::endl;
  AlConnClient cl;
  return 1;
}
