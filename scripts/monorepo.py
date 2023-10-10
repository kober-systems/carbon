# import all sublibraries into one monorepo like repo
Import("env")

import os
from os import path

PATH_PREFIX=path.join(env["PROJECT_LIBDEPS_DIR"], env["PIOENV"], "carbon", "libraries")

env.Append(
  CPPPATH=[path.join(PATH_PREFIX, p) for p in os.listdir(PATH_PREFIX)]
)

