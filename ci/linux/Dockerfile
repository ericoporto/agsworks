ARG FROM_DEBIAN=debian:latest
ARG FROM_PLATFORM=$BUILDPLATFORM
FROM --platform=$FROM_PLATFORM $FROM_DEBIAN

# Take default debconf options
ENV DEBIAN_FRONTEND noninteractive

RUN echo "deb [trusted=yes] http://archive.debian.org/debian jessie main non-free contrib" > /etc/apt/sources.list && \
    echo 'deb-src [trusted=yes] http://archive.debian.org/debian/ jessie main non-free contrib'  >> /etc/apt/sources.list && \
    echo 'deb [trusted=yes] http://archive.debian.org/debian-security/ jessie/updates main non-free contrib'  >> /etc/apt/sources.list

# Configure apt defaults
ARG APT_CONF_LOCAL=99local
RUN mkdir -p /etc/apt/apt.conf.d && \
  printf 'APT::Get::Assume-Yes "true";\n\
APT::Get::Install-Recommends "false";\n\
APT::Get::Install-Suggests "false";\n' > /etc/apt/apt.conf.d/$APT_CONF_LOCAL

# Upgrade existing packages
RUN apt-get update && apt-get upgrade

# Get curl and ssl for cmake fetches
RUN apt-get install curl libcurl4-openssl-dev libssl-dev

# a lot of these dependencies I really don't need but I will leave them here in case I change my mind later...
# Get build dependencies
RUN apt-get install \
  build-essential \
  debhelper \
  autoconf \
  dh-autoreconf \
  git \
  libogg-dev \
  libtheora-dev \
  libvorbis-dev \
  zlib1g-dev \
  pkg-config

# Get SDL2 build dependencies
RUN apt-get install \
  libasound2-dev \
  libdbus-1-dev \
  libegl1-mesa-dev \
  libgl1-mesa-dev \
  libgles2-mesa-dev \
  libglu1-mesa-dev \
  libibus-1.0-dev \
  libpulse-dev \
  libsndio-dev \
  libudev-dev \
  libwayland-dev \
  libx11-dev \
  libxcursor-dev \
  libxext-dev \
  libxi-dev \
  libxinerama-dev \
  libxkbcommon-dev \
  libxrandr-dev \
  libxss-dev \
  libxt-dev \
  libxv-dev \
  libxxf86vm-dev \
  libwayland-dev \
  libxkbcommon-dev

RUN ldconfig -v