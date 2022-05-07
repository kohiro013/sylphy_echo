FROM debian:bullseye-slim

ENV ARM_TOOLCHAIN_VERSION 10.3-2021.10
ENV GOOGLE_TEST_VERSION 1.11.0

ENV DEBIAN_FRONTEND=noninteractive
ENV DISPLAY=host.docker.internal:0

# 開発に必要なパッケージをインストール(ダウンロード元のリポジトリを日本サーバーに変更)
RUN apt-get update && apt-get upgrade -y \
    && apt-get install --no-install-recommends -y \
        build-essential \
        gcc \
        make \
        cmake \
        ninja-build \
        curl \
        ca-certificates \
		git \
        doxygen \
        x11-apps \
        openocd \
        telnet \
        libncurses5-dev \
        openssh-server \
    && apt-get clean \
    && rm -r /var/lib/apt/lists/* \
    && mkdir -p /tmp/src

# arm-none-eabi-gccをインストールする
WORKDIR /tmp/src
RUN curl -Lo gcc-arm-none-eabi.tar.bz2 "https://developer.arm.com/-/media/Files/downloads/gnu-rm/${ARM_TOOLCHAIN_VERSION}/gcc-arm-none-eabi-${ARM_TOOLCHAIN_VERSION}-x86_64-linux.tar.bz2" \
    && mkdir /opt/gcc-arm-none-eabi \
    && tar xf gcc-arm-none-eabi.tar.bz2 --strip-components=1 -C /opt/gcc-arm-none-eabi \
    && echo 'export PATH=$PATH:/opt/gcc-arm-none-eabi/bin' | tee -a /etc/profile.d/gcc-arm-none-eabi.sh

# Google testをインストールする
WORKDIR /tmp/src
RUN curl -OL https://github.com/google/googletest/archive/release-${GOOGLE_TEST_VERSION}.tar.gz \
    && tar -zxvf release-${GOOGLE_TEST_VERSION}.tar.gz \
    && mkdir -p ./googletest-release-${GOOGLE_TEST_VERSION}/build \
    && cd ./googletest-release-${GOOGLE_TEST_VERSION}/build \
    && cmake ../ \
    && make \
    && make install

# インストール作業で出た一時的なファイルを削除
WORKDIR /tmp
RUN rm -rf ./src/