#include <v8.h>
#include <node.h>
#include <nan.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "dumpme.h"

NAN_METHOD(dumpProcess) {
  v8::String::Utf8Value gcore(info[0]);
  v8::String::Utf8Value target(info[1]);
  char command[4096];
  char buffer[255];

  if ((unsigned)snprintf(command, sizeof command, "%s -o %s %ld 2>&1", *gcore, *target, (long)getpid()) > sizeof command) {
    fprintf(stderr, "[dumpme] Specified command length exceeds system limit (%ld bytes)\n", sizeof command);
    info.GetReturnValue().Set(false);
    return;
  }

  /*
   On linux kernels with hardening features, we need to authorize
   gcore to attach itself to the current PID
   */
  #ifdef PR_SET_PTRACER
    if (prctl(PR_SET_PTRACER, PR_SET_PTRACER_ANY, 0, 0, 0) != 0) {
      perror("Unable to get pattach permission from the kernel");
      info.GetReturnValue().Set(false);
      return;
    }
  #endif

  FILE *fp = popen(command, "r");

  if (fp == NULL) {
    perror("[dumpme]");
    info.GetReturnValue().Set(false);
    return;
  }

  while (fgets(buffer, 254, fp) != NULL) {
    printf("[dumpme] %s", buffer);
  }

  pclose(fp);

  #ifdef PR_SET_PTRACER
    if (prctl(PR_SET_PTRACER, 0, 0, 0, 0) != 0) {
      perror("Unable to revoke pattach permission");
      info.GetReturnValue().Set(false);
      return;
    }
  #endif

  info.GetReturnValue().Set(true);
}

NAN_MODULE_INIT(init) {
  Nan::Set(target, Nan::New("dumpProcess").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(dumpProcess)).ToLocalChecked());
}

NODE_MODULE(dumpme, init)
