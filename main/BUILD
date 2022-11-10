load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "main-lib",
    srcs = ["main.cc"],
    deps = [
        "@com_google_absl//absl/strings",
        "@raylib",
    ],
)

cc_binary(
    name = "boaf",
    deps = [
        ":main-lib",
    ],
)
