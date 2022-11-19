FROM mcr.microsoft.com/devcontainers/base:ubuntu-20.04

RUN apt update \
    && DEBIAN_FRONTEND=noninteractive apt install -y --no-install-recommends \
        clang-format \
        clang-tidy \
        cmake \
        doxygen \
        g++ \
        gcc \
        gdb \
        python3-pip \
    && rm -rf /var/lib/apt/lists/*
