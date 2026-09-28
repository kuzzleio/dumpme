'use strict';

// Loads the addon the way consumers do and runs it with `echo` standing in for
// gcore: no core file is written, but the whole native path runs (argument
// conversion, prctl on Linux, popen, return value).
//
// Only the type is asserted: on a Linux kernel without the Yama LSM (Docker
// Desktop, some CI hosts) PR_SET_PTRACER fails with EINVAL and dumpme returns
// false by design.
const assert = require('assert');
const dumpme = require('..');

assert.strictEqual(typeof dumpme, 'function');

const result = dumpme('echo', '/dev/null');
assert.strictEqual(typeof result, 'boolean');

console.log(`dumpme smoke test OK (returned ${result})`);
