#include <napi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "dumpme.h"

Napi::Value dumpProcess(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  std::string gcore = info[0].ToString().Utf8Value();
  std::string target = info[1].ToString().Utf8Value();

  char command[4096];
  char buffer[255];

  if ((unsigned)snprintf(command, sizeof command, "%s -o %s %ld 2>&1", gcore.c_str(), target.c_str(), (long)getpid()) > sizeof command) {
    fprintf(stderr, "[dumpme] Specified command length exceeds system limit (%ld bytes)\n", sizeof command);
    return Napi::Boolean::New(env, false);
  }

  /*
   On linux kernels with hardening features, we need to authorize
   gcore to attach itself to the current PID
   */
  #ifdef PR_SET_PTRACER
    if (prctl(PR_SET_PTRACER, PR_SET_PTRACER_ANY, 0, 0, 0) != 0) {
      perror("Unable to get pattach permission from the kernel");
      return Napi::Boolean::New(env, false);
    }
  #endif

  FILE *fp = popen(command, "r");

  if (fp == NULL) {
    perror("[dumpme]");
    return Napi::Boolean::New(env, false);
  }

  while (fgets(buffer, 254, fp) != NULL) {
    printf("[dumpme] %s", buffer);
  }

  pclose(fp);

  #ifdef PR_SET_PTRACER
    if (prctl(PR_SET_PTRACER, 0, 0, 0, 0) != 0) {
      perror("Unable to revoke pattach permission");
      return Napi::Boolean::New(env, false);
    }
  #endif

  return Napi::Boolean::New(env, true);
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
  exports.Set("dumpProcess", Napi::Function::New(env, dumpProcess));
  return exports;
}

NODE_API_MODULE(dumpme, init)
