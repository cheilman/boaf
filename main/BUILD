load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "bird",
    srcs = ["bird.cc"],
    hdrs = ["bird.h"],
    deps = [
        "@com_google_absl//absl/log",
        "@com_google_absl//absl/strings:str_format",
        "@raylib",
    ],
)

cc_library(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        ":bird",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/log",
        "@com_google_absl//absl/log:flags",
        "@com_google_absl//absl/log:initialize",
        "@raylib",
    ],
)

cc_binary(
    name = "boaf",
    deps = [
        ":main",
    ],
)
