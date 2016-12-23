'use strict';

const DumpProcess = require('bindings')('dumpme');

/**
 * Dumps the current process
 * @param {string} [gcore] - path and filename of the gcore binary
 * @param {string} [coredump] - path and filename of the target coredump file
 * @return {Boolean}
 */
function dump(gcore, coredump) {
  gcore = gcore || 'gcore';
  coredump = coredump || `${process.cwd()}/core.${process.pid}`;

  return DumpProcess.dumpProcess(gcore, coredump);
}

module.exports = dump;
