#!/usr/bin/env bash

set -euo pipefail

# Run BUILD formatting
echo ">>>> RUNNING BUILDIFIER <<<<"
./bin/bazel run //:buildifier

# Run C++ formatting
echo ">>>> RUNNING C++ FORMATTER <<<<"
# TODO: Move this into bazel someday
find . -regex '.*\.\(cc\|h\)' -exec clang-format -style=file -i {} \;

# Run C++ linter
echo ">>>> RUNNING C++ LINTER <<<<"
./bin/bazel build //... --config clang-tidy

